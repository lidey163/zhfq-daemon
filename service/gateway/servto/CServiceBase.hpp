#ifndef CSERVICEBASE_HPP__
#define CSERVICEBASE_HPP__

#include <string>
#include <map>
#include "errorutil.hpp"
#include "jsonutil.hpp"

namespace servto
{

typedef EINT (*FActionHander)(const json& jReq, json& jRes);

class CServiceBase
{
public:
	CServiceBase (const std::string& sName) : m_sName(sName) {}
	CServiceBase (const std::string& sName, int iPort) : m_sName(sName), m_iPort(iPort) {}
	CServiceBase (const std::string& sName, const std::string& sIP, int iPort) : m_sName(sName), m_sIP(sIP), m_iPort(iPort) {}
	virtual ~CServiceBase () {}

	virtual void RegisterAction() = 0;

	virtual EINT Init()
	{
		RegisterAction();
		return OK;
	}

	FActionHander GetActionHander(const std::string& sAction)
	{
		auto it = m_mapActionHander.find(sAction);
		if (it != m_mapActionHander.end())
		{
			return it->second;
		}
		return nullptr;
	}

protected:
	std::string m_sName;
	std::string m_sIP;
	int m_iPort;

	std::map<std::string, FActionHander> m_mapActionHander;
private:
	/* data */
};

} /* servto */ 

#endif /* end of include guard: CSERVICEBASE_HPP__ */
