/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void OS()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::cout << "ThreadID       : " << tinyToolkit::OS::ThreadID() << std::endl;
		std::cout << "ProcessID      : " << tinyToolkit::OS::ProcessID() << std::endl;
		std::cout << "ProcessorCount : " << tinyToolkit::OS::ProcessorCount() << std::endl;
		std::cout << "UserName       : " << tinyToolkit::OS::UserName() << std::endl;
		std::cout << "ComputerName   : " << tinyToolkit::OS::ComputerName() << std::endl;
		std::cout << "IsBigEndian    : " << tinyToolkit::OS::IsBigEndian() << std::endl;
		std::cout << "IsLittleEndian : " << tinyToolkit::OS::IsLittleEndian() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}


static void Signal()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		tinyToolkit::Signal::RegisterIgnore();

		tinyToolkit::Signal::RegisterTerminate([](int signalNo){ std::cout << "Terminate : " << signalNo << std::endl; });
		tinyToolkit::Signal::RegisterException([](int signalNo){ std::cout << "Exception : " << signalNo << std::endl; });

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		tinyToolkit::Signal::Raise(SIGILL);
		tinyToolkit::Signal::Raise(SIGFPE);
		tinyToolkit::Signal::Raise(SIGABRT);
		tinyToolkit::Signal::Raise(SIGSEGV);

		tinyToolkit::Signal::Raise(SIGINT);
		tinyToolkit::Signal::Raise(SIGTERM);
		tinyToolkit::Signal::Raise(SIGBREAK);

#else

		tinyToolkit::Signal::Raise(SIGHUP);
		tinyToolkit::Signal::Raise(SIGPIPE);
		tinyToolkit::Signal::Raise(SIGCHLD);

		tinyToolkit::Signal::Raise(SIGILL);
		tinyToolkit::Signal::Raise(SIGFPE);
		tinyToolkit::Signal::Raise(SIGABRT);
		tinyToolkit::Signal::Raise(SIGSEGV);

		tinyToolkit::Signal::Raise(SIGINT);
		tinyToolkit::Signal::Raise(SIGQUIT);
		tinyToolkit::Signal::Raise(SIGTERM);
		tinyToolkit::Signal::Raise(SIGTSTP);

#endif
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}


static void Application()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::cout << "Exist             : " << tinyToolkit::Application::Exist() << std::endl;
		std::cout << "Path              : " << tinyToolkit::Application::Path() << std::endl;
		std::cout << "Directory         : " << tinyToolkit::Application::Directory() << std::endl;
		std::cout << "Name              : " << tinyToolkit::Application::Name() << std::endl;
		std::cout << "Steam             : " << tinyToolkit::Application::Steam() << std::endl;
		std::cout << "Extension         : " << tinyToolkit::Application::Extension() << std::endl;
		std::cout << "CompileTime       : " << tinyToolkit::Application::CompileTime() << std::endl;
		std::cout << "CompileTimeString : " << tinyToolkit::Application::CompileTimeString() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char const * argv[])
{
	(void)argc;
	(void)argv;

	OS();
	Signal();
	Application();

	return 0;
}
