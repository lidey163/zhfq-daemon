#include "./debug.hpp"
#include <string>

namespace action
{

EINT on_hello(const json& jReq, json& jRet)
{
	const json& jData = jReq["data"];
	std::string strName = jData.value("name", "World");
	jRet["data"]["greet"] = "Hello " + strName + "!";
	return OK;
}

EINT on_echo(const json& jReq, json& jRet)
{
	const json& jData = jReq["data"];
	jRet["data"] = jData;
	return OK;
}


} /* action */ 
