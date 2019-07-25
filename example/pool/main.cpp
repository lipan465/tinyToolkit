/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Task()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		tinyToolkit::TaskPool pool;

		pool.Launch(2);

		pool.Submit([](){ std::cout << "task1 [" << std::this_thread::get_id() << "]" << std::endl; TINY_TOOLKIT_SLEEP_MS(100); });
		pool.Submit([](){ std::cout << "task2 [" << std::this_thread::get_id() << "]" << std::endl; TINY_TOOLKIT_SLEEP_MS(200); });
		pool.Submit([](){ std::cout << "task3 [" << std::this_thread::get_id() << "]" << std::endl; TINY_TOOLKIT_SLEEP_MS(300); });
		pool.Submit([](){ std::cout << "task4 [" << std::this_thread::get_id() << "]" << std::endl; TINY_TOOLKIT_SLEEP_MS(400); });
		pool.Submit([](){ std::cout << "task5 [" << std::this_thread::get_id() << "]" << std::endl; TINY_TOOLKIT_SLEEP_MS(500); });
		pool.Submit([](){ std::cout << "task6 [" << std::this_thread::get_id() << "]" << std::endl; TINY_TOOLKIT_SLEEP_MS(600); });
		pool.Submit([](){ std::cout << "task7 [" << std::this_thread::get_id() << "]" << std::endl; TINY_TOOLKIT_SLEEP_MS(700); });
		pool.Submit([](){ std::cout << "task8 [" << std::this_thread::get_id() << "]" << std::endl; TINY_TOOLKIT_SLEEP_MS(800); });
		pool.Submit([](){ std::cout << "task9 [" << std::this_thread::get_id() << "]" << std::endl; TINY_TOOLKIT_SLEEP_MS(900); });

		pool.Pause();

		TINY_TOOLKIT_SLEEP_S(2);

		pool.Resume();

		pool.Wait();
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}


static void Callback()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		tinyToolkit::CallbackPool<void, double_t, double_t> pool;

		auto t1 = pool.Register([&](double_t x, double_t y){ std::cout << std::setw(2) << "addition       : " << x + y << std::endl; });
		auto t2 = pool.Register([&](double_t x, double_t y){ std::cout << std::setw(2) << "subtraction    : " << x - y << std::endl; });
		auto t3 = pool.Register([&](double_t x, double_t y){ std::cout << std::setw(2) << "multiplication : " << x * y << std::endl; });
		auto t4 = pool.Register([&](double_t x, double_t y){ std::cout << std::setw(2) << "division       : " << x / y << std::endl; });
		auto t5 = pool.Register([&](double_t x, double_t y){ std::cout << std::endl; (void)x; (void)y; });

		pool.Call(23, 78);

		pool.UnRegister(t1);
		pool.UnRegister(t2);

		pool.Call(49, 13);

		pool.UnRegister(t3);
		pool.UnRegister(t4);

		pool.Call(21, 77);

		pool.UnRegister();

		pool.Call(85, 61);

		pool.UnRegister(t4);
		pool.UnRegister(t5);

		pool.Call(77, 93);
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
		class A
		{
		public:
			explicit A(const char * name) : _name(name)
			{
				std::cout << "constructor : " << _name << std::endl;
			}

			~A()
			{
				std::cout << "destructor : " << _name << std::endl;
			}

			void Print()
			{
				std::cout << "Print : " << _name << std::endl;
			}

		protected:
			std::string _name;
		};

		tinyToolkit::ApplicationPool<A> pool;

		auto t1 = pool.Create("t1");
		auto t2 = pool.Create("t2");

		t1->Print();
		t2->Print();

		pool.Recover(t1);
		pool.Recover(t2);
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

	Task();
	Callback();
	Application();

	return 0;
}
