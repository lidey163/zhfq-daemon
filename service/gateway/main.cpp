#include "commonutil/SetSignalDummy.h"
#include "commonutil/CtrlCHandler.h"
#include "commonnet/BasicStream.h"

#include "App.h"
#include "CRequestHandler.h"
#include "errorutil.hpp"
#include "script.hpp"

ApplicationGlobalData g_App;

static void InterruptedCallback(int signal)
{
    g_App.ptrServer->stop_work();
}

static EINT Init()
{
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
    
	log_trace("before init_log");
    init_log(ptrProperties->getProperty("LogService.IP"), ptrProperties->getPropertyAsInt("LogService.Port"));
	log_trace("after init_log");
    
    mysql::conn_info_t info;
    info.host = ptrProperties->getProperty("MysqlServer.IP");
    info.port = ptrProperties->getPropertyAsInt("MysqlServer.Port");
    info.user = ptrProperties->getProperty("MysqlServer.User");
    info.pwd  = ptrProperties->getProperty("MysqlServer.Password").c_str();
    info.db   = ptrProperties->getProperty("MysqlServer.Database");
    info.charset = ptrProperties->getProperty("MysqlServer.CharsetName");
    info.client_ip = ptrProperties->getProperty("MysqlServer.ClientIP");
    
    g_App.g_pool = new mysql::ConnectionPool(info, -1);
    g_App.g_pool->init();
    g_App.g_conn_delter.setPool(g_App.g_pool);

    Init();
    
    g_App.ptrTimer = new commonutil::Timer();
    g_App.ptrSelectorThread = new commonnet::SelectorThread(g_App.ptrTimer);
    
    CRequestHandler requestHandler;
    g_App.ptrServer = new t_fastcgi_server(ptrProperties->getProperty("Local.AllIP"),
                                                       ptrProperties->getPropertyAsInt("Gateway.Port"),
                                                       8,
                                                       g_App.ptrSelectorThread,
                                                       &requestHandler);
    g_App.ptrServer->start_work();
    g_App.ptrServer->join();
    g_App.ptrSelectorThread->destroy();
    g_App.ptrTimer->destroy();
    g_App.ptrSelectorThread->joinWithThread();

	delete g_App.ptrServer;
	delete g_App.g_pool;
}

