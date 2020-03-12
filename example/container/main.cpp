/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Any()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		container::Any any;

		any = 123;

		std::cout << any.Get<int32_t>() << std::endl;

		any = "456";

		std::cout << any.Get<const char * >() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Queue()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::string value{ };

		container::LockQueue<std::string> queue;

		queue.Push("123");
		queue.Push("456");
		queue.Push("789");

		std::cout << "***** push *****" << std::endl;
		std::cout << "size  : " << queue.Size() << std::endl;
		std::cout << "empty : " << queue.Empty() << std::endl;
		std::cout << "back  : " << queue.Back() << std::endl;
		std::cout << "front : " << queue.Front() << std::endl;

		queue.Pop();

		std::cout << "***** pop *****" << std::endl;
		std::cout << "size  : " << queue.Size() << std::endl;
		std::cout << "empty : " << queue.Empty() << std::endl;
		std::cout << "back  : " << queue.Back() << std::endl;
		std::cout << "front : " << queue.Front() << std::endl;

		queue.Pop(value);

		std::cout << "***** pop value *****" << std::endl;
		std::cout << "size  : " << queue.Size() << std::endl;
		std::cout << "empty : " << queue.Empty() << std::endl;
		std::cout << "back  : " << queue.Back() << std::endl;
		std::cout << "front : " << queue.Front() << std::endl;
		std::cout << "value : " << value << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Operation()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::vector<std::string> vec{ };

		for (int i = 0; i < 10; ++i)
		{
			vec.push_back(std::to_string(i));
		}

		std::cout << "vec count : " << vec.size() << std::endl;

		container::Operation::Swap(vec);

		std::cout << "vec count : " << vec.size() << std::endl;
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

	Any();
	Queue();
	Operation();

	return 0;
}
