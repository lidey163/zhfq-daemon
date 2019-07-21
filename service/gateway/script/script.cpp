#include "script.hpp"

#include <unistd.h>
#include <stdlib.h>
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
	log_trace("will call system: %s", strCmd.c_str());

	int fd[2];
	if(pipe(fd))
	{
		log_error("pipe error!\n");
		return ERR_SCRIPT_EXE_FAILED;
	}

	EINT error = OK;
	char buff[S_MAX_OUTPUT_BUFF] = {0};
	//hide stdout
	int bak_fd = dup(STDOUT_FILENO);
	int new_fd = dup2(fd[1], STDOUT_FILENO);
	do {
		error = system(strCmd.c_str());
		log_trace("exit status: system[%d], script[%d]", error, WEXITSTATUS(error));

		if (error == -1)
		{
			log_error("system failed");
			error = ERR_SCRIPT_EXE_FAILED;
			break;
		}
		if (error)
		{
			error = WEXITSTATUS(error);
			log_error("script exit with normal error code");
			break;
		}

		int iRead = read(fd[0], buff, S_MAX_OUTPUT_BUFF-1);
		if (iRead >= S_MAX_OUTPUT_BUFF-1)
		{
			log_error("script seems too much output");
		}
		if (iRead == 0)
		{
			log_error("script seems no output");
			error = ERR_SCRIPT_EXE_FAILED;
			break;
		}

		try
		{
			jRet = json::parse(buff);
			error = jRet.value("error", OK);
		}
		catch (json::parse_error& e)
		{
			log_error("system output: %s", buff);
			log_error("json parse exception: %s", e.what());
			error = ERR_RESPONSE_NOJSON;
		}

	} while(0);

	//resume stdout
	dup2(bak_fd, new_fd);

	return error;
}


}

// 私有实现
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
