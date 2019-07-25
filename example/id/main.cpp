/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Random()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		for (int i = 0; i < 3; ++i)
		{
			std::cout << tinyToolkit::Random::IntUniformDistribution(0, 65535) << std::endl;
		}

		std::cout << std::endl;

		for (int i = 0; i < 3; ++i)
		{
			std::cout << tinyToolkit::Random::FloatUniformDistribution(0.0, 65535.0) << std::endl;
		}
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}


static void UniqueID()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		for (int i = 0; i < 3; ++i)
		{
			std::cout << tinyToolkit::UniqueID::Get() << std::endl;
		}

		std::cout << std::endl;

		for (int i = 0; i < 3; ++i)
		{
			std::cout << tinyToolkit::UniqueID::Get() << std::endl;
		}
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
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
		for (int i = 0; i < 3; ++i)
		{
			std::cout << tinyToolkit::Snowflake::Get(0, 0) << std::endl;
		}

		std::cout << std::endl;

		for (int i = 0; i < 3; ++i)
		{
			std::cout << tinyToolkit::Snowflake::Get(255, 255) << std::endl;
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

	Random();
	UniqueID();
	Snowflake();

	return 0;
}
