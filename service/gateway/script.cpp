#include "script.hpp"

#include <unistd.h>
#include <stdio.h>

#include "commonutil/api_logger.h"

// 私有声明
namespace script
{
const int S_MAX_OUTPUT_BUFF = 1024;
std::string s_script_dir;
std::string findScript(const std::string& to, const std::string& action);
}

// 公共接口
namespace script
{

EINT Init(const std::string& strScriptDir)
{
	s_script_dir = strScriptDir;
	log_trace("set s_script_dir = %s", s_script_dir.c_str());
	return 0;
}

EINT FindRun(const std::string& to, const std::string& action, const json& jReq, json& jRet)
{
	std::string strPath = findScript(to, action);
	if (strPath.empty())
	{
		return ERR_SCRIPT_NOT_FOUND;
	}

	std::string strReq = jReq.dump();
	std::string strCmd = strPath + " '" + strReq + "' 2>/dev/null";
	log_trace("will popen: %s", strCmd.c_str());
	FILE *pStream = popen(strCmd.c_str(), "r");
	if (pStream == NULL)
	{
		log_error("popen cmd failed");
		return ERR_SCRIPT_EXE_FAILED;
	}

	char buff[S_MAX_OUTPUT_BUFF] = {0};
	int error = OK;
	do {
		int iRead = fread(buff, 1, S_MAX_OUTPUT_BUFF-1, pStream);
		if (iRead == 0)
		{
			log_error("script seems no output");
			error = ERR_SCRIPT_EXE_FAILED;
			break;
		}
		else if (iRead >= S_MAX_OUTPUT_BUFF-1)
		{
			log_error("script seems too much output");
		}
		buff[iRead] = '\0';

		try
		{
			jRet = json::parse(buff);
			error = jRet.value("error", OK);
		}
		catch (json::parse_error& e)
		{
			std::string strMsg = e.what();
			log_error("json parse exception: %s", strMsg.c_str());
			error = ERR_RESPONSE_NOJSON;
			break;
		}
	} while(0);

	pclose(pStream);
	return error;
}


}

// 私有声明
namespace script
{

std::string findScript(const std::string& to, const std::string& action)
{
	std::string strPath = s_script_dir + '/' + to + '/' + action;
	if (access(strPath.c_str(), R_OK | X_OK) == OK)
	{
		return strPath;
	}
	log_error("the script can not read or executable: %s", strPath.c_str());
	return "";
}

}

