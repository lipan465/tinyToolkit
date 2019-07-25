/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Event()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		class TimerEvent : public tinyToolkit::ITimerEvent
		{
		public:
			~TimerEvent() override = default;

			void OnPause() override
			{
				std::cout << "Pause" << std::endl;
			}

			void OnResume() override
			{
				std::cout << "Resume" << std::endl;
			}

			void OnTrigger() override
			{
				std::cout << "Trigger : " << ++_count << std::endl;
			}

			void OnFinish(bool forced) override
			{
				std::cout << "Finish : " << forced << std::endl;
			}

		public:
			int32_t _count{ 0 };
		};

		tinyToolkit::TimerManager manager;

		auto event = std::make_shared<TimerEvent>();

		manager.Start(event.get(), -1, 1000);

		while (event->_count != 5)
		{
			TINY_TOOLKIT_YIELD();
		}

		manager.Pause(event.get());

		TINY_TOOLKIT_SLEEP_S(3);

		manager.Resume(event.get());

		TINY_TOOLKIT_SLEEP_S(3);

		manager.Kill(event.get());
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}


static void Function()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		int32_t count = 3;

		tinyToolkit::TimerManager manager;

		manager.Start([&](){ std::cout << "tick : " << --count << std::endl; }, count, 1000);

		while (count != 0)
		{
			TINY_TOOLKIT_YIELD();
		}

		count = -1;

		manager.Start([&](){ std::cout << "tick : " << ++count << std::endl; }, count, 1000);

		while (count != 10)
		{
			TINY_TOOLKIT_YIELD();
		}
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char const * argv[])
{
	(void)argc;
	(void)argv;

	Event();
	Function();

	return 0;
}
