#include "CRequestHandler.h"
#include "App.h"
// #include "commonnet/BasicStream.h"
// #include "commonnet/Transceiver.h"
#include "commonutil/GlobalUtil.h"
#include "json/json.h"
#include <errno.h>

void CRequestHandler::HandleRequest(const commonnet::TransceiverPtr& ptrTransceiver, const t_fastcgi_request_ref &req)
{
    t_fastcgi_reply_ref reply = new t_fastcgi_reply;
    reply->set_cookies(req->cookies());
    int status = HandleFastcgi(req, reply);
	if (status != 0)
	{
		log_debug("HandleFastcgi finished on status: %s", status);
	}
	std::string reply_data = reply->serialize();
	ptrTransceiver->write(reply_data.c_str(), reply_data.size());
}

int CRequestHandler::HandleFastcgi(const t_fastcgi_request_ref &req, const t_fastcgi_reply_ref &reply)
{
    int error = 0;
	Json::Value jreply;
    do {
        std::string clientIp = GetRealClientIp(req);
        log_trace("clientIp:%s", clientIp.c_str());

        std::string reqdata = req->data();
        Json::Value root;
        Json::Reader reader;
        if (!reader.parse(reqdata, root)) {
            error = -1;
            log_error("%s is not json data", reqdata.c_str());
            break;
        }

        log_trace("%s", reqdata.c_str());
        if (!root.isObject()) {
            log_error("%s is not object data", reqdata.c_str());
            error = -1;
            break;
        }

        if (!root.isMember("from") || !root.isMember("to") || !root.isMember("action")) {
            log_error("%s arg error", reqdata.c_str());
            error = -1;
            break;
        }
        if (!root["from"].isString()
            || !root["to"].isString()
            || !root["action"].isString()) {
            log_error("arg error");
            error = -1;
            break;
        }

        if (root.isMember("info") && !root["info"].isObject()) {
            log_trace("arg error");
            error = -1;
            break;
        }

        if (root.isMember("data") && !root["data"].isObject()) {
            log_trace("data arg error");
            error = -1;
            break;
        }
        
        std::string to = root["to"].asString();
        std::string from = root["from"].asString();
        std::string action = root["action"].asString();

        root["info"]["client_ip"] = clientIp;

		jreply["from"] = root["from"];
		jreply["to"] = root["to"];
		jreply["info"] = root["info"];
		jreply["action"] = action;
    } while (0);

	jreply["error"] = error;
	std::string backData = jreply.toString();
	log_trace("res=%s", backData.c_str());
	reply->set_data(backData);

	reply->set_content_type(k_content_type_json);
	reply->set_charset("UTF-8");

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


