#ifndef CGIDSERVICE_HPP__
#define CGIDSERVICE_HPP__

#include "CServiceBase.hpp"

namespace servto
{

class CGidService : public CServiceBase
{
public:
	CGidService () : CServiceBase("gid") {}
	virtual ~CGidService () {}

	virtual void RegisterAction()
	{
	}
private:
	/* data */
};

} /* servto */ 

#endif /* end of include guard: CGIDSERVICE_HPP__ */
