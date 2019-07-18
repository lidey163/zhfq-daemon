#include "errormsg.hpp"
#include <commonutil/api_logger.h>
#include <map>

class CErrorCodeMgr
{
public:
	CErrorCodeMgr();
	std::string GetErrorMsg(EINT error);
private:
	void init();
	std::map<EINT, std::string> m_mapErrorCode;
};

static CErrorCodeMgr s_errorCodeMgr;

CErrorCodeMgr::CErrorCodeMgr()
{
	init();
}

#define GENERATE_ERROR_MESSAGE 1
void CErrorCodeMgr::init()
{
#ifdef GENERATE_ERROR_MESSAGE
#include "_errormsg.inc"
#endif
}

std::string CErrorCodeMgr::GetErrorMsg(EINT error)
{
	std::string errorStr;
	std::map<EINT, std::string>::iterator it = m_mapErrorCode.find(error);
	if (it != m_mapErrorCode.end())
	{
		errorStr = it->second;
	}
	else
	{
		errorStr = "未知错误";
	}

	if (error != OK)
	{
		log_error("errorCode=%d, errorStr=%s", error, errorStr.c_str());
	}
	return errorStr;
}

namespace errors
{

EINT Load()
{
	return OK;
}

std::string GetErrorMsg(EINT error)
{
	return s_errorCodeMgr.GetErrorMsg(error);
}

} /* errors */ 

