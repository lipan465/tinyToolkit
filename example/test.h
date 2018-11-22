#ifndef __EXAMPLE__TEST__TEST__H__
#define __EXAMPLE__TEST__TEST__H__


#include "../include/tinyToolkit/tinyToolkit.h"


using namespace tinyToolkit;


#define FUNCTION_TEST(func)																	\
{																							\
	std::cout	<< std::setw(100) << std::right << #func << "    " << func << std::endl;	\
}


#endif // __EXAMPLE__TEST__TEST__H__
