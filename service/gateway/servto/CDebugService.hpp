#ifndef CDEBUGSERVICE_HPP__
#define CDEBUGSERVICE_HPP__

#include "CServiceBase.hpp"
#include "action/debug.hpp"

namespace servto
{

class CDebugService : public CServiceBase
{
public:
	CDebugService () : CServiceBase("debug") {}
	virtual ~CDebugService () {}

	virtual void RegisterAction()
	{
		m_mapActionHander["hello"] = &action::on_hello;
		m_mapActionHander["echo"] = &action::on_echo;
	}
private:
	/* data */
};

} /* servto */ 


#endif /* end of include guard: CDEBUGSERVICE_HPP__ */
