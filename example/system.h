#ifndef __EXAMPLE__TEST__SYSTEM__H__
#define __EXAMPLE__TEST__SYSTEM__H__


#include "test.h"


class System
{
public:
	static void Test()
	{
		{
			std::cout << "-------------------------------------------------- os --------------------------------------------------" << std::endl;

			FUNCTION_TEST(tinyToolkit::OS::ThreadID())
			FUNCTION_TEST(tinyToolkit::OS::ProcessID())
		}

		{
			std::cout << "-------------------------------------------------- signal --------------------------------------------------" << std::endl;

			tinyToolkit::Signal::Raise(1);
		}

		{
			std::cout << "-------------------------------------------------- application --------------------------------------------------" << std::endl;

			FUNCTION_TEST(tinyToolkit::Application::Name())
			FUNCTION_TEST(tinyToolkit::Application::Path())
			FUNCTION_TEST(tinyToolkit::Application::Exist())
			FUNCTION_TEST(tinyToolkit::Application::Steam())
			FUNCTION_TEST(tinyToolkit::Application::Extension())
			FUNCTION_TEST(tinyToolkit::Application::Directory())
			FUNCTION_TEST(tinyToolkit::Application::CompileTime())
			FUNCTION_TEST(tinyToolkit::Application::CompileTimeString())
		}
	}
};


#endif // __EXAMPLE__TEST__SYSTEM__H__
