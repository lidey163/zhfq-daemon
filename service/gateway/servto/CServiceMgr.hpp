#ifndef CSERVICEMGR_HPP__
#define CSERVICEMGR_HPP__

#include "CServiceBase.hpp"

#include "CGidService.hpp"
#include "CLogService.hpp"
#include "CUserService.hpp"
#include "CPersonService.hpp"
#include "CCompanyService.hpp"
#include "CDebugService.hpp"

namespace servto
{

class CServiceMgr
{
public:
	CServiceMgr () {}
	~CServiceMgr () {}

	EINT Init()
	{
		m_mapService["gid"] = &m_gidService;
		m_mapService["log"] = &m_logService;
		m_mapService["user"] = &m_userService;
		m_mapService["person"] = &m_personService;
		m_mapService["company"] = &m_companyService;
		m_mapService["debug"] = &m_debugService;

		for (auto& pService : m_mapService)
		{
			pService.second->Init();
		}
		
		return OK;
	}

	CServiceBase* GetService(const std::string& sName)
	{
		auto it = m_mapService.find(sName);
		if (it != m_mapService.end())
		{
			return it->second;
		}
		return nullptr;
	}

private:
	std::map<std::string, CServiceBase*> m_mapService;

	CGidService m_gidService;
	CLogService m_logService;
	CUserService m_userService;
	CPersonService m_personService;
	CCompanyService m_companyService;
	CDebugService m_debugService;
};

} /* servto */ 

#endif /* end of include guard: CSERVICEMGR_HPP__ */
