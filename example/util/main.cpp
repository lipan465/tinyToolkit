/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Math()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		for (int32_t i = 0; i < 3; ++i)
		{
			auto value = util::Math::IntegerUniformDistributionRandom(0, 65535);

			std::cout << value;

			if (util::Math::IsOdd(value))
			{
				std::cout << " IsOdd";
			}
			else if (util::Math::IsEven(value))
			{
				std::cout << " IsEven";
			}

			std::cout << std::endl;
		}

		for (int32_t i = 0; i < 3; ++i)
		{
			std::cout << util::Math::FloatUniformDistributionRandom(0.0, 65535.0) << std::endl;
		}
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Time()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::time_t time1 = 1234509876;

		struct timeval time2{ };
		struct timespec time3{ };

		time2.tv_sec = 1234567890;
		time2.tv_usec = 0;

		time3.tv_sec = 2345678901;
		time3.tv_nsec = 0;

		auto timePoint1 = util::Time::TimePoint();
		auto timePoint2 = util::Time::TimePoint(time1);
		auto timePoint3 = util::Time::TimePoint(time2);
		auto timePoint4 = util::Time::TimePoint(time3);

		auto timeDuration1 = util::Time::TimeDuration();
		auto timeDuration2 = util::Time::TimeDuration(time1);
		auto timeDuration3 = util::Time::TimeDuration(time2);
		auto timeDuration4 = util::Time::TimeDuration(time3);

		std::cout << util::Time::Timezone() << std::endl;
		std::cout << util::Time::Hours() << std::endl;
		std::cout << util::Time::Minutes() << std::endl;
		std::cout << util::Time::Seconds() << std::endl;
		std::cout << util::Time::Milliseconds() << std::endl;
		std::cout << util::Time::Microseconds() << std::endl;
		std::cout << util::Time::Nanoseconds() << std::endl;
		std::cout << util::Time::DayTime(10, 1, 50, 30) << std::endl;
		std::cout << util::Time::DayTime(-2, 1, 50, 30) << std::endl;
		std::cout << util::Time::Format(10086) << std::endl;
		std::cout << util::Time::Format(10086, 3) << std::endl;
		std::cout << util::Time::Format(10000, "%4d_%02d_%02d_%02d_%02d_%02d") << std::endl;
		std::cout << util::Time::Format(10000, 5, "%4d_%02d_%02d_%02d_%02d_%02d") << std::endl;
		std::cout << util::Time::UTCTimeString() << std::endl;
		std::cout << util::Time::UTCTimeString("%4d_%02d_%02d_%02d_%02d_%02d") << std::endl;
		std::cout << util::Time::LocalTimeString() << std::endl;
		std::cout << util::Time::LocalTimeString("%4d_%02d_%02d_%02d_%02d_%02d") << std::endl;
		std::cout << util::Time::Seconds(timePoint1) << std::endl;
		std::cout << util::Time::Seconds(timePoint2) << std::endl;
		std::cout << util::Time::Seconds(timePoint3) << std::endl;
		std::cout << util::Time::Seconds(timePoint4) << std::endl;
		std::cout << util::Time::Seconds(timeDuration1) << std::endl;
		std::cout << util::Time::Seconds(timeDuration2) << std::endl;
		std::cout << util::Time::Seconds(timeDuration3) << std::endl;
		std::cout << util::Time::Seconds(timeDuration4) << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Signal()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		util::Signal::RegisterIgnore();

		util::Signal::RegisterTerminate([](int signalNo){ std::cout << "Terminate : " << signalNo << std::endl; });
		util::Signal::RegisterException([](int signalNo){ std::cout << "Exception : " << signalNo << std::endl; });

	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		util::Signal::Raise(SIGILL);
		util::Signal::Raise(SIGFPE);
		util::Signal::Raise(SIGABRT);
		util::Signal::Raise(SIGSEGV);

		util::Signal::Raise(SIGINT);
		util::Signal::Raise(SIGTERM);
		util::Signal::Raise(SIGBREAK);

	#else

		util::Signal::Raise(SIGHUP);
		util::Signal::Raise(SIGPIPE);
		util::Signal::Raise(SIGCHLD);

		util::Signal::Raise(SIGILL);
		util::Signal::Raise(SIGFPE);
		util::Signal::Raise(SIGABRT);
		util::Signal::Raise(SIGSEGV);

		util::Signal::Raise(SIGINT);
		util::Signal::Raise(SIGQUIT);
		util::Signal::Raise(SIGTERM);
		util::Signal::Raise(SIGTSTP);

	#endif
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void String()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::string line = " 123\n456\n---\nabc\n%%%\n22112121212324343 ";
		std::string content = " 123 456 --- abc %%% 22112121212324343 ";
		std::stringstream stream{ };

		std::vector<std::string> split{ };
		std::vector<std::string> splitLine{ };

		util::String::Print("{} is print\n", "string");
		util::String::Print(stdout, "{} is print\n", "string");
		util::String::Print(std::cout, "{} is print\n", "string");

		util::String::JoinToStream(stream, "123", 456, "abc", 0.123);

		std::cout << stream.str() << std::endl;

		util::String::Split(content, " ", split);
		util::String::SplitLines(line, splitLine);

		for (auto &iter : split)
		{
			std::cout << iter << std::endl;
		}

		for (auto &iter : splitLine)
		{
			std::cout << iter << std::endl;
		}

		util::String::Trim(content);
		util::String::Lower(content);
		util::String::Upper(content);
		util::String::Erase(content, 'a');
		util::String::Erase(content, "123");
		util::String::Replace(content, "456", "ttt");
		util::String::ReplaceLoop(content, "12", "21");

		std::cout << content << std::endl;
		std::cout << util::String::IsIP("192.168.2.1") << std::endl;
		std::cout << util::String::IsIP(content) << std::endl;
		std::cout << util::String::With("content", 'c') << std::endl;
		std::cout << util::String::With("content", "ttt") << std::endl;
		std::cout << util::String::With("content", content) << std::endl;
		std::cout << util::String::With(content, 'c') << std::endl;
		std::cout << util::String::With(content, "ttt") << std::endl;
		std::cout << util::String::With(content, content) << std::endl;
		std::cout << util::String::EndWith(content, "43") << std::endl;
		std::cout << util::String::StartWith(content, "ttt") << std::endl;
		std::cout << util::String::Join("123", 456, "789", 0.33) << std::endl;
		std::cout << util::String::Format("{}", 456) << std::endl;
		std::cout << util::String::FilterNote("123//66666") << std::endl;
		std::cout << util::String::ReduceJson("{\n\"key\":\"value\"\n}") << std::endl;
		std::cout << util::String::BeautifyJson("{\"key\":\"value\"}") << std::endl;
		std::cout << util::String::AsHexString("123") << std::endl;
		std::cout << util::String::AsHexString(reinterpret_cast<const uint8_t *>("313233")) << std::endl;
		std::cout << util::String::AsHexString(content) << std::endl;
		std::cout << util::String::FromHexString("313233") << std::endl;
		std::cout << util::String::FromHexString(reinterpret_cast<const uint8_t *>("313233")) << std::endl;
		std::cout << util::String::FromHexString(std::string("313233")) << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void System()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::cout << "ProcessorCount : " << util::System::ProcessorCount() << std::endl;
		std::cout << "UserName       : " << util::System::UserName() << std::endl;
		std::cout << "ComputerName   : " << util::System::ComputerName() << std::endl;
		std::cout << "IsBigEndian    : " << util::System::IsBigEndian() << std::endl;
		std::cout << "IsLittleEndian : " << util::System::IsLittleEndian() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Traits()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{

	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Singleton()
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
			explicit A(const std::string & name) : _name(name)
			{

			}

		private:
			std::string _name{ };
		};

		std::cout << std::addressof(util::Singleton<A>::Instance("A")) << std::endl;
		std::cout << std::addressof(util::Singleton<A>::Instance("B")) << std::endl;
		std::cout << std::addressof(util::Singleton<A>::Instance("C")) << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Filesystem()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::vector<std::string> read{ };
		std::vector<std::string> dirs{ };
		std::vector<std::string> files{ };
		std::vector<std::string> dirRegexs{ };
		std::vector<std::string> fileRegexs{ };

		std::cout << util::Filesystem::CreateFile("123.txt") << std::endl;
		std::cout << util::Filesystem::CreateFile("456.txt") << std::endl;
		std::cout << util::Filesystem::CreateDirectory("a") << std::endl;
		std::cout << util::Filesystem::CreateDirectories("a/b/c/d") << std::endl;
		std::cout << util::Filesystem::WriteFile("123.txt", "hello") << std::endl;
		std::cout << util::Filesystem::Exists("123.txt") << std::endl;
		std::cout << util::Filesystem::IsDirectory("a") << std::endl;
		std::cout << util::Filesystem::Content("123.txt") << std::endl;
		std::cout << util::Filesystem::Remove("123.txt") << std::endl;
		std::cout << util::Filesystem::Rename("456.txt", "123.txt") << std::endl;
		std::cout << util::Filesystem::ReadFile("123.txt", read) << std::endl;
		std::cout << util::Filesystem::Size("123.txt") << std::endl;
		std::cout << util::Filesystem::CurrentDirectory() << std::endl;
		std::cout << util::Filesystem::ParentDirectory("123.txt") << std::endl;
		std::cout << util::Filesystem::Name("123.txt") << std::endl;
		std::cout << util::Filesystem::Steam("123.txt") << std::endl;
		std::cout << util::Filesystem::Extension("123.txt") << std::endl;
		std::cout << util::Filesystem::Canonical("123.txt") << std::endl;
		std::cout << util::Filesystem::TraverseFile(".", files) << std::endl;
		std::cout << util::Filesystem::TraverseFile(".", fileRegexs, std::regex(".*.txt")) << std::endl;
		std::cout << util::Filesystem::TraverseDirectory(".", dirs, true) << std::endl;
		std::cout << util::Filesystem::TraverseDirectory(".", dirRegexs, std::regex(".*[a|b|c]+"), true) << std::endl;

		for (auto &iter : read)
		{
			std::cout << iter << std::endl;
		}

		std::cout << "--------" << std::endl;

		for (auto &iter : files)
		{
			std::cout << iter << std::endl;
		}

		std::cout << "--------" << std::endl;

		for (auto &iter : fileRegexs)
		{
			std::cout << iter << std::endl;
		}

		std::cout << "--------" << std::endl;

		for (auto &iter : dirs)
		{
			std::cout << iter << std::endl;
		}

		std::cout << "--------" << std::endl;

		for (auto &iter : dirRegexs)
		{
			std::cout << iter << std::endl;
		}
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
		std::cout << "Path              : " << util::Application::Path() << std::endl;
		std::cout << "Name              : " << util::Application::Name() << std::endl;
		std::cout << "Exist             : " << util::Application::Exist() << std::endl;
		std::cout << "Steam             : " << util::Application::Steam() << std::endl;
		std::cout << "ThreadID          : " << util::Application::ThreadID() << std::endl;
		std::cout << "ProcessID         : " << util::Application::ProcessID() << std::endl;
		std::cout << "Extension         : " << util::Application::Extension() << std::endl;
		std::cout << "CompileTime       : " << util::Application::CompileTime() << std::endl;
		std::cout << "ParentDirectory   : " << util::Application::ParentDirectory() << std::endl;
		std::cout << "CompileTimeString : " << util::Application::CompileTimeString() << std::endl;
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

	Math();
	Time();
	Signal();
	String();
	System();
	Traits();
	Singleton();
	Filesystem();
	Application();

	return 0;
}
