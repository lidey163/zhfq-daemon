#include "CServiceBase.hpp"

namespace servto
{
	
FActionHander CServiceBase::GetActionHander(const std::string& sAction)
{
	auto it = m_mapActionHander.find(sAction);
	if (it != m_mapActionHander.end())
	{
		return it->second;
	}
	return nullptr;
}

void CServiceBase::AddAction(const std::string& sAction, FActionHander funHander)
{
	m_mapActionHander[sAction] = funHander;
}

bool CServiceBase::HasAction()
{
	return !m_mapActionHander.empty();
}

bool CServiceBase::HasAction(const std::string& sAction)
{
	return m_mapActionHander.count(sAction) > 0;
}

} /* servto */ 
