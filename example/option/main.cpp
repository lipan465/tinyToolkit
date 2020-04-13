/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Options()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		option::Option option;

		auto group_1 = std::make_shared<option::DescriptionGroup>("group_1");
		auto group_2 = std::make_shared<option::DescriptionGroup>("group_2");
		auto group_3 = std::make_shared<option::DescriptionGroup>("group_3");
		auto group_4 = std::make_shared<option::DescriptionGroup>("group_4");

		group_1->AddOption()
		("help,h", "help message");

		group_2->AddOption()
		("float,f", "float message", option::Value<float>())
		("double,d", "double message", option::Value<double>());

		group_3->AddOption()
		("int8_t,i8", "int8_t message", option::Value<int8_t>())
		("uint8_t,u8", "uint8_t message", option::Value<uint8_t>())
		("int16_t,i16", "int16_t message", option::Value<int16_t>())
		("uint16_t,u16", "uint16_t message", option::Value<uint16_t>())
		("int32_t,i32", "int32_t message", option::Value<int32_t>())
		("uint32_t,u32", "uint32_t message", option::Value<uint32_t>())
		("int64_t,i64", "int64_t message", option::Value<int64_t>())
		("uint64_t,u64", "uint64_t message", option::Value<uint64_t>());

		group_4->AddOption()
		("string,s", "string message", option::Value<std::string>());

		option.AddDescriptionGroup(group_1);
		option.AddDescriptionGroup(group_2);
		option.AddDescriptionGroup(group_3);
		option.AddDescriptionGroup(group_4);

		const char * argv[] =
		{
			"option_example",
			"--help",
			"-d=0.02",
			"--int32_t=666",
			"-s=hello"
		};

		option.Parse(sizeof(argv) / sizeof(const char *), argv);

		if (option.Exits("help"))
		{
			std::cout << option.Verbose() << std::endl;
		}

		#define SHOW_VALUE(key, value) \
			if (option.Exits(key)) \
			{ \
				std::cout << option.Get<value>(key) << std::endl; \
			}

		SHOW_VALUE("f", float)
		SHOW_VALUE("double", double)
		SHOW_VALUE("i8", int8_t)
		SHOW_VALUE("u8", uint8_t)
		SHOW_VALUE("int16_t", int16_t)
		SHOW_VALUE("uint16_t", uint16_t)
		SHOW_VALUE("i32", int32_t)
		SHOW_VALUE("u32", uint32_t)
		SHOW_VALUE("int64_t", int64_t)
		SHOW_VALUE("uint64_t", uint64_t)
		SHOW_VALUE("s", std::string)
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

	Options();

	return 0;
}
