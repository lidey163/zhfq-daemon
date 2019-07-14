#ifndef CCOMPANYSERVICE_HPP__
#define CCOMPANYSERVICE_HPP__

#include "CServiceBase.hpp"

namespace servto
{

class CCompanyService : public CServiceBase
{
public:
	CCompanyService () : CServiceBase("company") {}
	virtual ~CCompanyService () {}

	virtual void RegisterAction()
	{
	}
private:
	/* data */
};

} /* servto */ 

#endif /* end of include guard: CCOMPANYSERVICE_HPP__ */
