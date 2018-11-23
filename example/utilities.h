#ifndef __EXAMPLE__TEST__UTILITIES__H__
#define __EXAMPLE__TEST__UTILITIES__H__


#include "test.h"


class Utilities
{
public:
	static void Test()
	{
		{
			std::cout << "-------------------------------------------------- ip --------------------------------------------------" << std::endl;

			FUNCTION_TEST(tinyToolkit::IP::AsString(16951488))
			FUNCTION_TEST(tinyToolkit::IP::AsString(3232236033))
			FUNCTION_TEST(tinyToolkit::IP::AsNetByte(16951488))
			FUNCTION_TEST(tinyToolkit::IP::AsNetByte(3232236033))
			FUNCTION_TEST(tinyToolkit::IP::AsNetByte("192.168.2.1"))
			FUNCTION_TEST(tinyToolkit::IP::AsNetByte("1.2.168.192"))
			FUNCTION_TEST(tinyToolkit::IP::AsNetByte(std::string("192.168.2.1")))
			FUNCTION_TEST(tinyToolkit::IP::AsNetByte(std::string("1.2.168.192")))
			FUNCTION_TEST(tinyToolkit::IP::AsHostByte(16951488))
			FUNCTION_TEST(tinyToolkit::IP::AsHostByte(3232236033))
			FUNCTION_TEST(tinyToolkit::IP::AsHostByte("192.168.2.1"))
			FUNCTION_TEST(tinyToolkit::IP::AsHostByte("1.2.168.192"))
			FUNCTION_TEST(tinyToolkit::IP::AsHostByte(std::string("192.168.2.1")))
			FUNCTION_TEST(tinyToolkit::IP::AsHostByte(std::string("1.2.168.192")))
		}

		{
			std::cout << "-------------------------------------------------- byte --------------------------------------------------" << std::endl;

			FUNCTION_TEST(tinyToolkit::Bytes::AsHex('K'))
			FUNCTION_TEST(tinyToolkit::Bytes::AsChar(0x0A))
		}

		{
			std::cout << "-------------------------------------------------- math --------------------------------------------------" << std::endl;

			FUNCTION_TEST(tinyToolkit::Math::IsIP("255.255.255.255"))
			FUNCTION_TEST(tinyToolkit::Math::IsIP("255.255.255.256"))
			FUNCTION_TEST(tinyToolkit::Math::IsOdd(0))
			FUNCTION_TEST(tinyToolkit::Math::IsOdd(1))
			FUNCTION_TEST(tinyToolkit::Math::IsEven(0))
			FUNCTION_TEST(tinyToolkit::Math::IsEven(1))
		}

		{
			std::cout << "-------------------------------------------------- time --------------------------------------------------" << std::endl;

			auto utcTm = tinyToolkit::Time::UTCTm();
			auto localTm = tinyToolkit::Time::LocalTm();

			FUNCTION_TEST(tinyToolkit::Time::FromTm(utcTm))
			FUNCTION_TEST(tinyToolkit::Time::FromTm(localTm))
			FUNCTION_TEST(tinyToolkit::Time::FromTimeString("2018-01-01 10:10:10"))
			FUNCTION_TEST(tinyToolkit::Time::TimeZone())
			FUNCTION_TEST(tinyToolkit::Time::Hours())
			FUNCTION_TEST(tinyToolkit::Time::Minutes())
			FUNCTION_TEST(tinyToolkit::Time::Seconds())
			FUNCTION_TEST(tinyToolkit::Time::Milliseconds())
			FUNCTION_TEST(tinyToolkit::Time::Microseconds())
			FUNCTION_TEST(tinyToolkit::Time::Nanoseconds())
			FUNCTION_TEST(tinyToolkit::Time::CurrentUTCTimeString())
			FUNCTION_TEST(tinyToolkit::Time::CurrentLocalTimeString())

			tinyToolkit::Timer timer;

			timer.CommitOnce(1000, [](int x){ std::cout << tinyToolkit::Time::CurrentLocalTimeString() << " timer-1- : " << x << std::endl; }, 1);
			timer.CommitOnce(4000, [](int x){ std::cout << tinyToolkit::Time::CurrentLocalTimeString() << " timer-2- : " << x << std::endl; }, 2);
			timer.CommitCircle(2000, [](int x){ std::cout << tinyToolkit::Time::CurrentLocalTimeString() << " timer-3- : " << x << std::endl; }, 3);

		}

		{
			std::cout << "-------------------------------------------------- random --------------------------------------------------" << std::endl;

			FUNCTION_TEST(tinyToolkit::Random::IntUniformDistribution(0, 65535))
			FUNCTION_TEST(tinyToolkit::Random::FloatUniformDistribution(0.0, 65535.0))
		}

		{
			std::cout << "-------------------------------------------------- string --------------------------------------------------" << std::endl;

			std::string str = "123456789";
			std::string lineStr = std::string("121545") + TINY_TOOLKIT_EOL + "fdsa" + TINY_TOOLKIT_EOL + "23242" + TINY_TOOLKIT_EOL + "sdf8967sdf" + TINY_TOOLKIT_EOL + "es5dsfsd";
			std::string splitStr = "121545-fdsa-23242-sdf8967sdf-es5dsfsd";

			FUNCTION_TEST(tinyToolkit::String::Trim(" 123 456 789 "))
			FUNCTION_TEST(tinyToolkit::String::Lower("ABCDEfgHIjkl"))
			FUNCTION_TEST(tinyToolkit::String::Upper("ABCDEfgHIjkl"))
			FUNCTION_TEST(tinyToolkit::String::Erase("ABCDEfgHIjkl", 'j'))
			FUNCTION_TEST(tinyToolkit::String::Erase("ABCDEfgHIjkl", 'j', 9))
			FUNCTION_TEST(tinyToolkit::String::Erase("ABCDEfgHIjkl", 'j', 10))
			FUNCTION_TEST(tinyToolkit::String::Erase("ABCDEfgHIjkl", "CD"))
			FUNCTION_TEST(tinyToolkit::String::Erase("ABCDEfgHIjkl", "CD", 2))
			FUNCTION_TEST(tinyToolkit::String::Erase("ABCDEfgHIjkl", "CD", 3))
			FUNCTION_TEST(tinyToolkit::String::Erase("ABCDEfgHIjkl", [](char value) -> bool { return value == 'f'; }))
			FUNCTION_TEST(tinyToolkit::String::Erase("ABCDEfgHIjkl", [](char value) -> bool { return value == 'f'; }, 5))
			FUNCTION_TEST(tinyToolkit::String::Erase("ABCDEfgHIjkl", [](char value) -> bool { return value == 'f'; }, 6))
			FUNCTION_TEST(tinyToolkit::String::Replace("1221212212", "12", "21"))
			FUNCTION_TEST(tinyToolkit::String::Replace("1221212212", "12", "21", 3))
			FUNCTION_TEST(tinyToolkit::String::Replace("1221212212", "12", "21", 4))
			FUNCTION_TEST(tinyToolkit::String::ReplaceAll("1221212212", "12", "21"))
			FUNCTION_TEST(tinyToolkit::String::ReplaceAll("1221212212", "12", "21", 3))
			FUNCTION_TEST(tinyToolkit::String::ReplaceAll("1221212212", "12", "21", 4))
			FUNCTION_TEST(tinyToolkit::String::With("123456", '4'))
			FUNCTION_TEST(tinyToolkit::String::With("123456", '4', 3))
			FUNCTION_TEST(tinyToolkit::String::With("123456", '4', 4))
			FUNCTION_TEST(tinyToolkit::String::With("123456", "4"))
			FUNCTION_TEST(tinyToolkit::String::With("123456", "4", 3))
			FUNCTION_TEST(tinyToolkit::String::With("123456", "4", 4))
			FUNCTION_TEST(tinyToolkit::String::With(str, '4'))
			FUNCTION_TEST(tinyToolkit::String::With(str, '4', 3))
			FUNCTION_TEST(tinyToolkit::String::With(str, '4', 4))
			FUNCTION_TEST(tinyToolkit::String::With(str, "4"))
			FUNCTION_TEST(tinyToolkit::String::With(str, "4", 3))
			FUNCTION_TEST(tinyToolkit::String::With(str, "4", 4))
			FUNCTION_TEST(tinyToolkit::String::EndWith("123456", "55"))
			FUNCTION_TEST(tinyToolkit::String::EndWith("123456", "56"))
			FUNCTION_TEST(tinyToolkit::String::StartWith("123456", "13"))
			FUNCTION_TEST(tinyToolkit::String::StartWith("123456", "12"))
			FUNCTION_TEST(tinyToolkit::String::FilterNote("123456//1234565abcd"))
			FUNCTION_TEST(tinyToolkit::String::Format("{} is {}", 1, true))
			FUNCTION_TEST(tinyToolkit::String::Format("{0} is {0}, {1} is {1}, {2} is {2}", 1, 0.01, true))
			FUNCTION_TEST(tinyToolkit::String::Splice("this is ", "new string, value=", 0, ", size=", 0.123, ", bool=", true))
			FUNCTION_TEST(tinyToolkit::String::AsHexString("123456789"))
			FUNCTION_TEST(tinyToolkit::String::AsHexString("123456789", true))
			FUNCTION_TEST(tinyToolkit::String::AsHexString(str))
			FUNCTION_TEST(tinyToolkit::String::AsHexString(str, true))
			FUNCTION_TEST(tinyToolkit::String::AsHexString("123456789", 5, false))
			FUNCTION_TEST(tinyToolkit::String::AsHexString("123456789", 5, true))
			FUNCTION_TEST(tinyToolkit::String::AsHexString(str, 5, false))
			FUNCTION_TEST(tinyToolkit::String::AsHexString(str, 5, true))
			FUNCTION_TEST(tinyToolkit::String::AsLocalString(123456789))
			FUNCTION_TEST(tinyToolkit::String::AsLocalString(123456789.123456789, 5))
			FUNCTION_TEST(tinyToolkit::String::Transform<int32_t>("123456789.123456789"))
			FUNCTION_TEST(tinyToolkit::String::Transform<double>("123456789"))

			FUNCTION_TEST(tinyToolkit::HexString::AsString("313233343536373839"))
			FUNCTION_TEST(tinyToolkit::HexString::AsString("313233343536373839", true))
			FUNCTION_TEST(tinyToolkit::HexString::AsString("313233343536373839", 12, false))
			FUNCTION_TEST(tinyToolkit::HexString::AsString("313233343536373839", 12, true))

			tinyToolkit::String::Print("{} is {}\r\n", 1, true);
			tinyToolkit::String::Print("{0} is {0}, {1} is {1}, {2} is {2}\r\n", 1, 0.01, true);

			tinyToolkit::String::Print(stderr, "{} is {}\r\n", 1, true);
			tinyToolkit::String::Print(stderr, "{0} is {0}, {1} is {1}, {2} is {2}\r\n", 1, 0.01, true);

			std::cout << "v1:" << std::endl;

			for (auto &iter : tinyToolkit::String::Split(splitStr, "-"))
			{
				std::cout << iter << std::endl;
			}

			std::cout << "v2:" << std::endl;

			for (auto &iter : tinyToolkit::String::SplitLines(lineStr))
			{
				std::cout << iter << std::endl;
			}
		}

		{
			std::cout << "-------------------------------------------------- singleton --------------------------------------------------" << std::endl;

			class A
			{
			public:
				A(int a, int b, const std::string & c)
				{
					std::cout << "a=" << a << ", b=" << b << ", c=" << c << std::endl;
				}

				~A()
				{
					std::cout << "A is free" << std::endl;
				}
			};

			auto & t1 = tinyToolkit::Singleton<A>::Instance(1, 2, "t1");
			auto & t2 = tinyToolkit::Singleton<A>::Instance(1, 2, "t2");
			auto & t3 = tinyToolkit::Singleton<A>::Instance(1, 2, "t3");

			std::cout << std::addressof(t1) << std::endl;
			std::cout << std::addressof(t2) << std::endl;
			std::cout << std::addressof(t3) << std::endl;
		}

		{
			std::cout << "-------------------------------------------------- filesystem --------------------------------------------------" << std::endl;

			StringVector strVector{ "1", "2", "3", "", "", "6", "", "8" };

			tinyToolkit::Filesystem::CreateFile("2.txt");

			if (!tinyToolkit::Filesystem::WriteFile("1.txt", strVector.begin(), strVector.end()))
			{
				std::cout << "write failed" << std::endl;
			}

			auto value = tinyToolkit::Filesystem::ReadAll("1.txt");

			if (!value.empty())
			{
				std::cout << "tinyToolkit::Filesystem::ReadAll(\"1.txt\") : " << value << std::endl;
			}

			std::cout << "v1:" << std::endl;

			for (auto &iter : tinyToolkit::Filesystem::ReadFile("1.txt"))
			{
				std::cout << iter << std::endl;
			}

			std::cout << "v2:" << std::endl;

			for (auto &iter : tinyToolkit::Filesystem::ReadFile("1.txt", true))
			{
				std::cout << iter << std::endl;
			}

#if TINY_TOOLKIT_CXX_SUPPORT >= 17

			std::cout << "trace file" << std::endl;

			for (auto &iter : tinyToolkit::Filesystem::TraverseFile("../"))
			{
				std::cout << iter.string() << std::endl;
			}

			std::cout << "trace file rule" << std::endl;

			for (auto &iter : tinyToolkit::Filesystem::TraverseFile("../", std::regex(".*.txt")))
			{
				std::cout << iter.string() << std::endl;
			}

			std::cout << "trace directory" << std::endl;

			for (auto &iter : tinyToolkit::Filesystem::TraverseDirectory("../"))
			{
				std::cout << iter.string() << std::endl;
			}

			std::cout << "trace directory rule" << std::endl;

			for (auto &iter : tinyToolkit::Filesystem::TraverseDirectory("../", std::regex(".*Debug.*")))
			{
				std::cout << iter.string() << std::endl;
			}

#endif
		}
	}
};



#endif // __EXAMPLE__TEST__UTILITIES__H__
