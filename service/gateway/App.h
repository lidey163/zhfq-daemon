#ifndef GATEWAY_APP_H
#define GATEWAY_APP_H

#include "commonutil/Properties.h"
#include "commonutil/Timer.h"
#include "commonutil/api_logger.h"
#include "commonutil/api_mysql.h"
#include "fastcgi/fastcgi_server.hpp"
#include "servto/CServiceMgr.hpp"
#include "CRequestHandler.hpp"

struct ApplicationGlobalData
{
	commonutil::PropertiesPtr ptrProperties;
	commonnet::SelectorThreadPtr ptrSelectorThread;
	commonutil::TimerPtr ptrTimer;
	t_fastcgi_server *ptrServer;
	CRequestHandler requestHandler;

	mysql::ConnectionPool *g_pool;
	PoolDeleter<mysql::Connection> g_conn_delter;

	servto::CServiceMgr serviceMgr;

};

extern ApplicationGlobalData g_App;

#endif


