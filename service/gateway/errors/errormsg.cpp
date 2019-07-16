#include "errormsg.hpp"

namespace errors
{
EINT Init()
{
	return OK;
}

std::string GetErrorMsg(EINT error)
{
	return "未知错误";
}

} /* errors */ 

