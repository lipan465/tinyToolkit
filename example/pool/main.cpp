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
		pool::TaskPool pool{ };

		pool.Launch();

		pool.AddTask([](){ std::cout << "task1 [" << std::this_thread::get_id() << "]" << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(1)); });
		pool.AddTask([](){ std::cout << "task2 [" << std::this_thread::get_id() << "]" << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(2)); });
		pool.AddTask([](){ std::cout << "task3 [" << std::this_thread::get_id() << "]" << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(3)); });
		pool.AddTask([](){ std::cout << "task4 [" << std::this_thread::get_id() << "]" << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(4)); });
		pool.AddTask([](){ std::cout << "task5 [" << std::this_thread::get_id() << "]" << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(5)); });
		pool.AddTask([](){ std::cout << "task6 [" << std::this_thread::get_id() << "]" << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(6)); });
		pool.AddTask([](){ std::cout << "task7 [" << std::this_thread::get_id() << "]" << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(7)); });
		pool.AddTask([](){ std::cout << "task8 [" << std::this_thread::get_id() << "]" << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(8)); });
		pool.AddTask([](){ std::cout << "task9 [" << std::this_thread::get_id() << "]" << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(9)); });

		pool.Pause();

		std::this_thread::sleep_for(std::chrono::seconds(2));

		pool.Resume();

		pool.Wait();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
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
		pool::CallbackPool<std::string, void, double, double> pool{ };

		pool.Register("task1", [&](double x, double y){ std::cout << std::setw(2) << "addition       : " << x + y << std::endl; });
		pool.Register("task2", [&](double x, double y){ std::cout << std::setw(2) << "subtraction    : " << x - y << std::endl; });
		pool.Register("task3", [&](double x, double y){ std::cout << std::setw(2) << "multiplication : " << x * y << std::endl; });
		pool.Register("task4", [&](double x, double y){ std::cout << std::setw(2) << "division       : " << x / y << std::endl; });

		pool.Call(23, 78);

		pool.UnRegister("task1");
		pool.UnRegister("task2");

		pool.Call(49, 13);

		pool.UnRegister("task3");
		pool.UnRegister("task4");

		pool.Call(21, 77);

		pool.Release();

		pool.Call(85, 61);

		pool.UnRegister("task5");
		pool.UnRegister("task6");

		pool.Call(77, 93);
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
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

		pool::ObjectPool<A> pool{ };

		auto t1 = pool.Borrow("t1");
		auto t2 = pool.Borrow("t2");

		t1->Print();
		t2->Print();

		pool.Return(t1);
		pool.Return(t2);
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
