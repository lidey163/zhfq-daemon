#ifndef CLOGSERVICE_HPP__
#define CLOGSERVICE_HPP__

#include "CServiceBase.hpp"

namespace servto
{

class CLogService : public CServiceBase
{
public:
	CLogService () : CServiceBase("log") {}
	virtual ~CLogService () {}

	virtual void RegisterAction()
	{
	}
private:
	/* data */
};

} /* servto */ 

#endif /* end of include guard: CLOGSERVICE_HPP__ */
