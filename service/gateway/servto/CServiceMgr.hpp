#ifndef CSERVICEMGR_HPP__
#define CSERVICEMGR_HPP__

#include "CServiceBase.hpp"

namespace servto
{

class CServiceMgr
{
public:
	CServiceMgr () {}
	~CServiceMgr () {}

	EINT Init();

	CServiceBase* GetService(const std::string& sName);

private:
	CServiceBase* GidService();
	CServiceBase* LogService();
	CServiceBase* DebugService();
	CServiceBase* UserService();
	CServiceBase* PersonService();
	CServiceBase* CompanyService();
private:
	std::map<std::string, CServiceBase*> m_mapService;
};

} /* servto */ 

#endif /* end of include guard: CSERVICEMGR_HPP__ */
