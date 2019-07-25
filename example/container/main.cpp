/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Queue()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		int value = 100;

		tinyToolkit::LockQueue<int> queue;

		queue.Push(1);
		queue.Push(2);
		queue.Push(value);

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
		std::cerr << e.what() << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char const * argv[])
{
	(void)argc;
	(void)argv;

	Queue();

	return 0;
}
