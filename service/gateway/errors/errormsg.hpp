#ifndef ERRORMSG_HPP__
#define ERRORMSG_HPP__

#include <string>
#include "errorcode.hpp"

namespace errors
{
EINT Init();
std::string GetErrorMsg(EINT error);
} /* errors */ 

#endif /* end of include guard: ERRORMSG_HPP__ */
