#ifndef CDEBUGSERVICE_HPP__
#define CDEBUGSERVICE_HPP__

#include "CServiceBase.hpp"

namespace servto
{

class CDebugService : public CServiceBase
{
public:
	CDebugService () : CServiceBase("debug") {}
	virtual ~CDebugService () {}

	virtual void RegisterAction()
	{
	}
private:
	/* data */
};

} /* servto */ 


#endif /* end of include guard: CDEBUGSERVICE_HPP__ */
