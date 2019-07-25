/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


TEST(INT, EQ)
{
	EXPECT_EQ(1, 0);
	EXPECT_EQ(1, 1);
	EXPECT_EQ(1, 2);
}

TEST(INT, NE)
{
	EXPECT_NE(1, 0);
	EXPECT_NE(1, 1);
	EXPECT_NE(1, 2);
}

TEST(INT, LT)
{
	EXPECT_LT(1, 0);
	EXPECT_LT(1, 1);
	EXPECT_LT(1, 2);
}

TEST(INT, LE)
{
	EXPECT_LE(1, 0);
	EXPECT_LE(1, 1);
	EXPECT_LE(1, 2);
}

TEST(INT, GT)
{
	EXPECT_GT(1, 0);
	EXPECT_GT(1, 1);
	EXPECT_GT(1, 2);
}

TEST(INT, GE)
{
	EXPECT_GE(1, 0);
	EXPECT_GE(1, 1);
	EXPECT_GE(1, 2);
}


TEST(STR, EQ)
{
	EXPECT_STR_EQ("456", "123");
	EXPECT_STR_EQ("456", "456");
	EXPECT_STR_EQ("456", "789");
}

TEST(STR, NE)
{
	EXPECT_STR_NE("456", "123");
	EXPECT_STR_NE("456", "456");
	EXPECT_STR_NE("456", "789");
}

TEST(STR, LT)
{
	EXPECT_STR_LT("456", "123");
	EXPECT_STR_LT("456", "456");
	EXPECT_STR_LT("456", "789");
}

TEST(STR, LE)
{
	EXPECT_STR_LE("456", "123");
	EXPECT_STR_LE("456", "456");
	EXPECT_STR_LE("456", "789");
}

TEST(STR, GT)
{
	EXPECT_STR_GT("456", "123");
	EXPECT_STR_GT("456", "456");
	EXPECT_STR_GT("456", "789");
}

TEST(STR, GE)
{
	EXPECT_STR_GE("456", "123");
	EXPECT_STR_GE("456", "456");
	EXPECT_STR_GE("456", "789");
}


TEST(STR_CASE, EQ)
{
	EXPECT_STR_CASE_EQ("456", "123");
	EXPECT_STR_CASE_EQ("456", "456");
	EXPECT_STR_CASE_EQ("456", "789");
}

TEST(STR_CASE, NE)
{
	EXPECT_STR_CASE_NE("456", "123");
	EXPECT_STR_CASE_NE("456", "456");
	EXPECT_STR_CASE_NE("456", "789");
}

TEST(STR_CASE, LT)
{
	EXPECT_STR_CASE_LT("456", "123");
	EXPECT_STR_CASE_LT("456", "456");
	EXPECT_STR_CASE_LT("456", "789");
}

TEST(STR_CASE, LE)
{
	EXPECT_STR_CASE_LE("456", "123");
	EXPECT_STR_CASE_LE("456", "456");
	EXPECT_STR_CASE_LE("456", "789");
}

TEST(STR_CASE, GT)
{
	EXPECT_STR_CASE_GT("456", "123");
	EXPECT_STR_CASE_GT("456", "456");
	EXPECT_STR_CASE_GT("456", "789");
}

TEST(STR_CASE, GE)
{
	EXPECT_STR_CASE_GE("456", "123");
	EXPECT_STR_CASE_GE("456", "456");
	EXPECT_STR_CASE_GE("456", "789");
}


TEST(BOOL, TRUE)
{
	EXPECT_TRUE(true);
	EXPECT_TRUE(false);
}

TEST(BOOL, FALSE)
{
	EXPECT_FALSE(true);
	EXPECT_FALSE(false);
}


class TestEnvironment : public tinyToolkit::TestEnvironment
{
public:
	void SetUp() override
	{
		try
		{
			std::cout << "TestEnvironment SetUp" << std::endl;
		}
		catch (std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	void TearDown() override
	{
		try
		{
			std::cout << "TestEnvironment TearDown" << std::endl;
		}
		catch (std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char const * argv[])
{
	tinyToolkit::OptionManager::Instance().Parse(argc, argv);

	TINY_TOOLKIT_ADD_TEST_ENVIRONMENT(TestEnvironment);

	return RUN_ALL_TEST();
}
