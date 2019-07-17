#include "commonutil/SetSignalDummy.h"
#include "commonutil/CtrlCHandler.h"
#include "commonnet/BasicStream.h"

#include "App.h"
#include "CRequestHandler.hpp"
#include "errorutil.hpp"
#include "script.hpp"

#ifdef QUICK_TEST
#include "qtest/qtest.hpp"
#endif

ApplicationGlobalData g_App;

static void InterruptedCallback(int signal)
{
    g_App.ptrServer->stop_work();
}

static EINT Init()
{
	// 日志初始化
#ifndef LOGTO_STDERR
	init_log(g_App.ptrProperties->getProperty("LogService.IP"), g_App.ptrProperties->getPropertyAsInt("LogService.Port"));
#endif

	// mysql 连接池初始化
	mysql::conn_info_t info;
	info.host = g_App.ptrProperties->getProperty("MysqlServer.IP");
	info.port = g_App.ptrProperties->getPropertyAsInt("MysqlServer.Port");
	info.user = g_App.ptrProperties->getProperty("MysqlServer.User");
	info.pwd  = g_App.ptrProperties->getProperty("MysqlServer.Password").c_str();
	info.db   = g_App.ptrProperties->getProperty("MysqlServer.Database");
	info.charset = g_App.ptrProperties->getProperty("MysqlServer.CharsetName");
	info.client_ip = g_App.ptrProperties->getProperty("MysqlServer.ClientIP");

	g_App.g_pool = new mysql::ConnectionPool(info, -1);
	g_App.g_pool->init();
	g_App.g_conn_delter.setPool(g_App.g_pool);

	g_App.serviceMgr.Init();
	std::string strScriptDir = g_App.ptrProperties->getProperty("Script.BaseDir");
	script::Init(strScriptDir);
    return OK;
}

int main(int argc, char* argv[])
{
    commonutil::CtrlCHandler ctrlCHandler;
    ctrlCHandler.setCallback(InterruptedCallback);
    
    commonutil::SetSignalDummy();
    
    commonutil::PropertiesPtr ptrProperties = new commonutil::Properties();
    ptrProperties->load(commonutil::Properties::getPropertyPath());
	g_App.ptrProperties = ptrProperties;
    
	log_trace("start to init: %s", argv[0]);
    Init();
	log_trace("finish to init: %s", argv[0]);
    
#ifdef QUICK_TEST
	log_trace("will run qtest before server ...");
	qtest::Run();
#endif

	log_trace("will start server ...");
    g_App.ptrTimer = new commonutil::Timer();
    g_App.ptrSelectorThread = new commonnet::SelectorThread(g_App.ptrTimer);
    g_App.ptrServer = new t_fastcgi_server(ptrProperties->getProperty("Local.AllIP"),
                                                       ptrProperties->getPropertyAsInt("Gateway.Port"),
                                                       8,
                                                       g_App.ptrSelectorThread,
													   &g_App.requestHandler);
    g_App.ptrServer->start_work();
    g_App.ptrServer->join();
    g_App.ptrSelectorThread->destroy();
    g_App.ptrTimer->destroy();
    g_App.ptrSelectorThread->joinWithThread();

	delete g_App.ptrServer;
	delete g_App.g_pool;
}

