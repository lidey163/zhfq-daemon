#ifndef CSERVICEBASE_HPP__
#define CSERVICEBASE_HPP__

#include <string>
#include <map>
#include "errorutil.hpp"
#include "jsonutil.hpp"

namespace servto
{

typedef EINT (*FActionHander)(const json& jReq, json& jRet);

class CServiceBase
{
public:
	CServiceBase (const std::string& sName) : m_sName(sName) {}
	CServiceBase (const std::string& sName, int iPort) : m_sName(sName), m_iPort(iPort) {}
	CServiceBase (const std::string& sName, const std::string& sIP, int iPort) : m_sName(sName), m_sIP(sIP), m_iPort(iPort) {}
	~CServiceBase () {}

	const std::string& Name() { return m_sName; }

	FActionHander GetActionHander(const std::string& sAction);
	void AddAction(const std::string& sAction, FActionHander funHander);
	bool HasAction();
	bool HasAction(const std::string& sAction);

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
