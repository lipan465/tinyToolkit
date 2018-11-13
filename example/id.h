#ifndef __EXAMPLE__TEST__ID__H__
#define __EXAMPLE__TEST__ID__H__


#include "test.h"


class ID
{
public:
	static void Test()
	{
		{
			std::cout << "-------------------------------------------------- unique --------------------------------------------------" << std::endl;

			FUNCTION_TEST(tinyToolkit::UniqueID::Get())
			FUNCTION_TEST(tinyToolkit::UniqueID::Get())
			FUNCTION_TEST(tinyToolkit::UniqueID::Get())
			TINY_TOOLKIT_SLEEP_S(1)
			FUNCTION_TEST(tinyToolkit::UniqueID::Get())
			FUNCTION_TEST(tinyToolkit::UniqueID::Get())
			FUNCTION_TEST(tinyToolkit::UniqueID::Get())
			TINY_TOOLKIT_SLEEP_MS(10)
			FUNCTION_TEST(tinyToolkit::UniqueID::Get())
			FUNCTION_TEST(tinyToolkit::UniqueID::Get())
			FUNCTION_TEST(tinyToolkit::UniqueID::Get())
		}

		{
			std::cout << "-------------------------------------------------- snowflake --------------------------------------------------" << std::endl;

			FUNCTION_TEST(tinyToolkit::Snowflake::Get(0, 0))
			FUNCTION_TEST(tinyToolkit::Snowflake::Get(0, 0))
			FUNCTION_TEST(tinyToolkit::Snowflake::Get(0, 0))

			FUNCTION_TEST(tinyToolkit::Snowflake::Get(0, 1))
			FUNCTION_TEST(tinyToolkit::Snowflake::Get(0, 1))
			FUNCTION_TEST(tinyToolkit::Snowflake::Get(0, 1))

			FUNCTION_TEST(tinyToolkit::Snowflake::Get(1, 0))
			FUNCTION_TEST(tinyToolkit::Snowflake::Get(1, 0))
			FUNCTION_TEST(tinyToolkit::Snowflake::Get(1, 0))

			FUNCTION_TEST(tinyToolkit::Snowflake::Get(1, 1))
			FUNCTION_TEST(tinyToolkit::Snowflake::Get(1, 1))
			FUNCTION_TEST(tinyToolkit::Snowflake::Get(1, 1))
		}
	}
};


#endif // __EXAMPLE__TEST__ID__H__
