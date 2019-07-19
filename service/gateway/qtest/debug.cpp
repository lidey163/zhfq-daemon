#include "./debug.hpp"
#include "App.h"

namespace qtest {
namespace debug {

EINT req_hello()
{
	json jRet;
	json jReq = {
		{"from", "qtest"},
		{"to", "debug"},
		{"action", "hello"},
	};

	g_App.requestHandler.HandleAction(jReq, jRet);

	jReq["data"]["name"] = "World";
	g_App.requestHandler.HandleAction(jReq, jRet);

	jReq["data"]["name"] = "world";
	jReq["data"]["count"] = 2;
	g_App.requestHandler.HandleAction(jReq, jRet);

	jReq["data"]["name"] = "Zhifengque";
	jReq["data"]["count"] = 3;
	g_App.requestHandler.HandleAction(jReq, jRet);

	return OK;
}

EINT req_echo()
{
	json jRet;
	json jReq = {
		{"from", "qtest"},
		{"to", "debug"},
		{"action", "echo"},
	};

	{
		json jReqData = {
			{"one", 1},
			{"tow", 2},
		};
		jReq["data"] = jReqData;
		g_App.requestHandler.HandleAction(jReq, jRet);
	}

	{
		json jReqData = {
			{"one", "一"},
			{"tow", "二"},
			{"three", "三"},
			{"more", {"百", "千", "万"}},
		};
		jReq["data"] = jReqData;
		g_App.requestHandler.HandleAction(jReq, jRet);
	}
	return OK;
}

EINT req_script_hello()
{
	json jRet;
	json jReq = {
		{"from", "qtest"},
		{"to", "debug"},
		{"action", "hello.pl"},
	};

	jReq["data"]["name"] = "World";
	g_App.requestHandler.HandleAction(jReq, jRet);

	jRet.clear();
	jReq["data"]["name"] = "world";
	g_App.requestHandler.HandleAction(jReq, jRet);

	jRet.clear();
	jReq["data"]["name"] = "Zhifengque";
	g_App.requestHandler.HandleAction(jReq, jRet);

	jRet.clear();
	jReq["action"] = "hello.py";
	jReq["data"]["name"] = "Zhifengque";
	g_App.requestHandler.HandleAction(jReq, jRet);

	return OK;
}

EINT Run()
{
	int iFail = 0;
	iFail += req_hello();
	iFail += req_echo();
	iFail += req_script_hello();
	return OK;
}

} /* debug */ 
} /* qtest */ 
