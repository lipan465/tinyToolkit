/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Unique()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		id::Unique unique{ };

		for (int i = 0; i < 3; ++i)
		{
			std::cout << unique.Generate() << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));

		for (int i = 0; i < 3; ++i)
		{
			std::cout << unique.Generate() << std::endl;
		}
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Snowflake()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		id::Snowflake snowflake(64, 128);

		for (int i = 0; i < 3; ++i)
		{
			std::cout << snowflake.Generate() << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));

		for (int i = 0; i < 3; ++i)
		{
			std::cout << snowflake.Generate() << std::endl;
		}
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

	Unique();
	Snowflake();

	return 0;
}
