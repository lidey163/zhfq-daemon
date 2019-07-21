#include "CServiceMgr.hpp"
#include "action/debug.hpp"

namespace servto
{
	
EINT CServiceMgr::Init()
{
	m_mapService["gid"] = GidService();
	m_mapService["log"] = LogService();
	m_mapService["debug"] = DebugService();
	m_mapService["user"] = UserService();
	m_mapService["person"] = PersonService();
	m_mapService["company"] = CompanyService();

	return OK;
}

CServiceBase* CServiceMgr::GidService()
{
	static CServiceBase s_service("gid");
	if (!s_service.HasAction())
	{
		
	}
	return &s_service;
}

CServiceBase* CServiceMgr::LogService()
{
	static CServiceBase s_service("gid");
	if (!s_service.HasAction())
	{

	}
	return &s_service;
}

CServiceBase* CServiceMgr::DebugService()
{
	static CServiceBase s_service("gid");
	if (!s_service.HasAction())
	{
		s_service.AddAction("hello", &action::on_hello);
		s_service.AddAction("echo", &action::on_echo);

	}
	return &s_service;
}

CServiceBase* CServiceMgr::UserService()
{
	static CServiceBase s_service("gid");
	if (!s_service.HasAction())
	{

	}
	return &s_service;
}

CServiceBase* CServiceMgr::PersonService()
{
	static CServiceBase s_service("gid");
	if (!s_service.HasAction())
	{

	}
	return &s_service;
}

CServiceBase* CServiceMgr::CompanyService()
{
	static CServiceBase s_service("gid");
	if (!s_service.HasAction())
	{

	}
	return &s_service;
}

CServiceBase* CServiceMgr::GetService(const std::string& sName)
{
	auto it = m_mapService.find(sName);
	if (it != m_mapService.end())
	{
		return it->second;
	}
	return nullptr;
}
} /* servto */ 
