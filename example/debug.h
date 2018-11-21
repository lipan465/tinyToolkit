#ifndef __EXAMPLE__TEST__DEBUG__H__
#define __EXAMPLE__TEST__DEBUG__H__


#include "test.h"


class Debug
{
public:
	static void Test()
	{
		{
			std::cout << "-------------------------------------------------- trace --------------------------------------------------" << std::endl;

			try
			{
				TINY_TOOLKIT_DEBUG("this is {}", "debug")
			}
			catch (const tinyToolkit::IException & e)
			{
				std::cout << "what : " << e.what() << std::endl;
				std::cout << "option : " << e.Option() << std::endl;
				std::cout << "message : " << e.Message() << std::endl;
			}

			try
			{
				TINY_TOOLKIT_FATAL("this is {}", "fatal")
			}
			catch (const tinyToolkit::IException & e)
			{
				std::cout << "what : " << e.what() << std::endl;
				std::cout << "option : " << e.Option() << std::endl;
				std::cout << "message : " << e.Message() << std::endl;
			}

			try
			{
				TINY_TOOLKIT_ASSERT(false, "this is {}", "assert")
			}
			catch (const tinyToolkit::IException & e)
			{
				std::cout << "what : " << e.what() << std::endl;
				std::cout << "option : " << e.Option() << std::endl;
				std::cout << "message : " << e.Message() << std::endl;
			}
		}

		{
			std::cout << "-------------------------------------------------- watcher --------------------------------------------------" << std::endl;

			tinyToolkit::ValueWatcher watcher{ };

			watcher.AddCallBack("t1", [](int oldValue, int newValue){ tinyToolkit::String::Print("t1 : {} --> {}\n", oldValue, newValue); });
			watcher.AddCallBack("t2", [](int oldValue, int newValue){ tinyToolkit::String::Print("t2 : {} --> {}\n", oldValue, newValue); });

			for (int i = 0; i < 10; ++i) {
				watcher.Set("t1", i * 10 - 1);
				watcher.Set("t2", i * 20 - 2);
			}

			watcher.Print();
		}

		{
			std::cout << "-------------------------------------------------- exception --------------------------------------------------" << std::endl;

			try
			{
				TINY_TOOLKIT_EXCEPTION_THROW(tinyToolkit::NotFoundException)
			}
			catch (tinyToolkit::IException & e)
			{
				std::cout << "what : " << e.what() << std::endl;
				std::cout << "option : " << e.Option() << std::endl;
				std::cout << "message : " << e.Message() << std::endl;

				e.PrintStackTrace();
			}

			std::cerr << std::endl << std::endl;

			TINY_TOOLKIT_EXCEPTION_THROW(tinyToolkit::NotFoundException, "this is message", "this is option")
		}
	}
};


#endif // __EXAMPLE__TEST__DEBUG__H__
