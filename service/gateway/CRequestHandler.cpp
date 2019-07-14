#include "CRequestHandler.h"
#include "App.h"
#include "commonutil/GlobalUtil.h"
#include <errno.h>

void CRequestHandler::HandleRequest(const commonnet::TransceiverPtr& ptrTransceiver, const t_fastcgi_request_ref &req)
{
    t_fastcgi_reply_ref reply = new t_fastcgi_reply;
	int status = HandleFastcgi(req, reply);
	if (status != 0)
	{
		log_debug("HandleFastcgi finished on status: %s", status);
	}
	std::string reply_data = reply->serialize();
	ptrTransceiver->write(reply_data.c_str(), reply_data.size());
}

EINT CRequestHandler::HandleFastcgi(const t_fastcgi_request_ref &req, const t_fastcgi_reply_ref &reply)
{
	reply->set_cookies(req->cookies());
	reply->set_content_type(k_content_type_json);
	reply->set_charset("UTF-8");

    EINT error = OK;
	json jReply;

    do {
        std::string reqdata = req->data();
		log_trace("req: %s", reqdata.c_str());
		if (reqdata.empty())
		{
			error = ERR_ARGUMENT_EMPTY;
			break;
		}

        json jRequest;
		try
		{
			jRequest = json::parse(reqdata);
		}
		catch (json::parse_error& e)
		{
			std::string strMsg = e.what();
			log_error("json parse exception: %s", strMsg.c_str());
			error = ERR_DATA_NOT_JSON;
			break;
		}

        if (!jRequest.is_object()) {
            log_error("reqdata is not object data");
            error = ERR_DATA_NOT_OBJECT;
            break;
        }

		if (jRequest.empty())
		{
			log_error("requset object is empty");
			error = ERR_ARGUMENT_EMPTY;
			break;
		}

		std::string clientIp = GetRealClientIp(req);
		log_trace("clientIp:%s", clientIp.c_str());
		json& session = jRequest["session"];
		session["client_ip"] = clientIp;

		error = HandleAction(jRequest, jReply);
    } while (0);

	if (error != OK)
	{
		jReply["error"] = error;
		if (jReply.value("errstr", "").empty())
		{
			// todo: 获取错误消息配置
			jReply["errstr"] = "系统错误";
		}
	}
	else
	{
		if (jReply["data"].empty())
		{
			jReply["data"] = jReply;
			jReply["error"] = OK;
		}
	}

	std::string replyData = jReply.dump();
	log_trace("reply: %s", replyData.c_str());
	reply->set_data(replyData);

    return error;
}

EINT CRequestHandler::HandleAction(const json& jReq, json& jRet)
{
	std::string from = jReq.value("from", "");
	std::string to = jReq.value("to", "");
	std::string action = jReq.value("action", "");
	if (from.empty() || to.empty() || action.empty())
	{
		log_error("from/to/action may not exists or empty");
		return ERR_ARGUMENT_EMPTY;
	}

	// json& data = jReq["data"];

	jRet["from"] = from;
	jRet["to"] = to;
	jRet["action"] = action;
	jRet["session"] = jReq["session"];

	EINT error = OK;
	do {
		servto::CServiceBase* pService = g_App.serviceMgr.GetService(to);
		if (!pService)
		{
			error = ERR_SERVICE_NOT_FOUND;
			break;
		}

		servto::FActionHander funHander = pService->GetActionHander(action);
		if (!funHander)
		{
			error = ERR_ACTION_NOT_FOUND;
			break;
		}
		error = (*funHander)(jReq, jRet);
	} while(0);

	if (error == ERR_SERVICE_NOT_FOUND || error == ERR_ACTION_NOT_FOUND)
	{
		// todo: 调用脚本
	}

	return error;
}

std::string CRequestHandler::GetRealClientIp(const t_fastcgi_request_ref &req)
{
    std::string x_forwarded_for = req->env("X_FORWARDED_FOR");
    if (x_forwarded_for.empty()) {
        return req->remote_addr();
    }

	size_t pos = x_forwarded_for.find_first_of(',');
	if (pos == std::string::npos) {
		return x_forwarded_for;
	} else {
		return x_forwarded_for.substr(0, pos);
	}
}


