#ifndef CUSERSERVICE_HPP__
#define CUSERSERVICE_HPP__

#include "CServiceBase.hpp"

namespace servto
{

class CUserService : public CServiceBase
{
public:
	CUserService () : CServiceBase("user") {}
	virtual ~CUserService () {}

	virtual void RegisterAction()
	{
	}
private:
	/* data */
};

} /* servto */ 


#endif /* end of include guard: CUSERSERVICE_HPP__ */
