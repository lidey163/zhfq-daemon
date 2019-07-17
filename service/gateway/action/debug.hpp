#ifndef ACTION_DEBUG_HPP__
#define ACTION_DEBUG_HPP__

#include "../project.hpp"

namespace action
{
	
EINT on_hello(const json& jReq, json& jRet);
EINT on_echo(const json& jReq, json& jRet);

} /* action */ 

#endif /* end of include guard: ACTION_DEBUG_HPP__ */
