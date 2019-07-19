#ifndef SCRIPT_HPP__
#define SCRIPT_HPP__

#include <string>
#include "errors/errorcode.hpp"
#include "jsonutil.hpp"

namespace script
{

EINT Init(const std::string& strScriptDir);
EINT FindRun(const std::string& to, const std::string& action, const json& jReq, json& jRet);

} /* script */ 

#endif /* end of include guard: SCRIPT_HPP__ */
