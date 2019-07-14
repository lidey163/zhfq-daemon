#ifndef GATEWAY_AP_H
#define GATEWAY_AP_H

#include "commonutil/Properties.h"
#include "commonutil/Timer.h"
#include "commonutil/api_logger.h"
#include "commonutil/api_mysql.h"
#include "fastcgi/fastcgi_server.hpp"
#include "map"


struct ApplicationGlobalData
{
	commonutil::PropertiesPtr ptrProperties;
	commonnet::SelectorThreadPtr ptrSelectorThread;
	commonutil::TimerPtr ptrTimer;
	t_fastcgi_server *ptrServer;

	mysql::ConnectionPool *g_pool;
	PoolDeleter<mysql::Connection> g_conn_delter;

};

extern ApplicationGlobalData g_App;

#endif


