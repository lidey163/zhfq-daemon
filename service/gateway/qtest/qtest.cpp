#include "qtest.hpp"
#include "debug.hpp"

namespace qtest
{

EINT Run()
{
	qtest::debug::Run();
	return OK;
}


} /* qtest */ 
