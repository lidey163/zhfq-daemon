#include "CRequestHandler.hpp"
#include "App.h"
#include <errno.h>
#include "script.hpp"
#include "commonutil/GlobalUtil.h"

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
			error = ERR_REQUEST_EMPTY;
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
			error = ERR_REQUEST_NOJSON;
			break;
		}

		std::string clientIp = GetRealClientIp(req);
		log_trace("clientIp:%s", clientIp.c_str());
		json& session = jRequest["session"];
		session["client_ip"] = clientIp;

		error = HandleAction(jRequest, jReply);

    } while (0);

	std::string replyData = jReply.dump();
	log_trace("reply: %s", replyData.c_str());
	reply->set_data(replyData);

    return error;
}

EINT CRequestHandler::HandleAction(const json& jReq, json& jRet)
{
#ifdef QUICK_TEST
	log_trace("jReq = %s", jReq.dump().c_str());
#endif

	if (!jReq.is_object()) {
		log_error("request is not object data");
		return ERR_REQUEST_NOOBJ;
	}

	if (jReq.empty())
	{
		log_error("requset object is empty");
		return ERR_REQUEST_EMPTY;
	}

	std::string from = jReq.value("from", "");
	std::string to = jReq.value("to", "");
	std::string action = jReq.value("action", "");

	if (to.empty())
	{
		log_error("to may not exists or empty");
		return ERR_REQUEST_NOTO;
	}

	if (action.empty())
	{
		log_error("action may not exists or empty");
		return ERR_REQUEST_NOACTION;
	}

	if (jReq.find("data") == jReq.end() || !jReq["data"].is_object())
	{
		log_error("req.data not object");
		return ERR_REQUEST_NODATA;
	}

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
		log_trace("goto action hander: %s/%s", to.c_str(), action.c_str());
		error = (*funHander)(jReq, jRet);
	} while(0);

	// 转用脚本处理
	if (error == ERR_SERVICE_NOT_FOUND || error == ERR_ACTION_NOT_FOUND)
	{
		log_trace("trans to script: %s/%s", to.c_str(), action.c_str());
		error = script::FindRun(to, action, jReq, jRet);
	}

	error = HandleError(error, jRet);
	jRet["from"] = from;
	jRet["to"] = to;
	jRet["action"] = action;
	// jRet["session"] = jReq["session"];

#ifdef QUICK_TEST
	log_trace("error = %d; jRet = %s", error, jRet.dump().c_str());
#endif
	return error;
}

// 允许处理返回的 jRet 包含 data 或不包含 data
// 一般为方便不需包含 data ，处理函数直接写入 jRet["data"] ，而用
// "../error" 与 "../errstr" 表示 jRet 层的错误字段
EINT CRequestHandler::HandleError(EINT error, json& jRet)
{
	do {
		if (error != OK)
		{
			if (!jRet.empty())
			{
				log_debug("when NOT OK ignore jRet: %s", jRet.dump().c_str());
				jRet.clear();
			}
			jRet["error"] = error;
			break;
		}

		if (jRet.is_null())
		{
			log_debug("jRet still null?");
			break;
		}

		if (jRet.find("data") == jRet.end() || !jRet["data"].is_object())
		{
			EINT data_error = OK;
			std::string data_errstr;
			auto it_error = jRet.find("../error");
			if (it_error != jRet.end())
			{
				data_error = it_error->get<EINT>();
				jRet.erase("../error");
			}
			auto it_errstr = jRet.find("../errstr");
			if (it_errstr != jRet.end())
			{
				data_errstr = it_error->get<std::string>();
				jRet.erase("../errstr");
			}

			if (data_error != OK)
			{
				jRet.clear();
				jRet["error"] = data_error;
				error = data_error;
			}
			else
			{
				jRet["data"] = jRet;
			}
			if (!data_errstr.empty())
			{
				jRet["errstr"] = data_errstr;
			}
		}
		else
		{
			log_debug("response already has data object");
			if (jRet.find("error") != jRet.end())
			{
				error = jRet["error"];
			}
		}
	} while(0);

	if (jRet.find("error") == jRet.end())
	{
		jRet["error"] = error;
	}

	if (error != OK && jRet.value("errstr", "").empty())
	{
		jRet["errstr"] = errors::GetErrorMsg(error);
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


