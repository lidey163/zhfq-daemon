#ifndef CPERSONSERVICE_HPP__
#define CPERSONSERVICE_HPP__

#include "CServiceBase.hpp"

namespace servto
{

class CPersonService : public CServiceBase
{
public:
	CPersonService () : CServiceBase("person") {}
	virtual ~CPersonService () {}

	virtual void RegisterAction()
	{
	}
private:
	/* data */
};

} /* servto */ 


#endif /* end of include guard: CPERSONSERVICE_HPP__ */
