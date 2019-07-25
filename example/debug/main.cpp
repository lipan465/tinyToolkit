/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Trace()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		TINY_TOOLKIT_DEBUG("this is debug message : {}", 1234);
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	try
	{
		TINY_TOOLKIT_FATAL("this is fatal message : {}", 1234);
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	try
	{
		TINY_TOOLKIT_ASSERT(true, "this is true assert message : {}", 1234);
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	try
	{
		TINY_TOOLKIT_ASSERT(false, "this is false assert message : {}", 1234);
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}


static void CoreDump()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	tinyToolkit::CoreDump::Register();
}


static void Exception()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		TINY_TOOLKIT_EXCEPTION_THROW(tinyToolkit::IOException, "throw exception");
	}
	catch (tinyToolkit::IException & e)
	{
		std::cerr << e.what() << std::endl;
	}

	try
	{
		throw TINY_TOOLKIT_EXCEPTION_CREATE(tinyToolkit::IOException, "throw create exception");
	}
	catch (tinyToolkit::IException & e)
	{
		std::cerr << e.what() << std::endl;
	}

	try
	{
		std::rethrow_exception(TINY_TOOLKIT_EXCEPTION_CREATE_POINT(tinyToolkit::IOException, "rethrow exception"));
	}
	catch (tinyToolkit::IException & e)
	{
		std::cerr << e.what() << std::endl;
	}
}


static void StackTrace()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		tinyToolkit::StackTrace::Print();
	}
	catch (tinyToolkit::IException & e)
	{
		std::cerr << e.what() << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char const * argv[])
{
	(void)argc;
	(void)argv;

	Trace();
	CoreDump();
	Exception();
	StackTrace();

	return 0;
}
