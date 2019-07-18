#include "./debug.hpp"
#include <string>

namespace action
{

EINT on_hello(const json& jReq, json& jRet)
{
	const json& jData = jReq["data"];
	std::string strName = jData.value("name", "World");
	log_debug("receive hello name: %s", strName.c_str());
	jRet["data"]["greet"] = "Hello " + strName + "!";
	return OK;
}

EINT on_echo(const json& jReq, json& jRet)
{
	const json& jData = jReq["data"];
	// jRet["data"] = jData;
	jRet = jData;
	return OK;
}


} /* action */ 
