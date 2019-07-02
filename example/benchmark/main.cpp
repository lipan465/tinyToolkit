/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


TEST(ID, Random)
{
	try
	{
		EXPECT_LE(tinyToolkit::Random::IntUniformDistribution(0, 65535), 65535);
		EXPECT_LE(tinyToolkit::Random::FloatUniformDistribution(0.0, 65535.0), 65535.0);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(ID, UniqueID)
{
	try
	{
		EXPECT_LE(tinyToolkit::UniqueID::Get(), tinyToolkit::UniqueID::Get() + 1);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(ID, Snowflake)
{
	try
	{
		EXPECT_LE(tinyToolkit::Snowflake::Get(0, 0), tinyToolkit::Snowflake::Get(0, 0) + 1);
		EXPECT_LE(tinyToolkit::Snowflake::Get(0, 1), tinyToolkit::Snowflake::Get(0, 1) + 1);
		EXPECT_LE(tinyToolkit::Snowflake::Get(1, 0), tinyToolkit::Snowflake::Get(1, 0) + 1);
		EXPECT_LE(tinyToolkit::Snowflake::Get(1, 1), tinyToolkit::Snowflake::Get(1, 1) + 1);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void SyncLogger(const std::shared_ptr<tinyToolkit::ILogSink> & sink, std::size_t count)
{
	std::shared_ptr<tinyToolkit::ILogger> logger = std::make_shared<tinyToolkit::AsyncLogger>();

	logger->AddSink(sink)
			->SetLayout(std::make_shared<tinyToolkit::SimpleLogLayout>())
			->SetFilter(std::make_shared<tinyToolkit::PriorityRangeLogFilter>(tinyToolkit::LOG_PRIORITY_TYPE::INFO, tinyToolkit::LOG_PRIORITY_TYPE::FATAL));

	for (std::size_t i = 0; i < count; ++i)
	{
		logger->Info("this is {}", "Info");
		logger->Debug("this is {}", "Debug");
		logger->Fatal("this is {}", "Fatal");
		logger->Emerg("this is {}", "Emerg");

		TINY_TOOLKIT_LOG_INFO(logger, "this is {}", "Info");
		TINY_TOOLKIT_LOG_DEBUG(logger, "this is {}", "Debug");
		TINY_TOOLKIT_LOG_FATAL(logger, "this is {}", "Fatal");
		TINY_TOOLKIT_LOG_EMERG(logger, "this is {}", "Emerg");
	}

	logger->Wait();

	sink->Flush();
}


TEST(LogSync, FileSink)
{
	try
	{
		std::size_t count = 100;

		std::string fileName = tinyToolkit::String::Format("{0}{1}log{1}syncFile.log", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);
		std::string realFileName = fileName;

		auto sink = std::make_shared<tinyToolkit::FileLogSink>("syncFile", fileName, true);

		SyncLogger(sink, count);

		EXPECT_EQ(tinyToolkit::Filesystem::Size(realFileName), ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(LogSync, OStreamSink)
{
	try
	{
		std::size_t count = 100;

		auto stream = std::make_shared<std::stringstream>();

		auto sink = std::make_shared<tinyToolkit::OStreamLogSink>("syncOStream", stream);

		SyncLogger(sink, count);

		EXPECT_EQ(stream->str().size(), ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


#if TINY_TOOLKIT_PLATFORM != TINY_TOOLKIT_PLATFORM_WINDOWS

TEST(LogSync, Syslog)
{
	try
	{
		std::size_t count = 100;

		auto sink = std::make_shared<tinyToolkit::SyslogLogSink>("syncSyslog");

		SyncLogger(sink, count);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}

#endif


TEST(LogSync, DailyFileSink)
{
	try
	{
		std::size_t count = 100;

		std::string fileName = tinyToolkit::String::Format("{0}{1}log{1}syncDailyFile.log", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);
		std::string realFileName = tinyToolkit::String::Format
		(
			"{}_{}{}",
			fileName.substr(0, fileName.rfind('.')),
			tinyToolkit::Time::CurrentLocalTimeString("%04d_%02d_%02d_%02d_%02d_%02d"),
			fileName.substr(fileName.rfind('.'))
		);

		auto sink = std::make_shared<tinyToolkit::DailyFileLogSink>("syncDailyFile", fileName);

		SyncLogger(sink, count);

		EXPECT_EQ(tinyToolkit::Filesystem::Size(realFileName), ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(LogSync, StringQueueSink)
{
	try
	{
		std::size_t count = 100;

		auto sink = std::make_shared<tinyToolkit::StringQueueLogSink>("syncStringQueue");

		SyncLogger(sink, count);

		EXPECT_EQ(sink->Size(), 4 * count);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(LogSync, RotatingFileSink)
{
	try
	{
		std::size_t count = 100;

		std::string fileName = tinyToolkit::String::Format("{0}{1}log{1}syncRotatingFile.log", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);
		std::string realFileName = fileName;

		auto sink = std::make_shared<tinyToolkit::RotatingFileLogSink>("syncRotatingFile", fileName, TINY_TOOLKIT_MB);

		SyncLogger(sink, count);

		EXPECT_LE(tinyToolkit::Filesystem::Size(realFileName), static_cast<std::size_t>(TINY_TOOLKIT_MB));
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void AsyncLogger(const std::shared_ptr<tinyToolkit::ILogSink> & sink, std::size_t count, std::size_t threadCount)
{
	std::shared_ptr<tinyToolkit::ILogger> logger = std::make_shared<tinyToolkit::AsyncLogger>();

	logger->AddSink(sink)
			->SetLayout(std::make_shared<tinyToolkit::SimpleLogLayout>())
			->SetFilter(std::make_shared<tinyToolkit::PriorityRangeLogFilter>(tinyToolkit::LOG_PRIORITY_TYPE::INFO, tinyToolkit::LOG_PRIORITY_TYPE::FATAL));

	std::atomic<std::size_t> counter{ 0 };

	tinyToolkit::ThreadPool threadPool(threadCount);

	threadPool.Commit
	(
		[&]()
		{
			while (++counter <= count)
			{
				logger->Info("this is {}", "Info");
				logger->Debug("this is {}", "Debug");
				logger->Fatal("this is {}", "Fatal");
				logger->Emerg("this is {}", "Emerg");

				TINY_TOOLKIT_LOG_INFO(logger, "this is {}", "Info");
				TINY_TOOLKIT_LOG_DEBUG(logger, "this is {}", "Debug");
				TINY_TOOLKIT_LOG_FATAL(logger, "this is {}", "Fatal");
				TINY_TOOLKIT_LOG_EMERG(logger, "this is {}", "Emerg");
			}
		}
	);

	threadPool.Wait();

	logger->Wait();

	sink->Flush();
}


TEST(LogAsync, FileSink)
{
	try
	{
		std::size_t count = 100;

		std::string fileName = tinyToolkit::String::Format("{0}{1}log{1}asyncFile.log", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);
		std::string realFileName = fileName;

		auto sink = std::make_shared<tinyToolkit::FileLogSink>("asyncFile", fileName, true);

		AsyncLogger(sink, count, std::thread::hardware_concurrency());

		EXPECT_EQ(tinyToolkit::Filesystem::Size(realFileName), ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


#if TINY_TOOLKIT_PLATFORM != TINY_TOOLKIT_PLATFORM_WINDOWS

TEST(LogAsync, Syslog)
{
	try
	{
		std::size_t count = 100;

		auto sink = std::make_shared<tinyToolkit::SyslogLogSink>("syncSyslog");

		AsyncLogger(sink, count, std::thread::hardware_concurrency());
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}

#endif


TEST(LogAsync, OStreamSink)
{
	try
	{
		std::size_t count = 100;

		auto stream = std::make_shared<std::stringstream>();

		auto sink = std::make_shared<tinyToolkit::OStreamLogSink>("asyncOStream", stream);

		AsyncLogger(sink, count, std::thread::hardware_concurrency());

		EXPECT_EQ(stream->str().size(), ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(LogAsync, DailyFileSink)
{
	try
	{
		std::size_t count = 100;

		std::string fileName = tinyToolkit::String::Format("{0}{1}log{1}asyncDailyFile.log", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);
		std::string realFileName = tinyToolkit::String::Format
		(
			"{}_{}{}",
			fileName.substr(0, fileName.rfind('.')),
			tinyToolkit::Time::CurrentLocalTimeString("%04d_%02d_%02d_%02d_%02d_%02d"),
			fileName.substr(fileName.rfind('.'))
		);

		auto sink = std::make_shared<tinyToolkit::DailyFileLogSink>("asyncDailyFile", fileName);

		AsyncLogger(sink, count, std::thread::hardware_concurrency());

		EXPECT_EQ(tinyToolkit::Filesystem::Size(realFileName), ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(LogAsync, StringQueueSink)
{
	try
	{
		std::size_t count = 100;

		auto sink = std::make_shared<tinyToolkit::StringQueueLogSink>("asyncStringQueue");

		AsyncLogger(sink, count, std::thread::hardware_concurrency());

		EXPECT_EQ(sink->Size(), 4 * count);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(LogAsync, RotatingFileSink)
{
	try
	{
		std::size_t count = 100;

		std::string fileName = tinyToolkit::String::Format("{0}{1}log{1}asyncRotatingFile.log", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);
		std::string realFileName = fileName;

		auto sink = std::make_shared<tinyToolkit::RotatingFileLogSink>("asyncRotatingFile", fileName, TINY_TOOLKIT_MB);

		AsyncLogger(sink, count, std::thread::hardware_concurrency());

		EXPECT_LE(tinyToolkit::Filesystem::Size(realFileName), static_cast<std::size_t>(TINY_TOOLKIT_MB));
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


class TimerEventTest : public tinyToolkit::ITimerEvent
{
public:
	~TimerEventTest() override = default;

	void OnPause() override
	{
		_status = 1;
	}

	void OnResume() override
	{
		_status = 2;
	}

	void OnTrigger() override
	{
		_status = 0;
	}

	void OnFinish(bool forced) override
	{
		_status = -1;
		_forced = forced;
	}

public:
	bool _forced{ false };

	int32_t _status{ -1 };
};


TEST(Timer, EventNone)
{
	try
	{
		tinyToolkit::TimerManager manager;

		auto event = new TimerEventTest;

		manager.Start(event, 0, 10);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		manager.Pause(event);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		manager.Resume(event);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		manager.Kill(event);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		delete event;
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Timer, EventCricle)
{
	try
	{
		tinyToolkit::TimerManager manager;

		auto event = new TimerEventTest;

		manager.Start(event, -1, 10);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, 0);

		EXPECT_FALSE(event->_forced);

		manager.Pause(event);

		EXPECT_EQ(event->_status, 1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, 1);

		EXPECT_FALSE(event->_forced);

		manager.Resume(event);

		EXPECT_EQ(event->_status, 2);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, 0);

		EXPECT_FALSE(event->_forced);

		manager.Kill(event);

		EXPECT_EQ(event->_status, -1);

		EXPECT_TRUE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, -1);

		EXPECT_TRUE(event->_forced);

		delete event;
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Timer, EventLessCount)
{
	try
	{
		tinyToolkit::TimerManager manager;

		auto event = new TimerEventTest;

		manager.Start(event, 2, 10);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		manager.Pause(event);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		manager.Resume(event);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		manager.Kill(event);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		delete event;
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Timer, EventGreaterCount)
{
	try
	{
		tinyToolkit::TimerManager manager;

		auto event = new TimerEventTest;

		manager.Start(event, 15, 10);

		EXPECT_EQ(event->_status, -1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, 0);

		EXPECT_FALSE(event->_forced);

		manager.Pause(event);

		EXPECT_EQ(event->_status, 1);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, 1);

		EXPECT_FALSE(event->_forced);

		manager.Resume(event);

		EXPECT_EQ(event->_status, 2);

		EXPECT_FALSE(event->_forced);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, 0);

		EXPECT_FALSE(event->_forced);

		manager.Kill(event);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(event->_status, -1);

		EXPECT_TRUE(event->_forced);

		delete event;
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Timer, FunctionNone)
{
	try
	{
		int32_t count = 0;

		tinyToolkit::TimerManager manager;

		manager.Start([&](){ ++count; }, 0, 10);

		EXPECT_EQ(count, 0);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(count, 0);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Timer, FunctionCricle)
{
	try
	{
		int32_t count = 0;

		tinyToolkit::TimerManager manager;

		manager.Start([&](){ ++count; }, -1, 10);

		EXPECT_EQ(count, 0);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_GE(count, 2);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Timer, FunctionLessCount)
{
	try
	{
		int32_t count = 0;

		tinyToolkit::TimerManager manager;

		manager.Start([&](){ ++count; }, 2, 10);

		EXPECT_EQ(count, 0);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_EQ(count, 2);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}

TEST(Timer, FunctionGreaterCount)
{
	try
	{
		int32_t count = 0;

		tinyToolkit::TimerManager manager;

		manager.Start([&](){ ++count; }, 15, 10);

		EXPECT_EQ(count, 0);

		TINY_TOOLKIT_SLEEP_MS(30);

		EXPECT_GE(count, 2);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


class ThreadPoolTest
{
public:
	static void plus(std::atomic<int32_t> & result, int32_t value)
	{
		result += value;
		result -= 1;
	}
};


TEST(Pool, Thread)
{
	try
	{
		std::atomic<int32_t> result;

		result.store(0);

		tinyToolkit::ThreadPool pool(8);

		for (int32_t i = 0; i < 10; ++i)
		{
			pool.Commit(&ThreadPoolTest::plus, std::ref(result), i + 1);
		}

		pool.Wait();

		EXPECT_EQ(pool.TaskSize(), static_cast<std::size_t>(0));
		EXPECT_EQ(pool.ThreadSize(), static_cast<std::size_t>(8));

		EXPECT_FALSE(pool.IsClose());

		pool.Release();

		EXPECT_EQ(result.load(), 45);
		EXPECT_EQ(pool.TaskSize(), static_cast<std::size_t>(0));
		EXPECT_EQ(pool.ThreadSize(), static_cast<std::size_t>(0));

		EXPECT_TRUE(pool.IsClose());
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


class CallBackPoolTest
{
public:
	void Callback(int x, int y)
	{
		_result -= x;
		_result += y;
	}

	int64_t Result() const
	{
		return _result;
	}

protected:
	int64_t _result{ 0 };
};


TEST(Pool, CallBack)
{
	try
	{
		int64_t result = 0;

		CallBackPoolTest testClass;

		std::map<std::string, std::size_t> container;

		tinyToolkit::CallBackPool<void, int, int> pool;

		container.insert(std::make_pair("test1", pool.Register([&](int x, int y){ result += x; result -= y; })));
		container.insert(std::make_pair("test2", pool.Register(std::bind(&CallBackPoolTest::Callback, &testClass, std::placeholders::_1, std::placeholders::_2))));

		pool.Call(11, 22);

		EXPECT_EQ(result, -11);
		EXPECT_EQ(pool.Size(), static_cast<std::size_t>(2));
		EXPECT_EQ(testClass.Result(), static_cast<int64_t>(11));

		pool.Call(22, 11);

		EXPECT_EQ(result, 0);
		EXPECT_EQ(pool.Size(), static_cast<std::size_t>(2));
		EXPECT_EQ(testClass.Result(), 0);

		pool.UnRegister(container["test1"]);

		pool(10, 50);

		EXPECT_EQ(result, 0);
		EXPECT_EQ(pool.Size(), static_cast<std::size_t>(1));
		EXPECT_EQ(testClass.Result(), 40);

		pool.Call(20, 10);

		EXPECT_EQ(result, 0);
		EXPECT_EQ(pool.Size(), static_cast<std::size_t>(1));
		EXPECT_EQ(testClass.Result(), 30);

		pool -= container["test2"];

		pool(10, 50);

		EXPECT_EQ(result, 0);
		EXPECT_EQ(pool.Size(), static_cast<std::size_t>(0));
		EXPECT_EQ(testClass.Result(), 30);

		pool.Call(20, 10);

		EXPECT_EQ(result, 0);
		EXPECT_EQ(pool.Size(), static_cast<std::size_t>(0));
		EXPECT_EQ(testClass.Result(), 30);

		pool += [&](int x, int y){ result += x; result -= y; };

		pool(80, 50);

		EXPECT_EQ(result, 30);
		EXPECT_EQ(pool.Size(), static_cast<std::size_t>(1));
		EXPECT_EQ(testClass.Result(), 30);

		pool.Call(20, 40);

		EXPECT_EQ(result, 10);
		EXPECT_EQ(pool.Size(), static_cast<std::size_t>(1));
		EXPECT_EQ(testClass.Result(), 30);

		pool.UnRegister();

		pool(80, 50);

		EXPECT_EQ(result, 10);
		EXPECT_EQ(pool.Size(), static_cast<std::size_t>(0));
		EXPECT_EQ(testClass.Result(), 30);

		pool.Call(20, 40);

		EXPECT_EQ(result, 10);
		EXPECT_EQ(pool.Size(), static_cast<std::size_t>(0));
		EXPECT_EQ(testClass.Result(), 30);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


class ApplicationPoolInt
{
public:
	explicit ApplicationPoolInt(int32_t x) : _x(x)
	{

	}

protected:
	int32_t _x{ 0 };
};


class ApplicationPoolDouble
{
public:
	explicit ApplicationPoolDouble(double x) : _x(x)
	{

	}

protected:
	double _x{ 0.0 };
};


TEST(Pool, Application)
{
	try
	{
		{
			std::vector<ApplicationPoolInt *> container;

			tinyToolkit::ApplicationPool<ApplicationPoolInt, 4> pool;

			for (std::size_t i = 0; i <= 4; ++i)
			{
				container.push_back(pool.Create(i));
			}

			EXPECT_EQ(pool.UsedSize(), static_cast<std::size_t>(4 + 1));
			EXPECT_EQ(pool.ChunkSize(), static_cast<std::size_t>(4 * 2));
			EXPECT_EQ(pool.ChunkListSize(), static_cast<std::size_t>(2));

			pool.Recover(container.back());

			container.pop_back();

			EXPECT_EQ(pool.UsedSize(), static_cast<std::size_t>(4));
			EXPECT_EQ(pool.ChunkSize(), static_cast<std::size_t>(4));
			EXPECT_EQ(pool.ChunkListSize(), static_cast<std::size_t>(1));

			for (auto &iter : container)
			{
				pool.Recover(iter);
			}

			EXPECT_EQ(pool.UsedSize(), static_cast<std::size_t>(0));
			EXPECT_EQ(pool.ChunkSize(), static_cast<std::size_t>(4));
			EXPECT_EQ(pool.ChunkListSize(), static_cast<std::size_t>(1));
		}

		{
			std::vector<ApplicationPoolDouble *> container;

			tinyToolkit::ApplicationPool<ApplicationPoolDouble, 4> pool;

			for (std::size_t i = 0; i < 4; ++i)
			{
				container.push_back(pool.Create(i / 1.0));
			}

			EXPECT_EQ(pool.UsedSize(), static_cast<std::size_t>(4));
			EXPECT_EQ(pool.ChunkSize(), static_cast<std::size_t>(4));
			EXPECT_EQ(pool.ChunkListSize(), static_cast<std::size_t>(1));

			pool.Recover(container.back());

			container.pop_back();

			EXPECT_EQ(pool.UsedSize(), static_cast<std::size_t>(3));
			EXPECT_EQ(pool.ChunkSize(), static_cast<std::size_t>(4));
			EXPECT_EQ(pool.ChunkListSize(), static_cast<std::size_t>(1));

			for (auto &iter : container)
			{
				pool.Recover(iter);
			}

			EXPECT_EQ(pool.UsedSize(), static_cast<std::size_t>(0));
			EXPECT_EQ(pool.ChunkSize(), static_cast<std::size_t>(4));
			EXPECT_EQ(pool.ChunkListSize(), static_cast<std::size_t>(1));
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Crypto, MD5)
{
	try
	{
		{
			tinyToolkit::MD5 md5("123");

			EXPECT_STR_EQ(md5.Hex().c_str(), "202cb962ac59075b964b07152d234b70");

			md5.Update("abc");

			EXPECT_STR_EQ(md5.Hex().c_str(), "a906449d5769fa7361d7ecc6aa3f6d28");

			md5.Reset();
			md5.Update("xyz");

			EXPECT_STR_EQ(md5.Hex().c_str(), "d16fb36f0911f878998c136191af705e");

			md5.Reset();
			md5.Update("4567890");

			EXPECT_STR_EQ(md5.Hex().c_str(), "a14b1bbaad4c127657d9c8d907fc6a75");
		}

		{
			tinyToolkit::MD5 md5("123456789", 3);

			EXPECT_STR_EQ(md5.Hex().c_str(), "202cb962ac59075b964b07152d234b70");

			md5.Update("abc");

			EXPECT_STR_EQ(md5.Hex().c_str(), "a906449d5769fa7361d7ecc6aa3f6d28");

			md5.Reset();
			md5.Update("xyz");

			EXPECT_STR_EQ(md5.Hex().c_str(), "d16fb36f0911f878998c136191af705e");

			md5.Reset();
			md5.Update("4567890");

			EXPECT_STR_EQ(md5.Hex().c_str(), "a14b1bbaad4c127657d9c8d907fc6a75");
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Crypto, SHA1)
{
	try
	{
		{
			tinyToolkit::SHA1 sha1("123");

			EXPECT_STR_EQ(sha1.Hex().c_str(), "40bd001563085fc35165329ea1ff5c5ecbdbbeef");

			sha1.Update("abc");

			EXPECT_STR_EQ(sha1.Hex().c_str(), "4be30d9814c6d4e9800e0d2ea9ec9fb00efa887b");

			sha1.Reset();
			sha1.Update("xyz");

			EXPECT_STR_EQ(sha1.Hex().c_str(), "66b27417d37e024c46526c2f6d358a754fc552f3");

			sha1.Reset();
			sha1.Update("4567890");

			EXPECT_STR_EQ(sha1.Hex().c_str(), "94038465034c7fcea862797087a6a90ee557ff2a");
		}

		{
			tinyToolkit::SHA1 sha1("123456789", 3);

			EXPECT_STR_EQ(sha1.Hex().c_str(), "40bd001563085fc35165329ea1ff5c5ecbdbbeef");

			sha1.Update("abc");

			EXPECT_STR_EQ(sha1.Hex().c_str(), "4be30d9814c6d4e9800e0d2ea9ec9fb00efa887b");

			sha1.Reset();
			sha1.Update("xyz");

			EXPECT_STR_EQ(sha1.Hex().c_str(), "66b27417d37e024c46526c2f6d358a754fc552f3");

			sha1.Reset();
			sha1.Update("4567890");

			EXPECT_STR_EQ(sha1.Hex().c_str(), "94038465034c7fcea862797087a6a90ee557ff2a");
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Crypto, SHA224)
{
	try
	{
		{
			tinyToolkit::SHA224 sha224("123");

			EXPECT_STR_EQ(sha224.Hex().c_str(), "78d8045d684abd2eece923758f3cd781489df3a48e1278982466017f");

			sha224.Update("abc");

			EXPECT_STR_EQ(sha224.Hex().c_str(), "867916c509e025cd1eef93151e82f518fcfdfcacd759965fd99e27af");

			sha224.Reset();
			sha224.Update("xyz");

			EXPECT_STR_EQ(sha224.Hex().c_str(), "30e90f1cd0ceff8eb3dd6a540a605c0666f841d35de63c57e4dd2877");

			sha224.Reset();
			sha224.Update("4567890");

			EXPECT_STR_EQ(sha224.Hex().c_str(), "9d04ca6244472b08d350f9064749c397ecd8d2b716b04e0d5226a3e2");
		}

		{
			tinyToolkit::SHA224 sha224("123456789", 3);

			EXPECT_STR_EQ(sha224.Hex().c_str(), "78d8045d684abd2eece923758f3cd781489df3a48e1278982466017f");

			sha224.Update("abc");

			EXPECT_STR_EQ(sha224.Hex().c_str(), "867916c509e025cd1eef93151e82f518fcfdfcacd759965fd99e27af");

			sha224.Reset();
			sha224.Update("xyz");

			EXPECT_STR_EQ(sha224.Hex().c_str(), "30e90f1cd0ceff8eb3dd6a540a605c0666f841d35de63c57e4dd2877");

			sha224.Reset();
			sha224.Update("4567890");

			EXPECT_STR_EQ(sha224.Hex().c_str(), "9d04ca6244472b08d350f9064749c397ecd8d2b716b04e0d5226a3e2");
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Crypto, SHA256)
{
	try
	{
		{
			tinyToolkit::SHA256 sha256("123");

			EXPECT_STR_EQ(sha256.Hex().c_str(), "a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3");

			sha256.Update("abc");

			EXPECT_STR_EQ(sha256.Hex().c_str(), "dd130a849d7b29e5541b05d2f7f86a4acd4f1ec598c1c9438783f56bc4f0ff80");

			sha256.Reset();
			sha256.Update("xyz");

			EXPECT_STR_EQ(sha256.Hex().c_str(), "3608bca1e44ea6c4d268eb6db02260269892c0b42b86bbf1e77a6fa16c3c9282");

			sha256.Reset();
			sha256.Update("4567890");

			EXPECT_STR_EQ(sha256.Hex().c_str(), "611ff1be579a929782a03a688a198c4eae13b16f2ccf476f52d81b24dddbc780");
		}

		{
			tinyToolkit::SHA256 sha256("123456789", 3);

			EXPECT_STR_EQ(sha256.Hex().c_str(), "a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3");

			sha256.Update("abc");

			EXPECT_STR_EQ(sha256.Hex().c_str(), "dd130a849d7b29e5541b05d2f7f86a4acd4f1ec598c1c9438783f56bc4f0ff80");

			sha256.Reset();
			sha256.Update("xyz");

			EXPECT_STR_EQ(sha256.Hex().c_str(), "3608bca1e44ea6c4d268eb6db02260269892c0b42b86bbf1e77a6fa16c3c9282");

			sha256.Reset();
			sha256.Update("4567890");

			EXPECT_STR_EQ(sha256.Hex().c_str(), "611ff1be579a929782a03a688a198c4eae13b16f2ccf476f52d81b24dddbc780");
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Crypto, SHA384)
{
	try
	{
		{
			tinyToolkit::SHA384 sha384("123");

			EXPECT_STR_EQ(sha384.Hex().c_str(), "9a0a82f0c0cf31470d7affede3406cc9aa8410671520b727044eda15b4c25532a9b5cd8aaf9cec4919d76255b6bfb00f");

			sha384.Update("abc");

			EXPECT_STR_EQ(sha384.Hex().c_str(), "40889bd7aee5be35f9d713c4f51ff00fe2099fd901d094284d6edc9aa39ec096cebe9547d3cbaf1f35101a6489f033c3");

			sha384.Reset();
			sha384.Update("xyz");

			EXPECT_STR_EQ(sha384.Hex().c_str(), "edcb0f4721e6578d900e4c24ad4b19e194ab6c87f8243bfc6b11754dd8b0bbde4f30b1d18197932b6376da004dcd97c4");

			sha384.Reset();
			sha384.Update("4567890");

			EXPECT_STR_EQ(sha384.Hex().c_str(), "a3c5da2c343f870d3d64e90ae17abd7060487caa089d170d941005a72614fe57f398b8a3d4d4e57c90a6f4022529fa47");
		}

		{
			tinyToolkit::SHA384 sha384("123456789", 3);

			EXPECT_STR_EQ(sha384.Hex().c_str(), "9a0a82f0c0cf31470d7affede3406cc9aa8410671520b727044eda15b4c25532a9b5cd8aaf9cec4919d76255b6bfb00f");

			sha384.Update("abc");

			EXPECT_STR_EQ(sha384.Hex().c_str(), "40889bd7aee5be35f9d713c4f51ff00fe2099fd901d094284d6edc9aa39ec096cebe9547d3cbaf1f35101a6489f033c3");

			sha384.Reset();
			sha384.Update("xyz");

			EXPECT_STR_EQ(sha384.Hex().c_str(), "edcb0f4721e6578d900e4c24ad4b19e194ab6c87f8243bfc6b11754dd8b0bbde4f30b1d18197932b6376da004dcd97c4");

			sha384.Reset();
			sha384.Update("4567890");

			EXPECT_STR_EQ(sha384.Hex().c_str(), "a3c5da2c343f870d3d64e90ae17abd7060487caa089d170d941005a72614fe57f398b8a3d4d4e57c90a6f4022529fa47");
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Crypto, SHA512)
{
	try
	{
		{
			tinyToolkit::SHA512 sha512("123");

			EXPECT_STR_EQ(sha512.Hex().c_str(), "3c9909afec25354d551dae21590bb26e38d53f2173b8d3dc3eee4c047e7ab1c1eb8b85103e3be7ba613b31bb5c9c36214dc9f14a42fd7a2fdb84856bca5c44c2");

			sha512.Update("abc");

			EXPECT_STR_EQ(sha512.Hex().c_str(), "7b6ad79b346fb6951275343948e13c1b4ebca82a5452a6c5d15684377f096ca927506a23a847e6e046061399631b16fc2820c8b0e02d0ea87aa5a203a77c2a7e");

			sha512.Reset();
			sha512.Update("xyz");

			EXPECT_STR_EQ(sha512.Hex().c_str(), "4a3ed8147e37876adc8f76328e5abcc1b470e6acfc18efea0135f983604953a58e183c1a6086e91ba3e821d926f5fdeb37761c7ca0328a963f5e92870675b728");

			sha512.Reset();
			sha512.Update("4567890");

			EXPECT_STR_EQ(sha512.Hex().c_str(), "a642cf14e65b0b2b8dd2084e557b0939dcebb63d46cbf8730e3084f536506da4f0457029c0b0d69d598987956796ee29fbf99d3b37bfbf53153bd3d32a180c8a");
		}

		{
			tinyToolkit::SHA512 sha512("123456789", 3);

			EXPECT_STR_EQ(sha512.Hex().c_str(), "3c9909afec25354d551dae21590bb26e38d53f2173b8d3dc3eee4c047e7ab1c1eb8b85103e3be7ba613b31bb5c9c36214dc9f14a42fd7a2fdb84856bca5c44c2");

			sha512.Update("abc");

			EXPECT_STR_EQ(sha512.Hex().c_str(), "7b6ad79b346fb6951275343948e13c1b4ebca82a5452a6c5d15684377f096ca927506a23a847e6e046061399631b16fc2820c8b0e02d0ea87aa5a203a77c2a7e");

			sha512.Reset();
			sha512.Update("xyz");

			EXPECT_STR_EQ(sha512.Hex().c_str(), "4a3ed8147e37876adc8f76328e5abcc1b470e6acfc18efea0135f983604953a58e183c1a6086e91ba3e821d926f5fdeb37761c7ca0328a963f5e92870675b728");

			sha512.Reset();
			sha512.Update("4567890");

			EXPECT_STR_EQ(sha512.Hex().c_str(), "a642cf14e65b0b2b8dd2084e557b0939dcebb63d46cbf8730e3084f536506da4f0457029c0b0d69d598987956796ee29fbf99d3b37bfbf53153bd3d32a180c8a");
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Crypto, Base64)
{
	try
	{
		{
			const char * src = "d16fb36f0911f878998c136191af705e";
			const char * dst = "ZDE2ZmIzNmYwOTExZjg3ODk5OGMxMzYxOTFhZjcwNWU=";

			EXPECT_STR_EQ(tinyToolkit::Base64::Encode(src).c_str(), dst);
			EXPECT_STR_EQ(tinyToolkit::Base64::Decode(dst).c_str(), src);
		}

		{
			const char * src = "d16fb36f0911f878998c136191af705e";
			const char * dst = "ZDE2ZmIzNmYwOTExZjg3ODk5OGMxMzYxOTFhZjcwNWU=";

			EXPECT_STR_EQ(tinyToolkit::Base64::Encode(reinterpret_cast<const uint8_t *>(src)).c_str(), dst);
			EXPECT_STR_EQ(tinyToolkit::Base64::Decode(reinterpret_cast<const uint8_t *>(dst)).c_str(), src);
		}

		{
			const char * src = "d16fb36f0911f878998c136191af705e";
			const char * dst = "ZDE2ZmIzNmYwOTExZjg3ODk5OGMxMzYxOTFhZjcwNWU=";

			EXPECT_STR_EQ(tinyToolkit::Base64::Encode(std::string(src)).c_str(), dst);
			EXPECT_STR_EQ(tinyToolkit::Base64::Decode(std::string(dst)).c_str(), src);
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Crypto, UrlTransform)
{
	try
	{
		{
			const char * src = "b+-f*0^1$8%7';2^#$)(_c)&$#!!^9";
			const char * dst = "b%2B-f*0%5E1%248%257%27%3B2%5E%23%24)(_c)%26%24%23!!%5E9";

			EXPECT_STR_EQ(tinyToolkit::UrlTransform::Encode(src).c_str(), dst);
			EXPECT_STR_EQ(tinyToolkit::UrlTransform::Decode(dst).c_str(), src);
		}

		{
			const char * src = "b+-f*0^1$8%7';2^#$)(_c)&$#!!^9";
			const char * dst = "b%2B-f*0%5E1%248%257%27%3B2%5E%23%24)(_c)%26%24%23!!%5E9";

			EXPECT_STR_EQ(tinyToolkit::UrlTransform::Encode(reinterpret_cast<const uint8_t *>(src)).c_str(), dst);
			EXPECT_STR_EQ(tinyToolkit::UrlTransform::Decode(reinterpret_cast<const uint8_t *>(dst)).c_str(), src);
		}

		{
			const char * src = "b+-f*0^1$8%7';2^#$)(_c)&$#!!^9";
			const char * dst = "b%2B-f*0%5E1%248%257%27%3B2%5E%23%24)(_c)%26%24%23!!%5E9";

			EXPECT_STR_EQ(tinyToolkit::UrlTransform::Encode(std::string(src)).c_str(), dst);
			EXPECT_STR_EQ(tinyToolkit::UrlTransform::Decode(std::string(dst)).c_str(), src);
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(System, OS)
{
	try
	{
		EXPECT_TRUE(tinyToolkit::OS::ThreadID());
		EXPECT_TRUE(tinyToolkit::OS::ProcessID());
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


#if TINY_TOOLKIT_PLATFORM != TINY_TOOLKIT_PLATFORM_WINDOWS

int32_t signalStatus = 0;

TEST(System, Signal)
{
	tinyToolkit::Signal::RegisterIgnore();

	tinyToolkit::Signal::RegisterTerminate([](int signalNo){ signalStatus = signalNo * 3; });
	tinyToolkit::Signal::RegisterStackTrace([](int signalNo){ signalStatus = signalNo * 1; });

	tinyToolkit::Signal::Raise(SIGTSTP);

	EXPECT_EQ(signalStatus, SIGTSTP * 3);

	tinyToolkit::Signal::Raise(SIGTSTP);

	EXPECT_EQ(signalStatus, SIGTSTP * 3);

	tinyToolkit::Signal::Raise(SIGPIPE);

	EXPECT_EQ(signalStatus, SIGTSTP * 3);

	tinyToolkit::Signal::Raise(SIGCHLD);

	EXPECT_EQ(signalStatus, SIGTSTP * 3);

	tinyToolkit::Signal::Raise(SIGSEGV);

	EXPECT_EQ(signalStatus, SIGSEGV);
}

#endif


TEST(System, Application)
{
	try
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		EXPECT_TRUE(tinyToolkit::Application::Exist()) << tinyToolkit::OS::LastErrorMessage();

		EXPECT_STR_EQ(tinyToolkit::Application::Name().c_str(), "benchmarkTest.exe");
		EXPECT_STR_EQ(tinyToolkit::Application::Extension().c_str(), ".exe");

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		EXPECT_FALSE(tinyToolkit::Application::Exist()) << tinyToolkit::OS::LastErrorMessage();

		EXPECT_STR_EQ(tinyToolkit::Application::Name().c_str(), "benchmarkTest");
		EXPECT_STR_EQ(tinyToolkit::Application::Extension().c_str(), "");

#else

		EXPECT_FALSE(tinyToolkit::Application::Exist()) << tinyToolkit::OS::LastErrorMessage();

		EXPECT_STR_EQ(tinyToolkit::Application::Name().c_str(), "benchmarkTest");
		EXPECT_STR_EQ(tinyToolkit::Application::Extension().c_str(), "");

#endif

		EXPECT_LE(tinyToolkit::Application::CompileTime(), tinyToolkit::Time::Seconds());
		EXPECT_EQ(tinyToolkit::Application::CompileTime(), tinyToolkit::Time::FromTimeString(tinyToolkit::Application::CompileTimeString()));

		EXPECT_STR_EQ(tinyToolkit::Application::Steam().c_str(), "benchmarkTest");
		EXPECT_STR_EQ(tinyToolkit::Application::Path().c_str(), (tinyToolkit::Application::Directory() + TINY_TOOLKIT_FOLDER_SEP + tinyToolkit::Application::Name()).c_str());
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Utilities, Net)
{
	try
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		WSAData wsaData;

		WSAStartup(MAKEWORD(2, 2), &wsaData);

#endif

		{
			std::vector<std::string> list;

			tinyToolkit::Net::TraverseAddressFromHost("127.0.0.1", list);

			EXPECT_EQ(list.size(), static_cast<std::size_t>(1));
		}

		{
			std::vector<std::string> list;

			tinyToolkit::Net::TraverseAddressFromHost("192.168.2.1", list);

			EXPECT_EQ(list.size(), static_cast<std::size_t>(1));
		}

		EXPECT_STR_EQ(tinyToolkit::Net::AsString(16951488).c_str(), "1.2.168.192");
		EXPECT_STR_EQ(tinyToolkit::Net::AsString(3232236033).c_str(), "192.168.2.1");

		EXPECT_EQ(tinyToolkit::Net::AsNetByte("192.168.2.1"), static_cast<uint32_t>(16951488));
		EXPECT_EQ(tinyToolkit::Net::AsNetByte("1.2.168.192"), static_cast<uint32_t>(3232236033));

		EXPECT_EQ(tinyToolkit::Net::AsHostByte("192.168.2.1"), static_cast<uint32_t>(3232236033));
		EXPECT_EQ(tinyToolkit::Net::AsHostByte("1.2.168.192"), static_cast<uint32_t>(16951488));

		{
			uint32_t head = 0;
			uint32_t tail = 0;

			EXPECT_TRUE(tinyToolkit::Net::AsNetByte("192.168.2.1", head, tail));

			EXPECT_EQ(head, tinyToolkit::Net::AsNetByte("192.168.2.1"));
			EXPECT_EQ(tail, tinyToolkit::Net::AsNetByte("192.168.2.1"));
		}

		{
			uint32_t head = 0;
			uint32_t tail = 0;

			EXPECT_TRUE(tinyToolkit::Net::AsNetByte("192.168.2.1/24", head, tail));

			EXPECT_EQ(head, tinyToolkit::Net::AsNetByte("192.168.2.1"));
			EXPECT_EQ(tail, tinyToolkit::Net::AsNetByte("192.168.2.255"));
		}

		{
			uint32_t head = 0;
			uint32_t tail = 0;

			EXPECT_TRUE(tinyToolkit::Net::AsNetByte("192.168.2.1-192.168.2.255", head, tail));

			EXPECT_EQ(head, tinyToolkit::Net::AsNetByte("192.168.2.1"));
			EXPECT_EQ(tail, tinyToolkit::Net::AsNetByte("192.168.2.255"));
		}

		{
			uint32_t head = 0;
			uint32_t tail = 0;

			EXPECT_TRUE(tinyToolkit::Net::AsNetByte("192.168.2.1/192.168.2.255", head, tail));

			EXPECT_EQ(head, tinyToolkit::Net::AsNetByte("192.168.2.1"));
			EXPECT_EQ(tail, tinyToolkit::Net::AsNetByte("192.168.2.255"));
		}

		{
			uint32_t head = 0;
			uint32_t tail = 0;

			EXPECT_TRUE(tinyToolkit::Net::AsHostByte("192.168.2.1", head, tail));

			EXPECT_EQ(head, tinyToolkit::Net::AsHostByte("192.168.2.1"));
			EXPECT_EQ(tail, tinyToolkit::Net::AsHostByte("192.168.2.1"));
		}

		{
			uint32_t head = 0;
			uint32_t tail = 0;

			EXPECT_TRUE(tinyToolkit::Net::AsHostByte("192.168.2.1/24", head, tail));

			EXPECT_EQ(head, tinyToolkit::Net::AsHostByte("192.168.2.1"));
			EXPECT_EQ(tail, tinyToolkit::Net::AsHostByte("192.168.2.255"));
		}

		{
			uint32_t head = 0;
			uint32_t tail = 0;

			EXPECT_TRUE(tinyToolkit::Net::AsHostByte("192.168.2.1-192.168.2.255", head, tail));

			EXPECT_EQ(head, tinyToolkit::Net::AsHostByte("192.168.2.1"));
			EXPECT_EQ(tail, tinyToolkit::Net::AsHostByte("192.168.2.255"));
		}

		{
			uint32_t head = 0;
			uint32_t tail = 0;

			EXPECT_TRUE(tinyToolkit::Net::AsHostByte("192.168.2.1/192.168.2.255", head, tail));

			EXPECT_EQ(head, tinyToolkit::Net::AsHostByte("192.168.2.1"));
			EXPECT_EQ(tail, tinyToolkit::Net::AsHostByte("192.168.2.255"));
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		WSACleanup();

#endif
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Utilities, Math)
{
	try
	{
		EXPECT_TRUE(tinyToolkit::Math::IsIP("0.0.0.0"));
		EXPECT_TRUE(tinyToolkit::Math::IsIP("255.255.255.255"));

		EXPECT_FALSE(tinyToolkit::Math::IsIP("0.0.0.256"));
		EXPECT_FALSE(tinyToolkit::Math::IsIP("255.255.255.256"));

		EXPECT_TRUE(tinyToolkit::Math::IsIP(std::string("0.0.0.0")));
		EXPECT_TRUE(tinyToolkit::Math::IsIP(std::string("255.255.255.255")));

		EXPECT_FALSE(tinyToolkit::Math::IsIP(std::string("0.0.0.256")));
		EXPECT_FALSE(tinyToolkit::Math::IsIP(std::string("255.255.255.256")));

		EXPECT_TRUE(tinyToolkit::Math::IsOdd(1));
		EXPECT_TRUE(tinyToolkit::Math::IsOdd(255));

		EXPECT_FALSE(tinyToolkit::Math::IsOdd(2));
		EXPECT_FALSE(tinyToolkit::Math::IsOdd(256));

		EXPECT_TRUE(tinyToolkit::Math::IsEven(6));
		EXPECT_TRUE(tinyToolkit::Math::IsEven(388));

		EXPECT_FALSE(tinyToolkit::Math::IsEven(123));
		EXPECT_FALSE(tinyToolkit::Math::IsEven(567));
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Utilities, Time)
{
	try
	{
		struct timeval v1{ };
		struct timespec v2{ };

		v1.tv_sec = 123;
		v1.tv_usec = 456;

		v2.tv_sec = 123;
		v2.tv_nsec = 567;

		EXPECT_TRUE(tinyToolkit::Time::IsSameYear(tinyToolkit::Time::FromTimeString("2018-01-02 03:04:05"),
												  tinyToolkit::Time::FromTimeString("2018-11-12 13:14:15")));
		EXPECT_TRUE(tinyToolkit::Time::IsSameMonth(tinyToolkit::Time::FromTimeString("2018-01-02 03:04:05"),
												   tinyToolkit::Time::FromTimeString("2018-01-12 13:14:15")));
		EXPECT_TRUE(tinyToolkit::Time::IsSameDay(tinyToolkit::Time::FromTimeString("2018-01-02 03:04:05"),
												 tinyToolkit::Time::FromTimeString("2018-01-02 13:14:15")));
		EXPECT_TRUE(tinyToolkit::Time::IsSameHour(tinyToolkit::Time::FromTimeString("2018-01-02 03:04:05"),
												  tinyToolkit::Time::FromTimeString("2018-01-02 03:14:15")));
		EXPECT_TRUE(tinyToolkit::Time::IsSameMinute(tinyToolkit::Time::FromTimeString("2018-01-02 03:04:05"),
													tinyToolkit::Time::FromTimeString("2018-01-02 03:04:15")));

		auto utcTm = tinyToolkit::Time::UTCTm();
		auto localTm = tinyToolkit::Time::LocalTm();

		EXPECT_EQ((localTm.tm_mday - utcTm.tm_mday) * 24 + localTm.tm_hour - utcTm.tm_hour, 8);
		EXPECT_EQ(tinyToolkit::Time::Cast<int32_t>(), tinyToolkit::Time::Seconds());
		EXPECT_EQ(tinyToolkit::Time::TimeZone(), 8);
		EXPECT_EQ(tinyToolkit::Time::Hours(), tinyToolkit::Time::Minutes() / 60);
		EXPECT_EQ(tinyToolkit::Time::Minutes(), tinyToolkit::Time::Seconds() / 60);
		EXPECT_EQ(tinyToolkit::Time::Seconds(), tinyToolkit::Time::Milliseconds() / 1000);
		EXPECT_EQ(tinyToolkit::Time::Milliseconds(), tinyToolkit::Time::Microseconds() / 1000);

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		EXPECT_LE(tinyToolkit::Time::Microseconds(), tinyToolkit::Time::Nanoseconds() / 1000);

#else

		EXPECT_GE(tinyToolkit::Time::Microseconds(), tinyToolkit::Time::Nanoseconds() / 1000);

#endif

		EXPECT_EQ(tinyToolkit::Time::NextDayTime(), tinyToolkit::Time::CurrentDayTime() + TINY_TOOLKIT_DAY);
		EXPECT_EQ(tinyToolkit::Time::Seconds(tinyToolkit::Time::TimeDuration()), tinyToolkit::Time::Seconds(tinyToolkit::Time::TimePoint()));
		EXPECT_EQ(tinyToolkit::Time::Seconds(tinyToolkit::Time::TimeDuration(123)), tinyToolkit::Time::Seconds(tinyToolkit::Time::TimePoint(123)));
		EXPECT_EQ(tinyToolkit::Time::Seconds(tinyToolkit::Time::TimeDuration(v1)), tinyToolkit::Time::Seconds(tinyToolkit::Time::TimePoint(v1)));
		EXPECT_EQ(tinyToolkit::Time::Seconds(tinyToolkit::Time::TimeDuration(v2)), tinyToolkit::Time::Seconds(tinyToolkit::Time::TimePoint(v2)));
		EXPECT_EQ(tinyToolkit::Time::FromTimeString("2018-01-02 03:04:05"), 1514833445);

		EXPECT_STR_EQ(tinyToolkit::Time::FormatTimeString(tinyToolkit::Time::Seconds(), static_cast<std::time_t>(0)).c_str(), tinyToolkit::Time::CurrentUTCTimeString().c_str());
		EXPECT_STR_EQ(tinyToolkit::Time::FormatTimeString(1514833445).c_str(), "2018-01-02 03:04:05");
		EXPECT_STR_EQ(tinyToolkit::Time::FormatTimeString(1514833445, "%4d/%02d/%02d %02d:%02d:%02d").c_str(), "2018/01/02 03:04:05");
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Utilities, Defer)
{
	try
	{
		int i = 0;

		{
			TINY_TOOLKIT_DEFER
			(
					EXPECT_EQ(i, 200);

					i += 100;

					EXPECT_EQ(i, 300);
			);

			TINY_TOOLKIT_DEFER
			(
					EXPECT_EQ(i, 0);

					i += 200;

					EXPECT_EQ(i, 200);
			);

			EXPECT_EQ(i, 0);
		}

		EXPECT_EQ(i, 300);
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Utilities, Option)
{
	try
	{
		tinyToolkit::OptionManager manager;

		EXPECT_TRUE(manager.Empty());

		EXPECT_FALSE(manager.Has("test"));
		EXPECT_FALSE(manager.Has("next"));
		EXPECT_FALSE(manager.Has("need"));
		EXPECT_FALSE(manager.Has(std::string("test")));
		EXPECT_FALSE(manager.Has(std::string("next")));
		EXPECT_FALSE(manager.Has(std::string("need")));

		manager.Define("test", "this is test description");
		manager.DefineArg("next", "this is next arg", "123");
		manager.DefineArg("need", "this is need input arg");

		manager.DefineVersion(tinyToolkit::Application::CompileTimeString());

		EXPECT_TRUE(manager.Empty());

		EXPECT_FALSE(manager.Has("test"));
		EXPECT_FALSE(manager.Has("next"));
		EXPECT_FALSE(manager.Has("need"));
		EXPECT_FALSE(manager.Has(std::string("test")));
		EXPECT_FALSE(manager.Has(std::string("next")));
		EXPECT_FALSE(manager.Has(std::string("need")));

		const char * arg[] = { "example", "--test", "--next=456", "--need=abc" };

		manager.Parse(4, arg);

		EXPECT_TRUE(manager.Has("test"));
		EXPECT_TRUE(manager.Has("next"));
		EXPECT_TRUE(manager.Has("need"));
		EXPECT_TRUE(manager.Has(std::string("test")));
		EXPECT_TRUE(manager.Has(std::string("next")));
		EXPECT_TRUE(manager.Has(std::string("need")));

		EXPECT_STR_EQ(manager.Get("next").c_str(), "456");
		EXPECT_STR_EQ(manager.Get("need").c_str(), "abc");
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Utilities, String)
{
	try
	{
		EXPECT_TRUE(tinyToolkit::String::With("123456", '4'));
		EXPECT_TRUE(tinyToolkit::String::With("123456", '4', 3));
		EXPECT_TRUE(tinyToolkit::String::With("123456", "45"));
		EXPECT_TRUE(tinyToolkit::String::With("123456", "45", 3));
		EXPECT_TRUE(tinyToolkit::String::With(std::string("123456"), '4'));
		EXPECT_TRUE(tinyToolkit::String::With(std::string("123456"), '4', 3));
		EXPECT_TRUE(tinyToolkit::String::With(std::string("123456"), "45"));
		EXPECT_TRUE(tinyToolkit::String::With(std::string("123456"), "45", 3));
		EXPECT_TRUE(tinyToolkit::String::With(std::string("123456"), std::string("45")));
		EXPECT_TRUE(tinyToolkit::String::With(std::string("123456"), std::string("45"), 3));
		EXPECT_TRUE(tinyToolkit::String::EndWith("123456", "56"));
		EXPECT_TRUE(tinyToolkit::String::StartWith("123456", "12"));

		EXPECT_FALSE(tinyToolkit::String::With("123456", '4', 4));
		EXPECT_FALSE(tinyToolkit::String::With("123456", "45", 4));
		EXPECT_FALSE(tinyToolkit::String::With(std::string("123456"), '4', 4));
		EXPECT_FALSE(tinyToolkit::String::With(std::string("123456"), "45", 4));
		EXPECT_FALSE(tinyToolkit::String::With(std::string("123456"), std::string("45"), 4));
		EXPECT_FALSE(tinyToolkit::String::EndWith("123456", "55"));
		EXPECT_FALSE(tinyToolkit::String::StartWith("123456", "13"));

		EXPECT_EQ(tinyToolkit::String::Split("121545-fdsa-23242-sdf8967sdf-es5dsfsd", "-").size(), static_cast<uint32_t>(5));
		EXPECT_EQ(tinyToolkit::String::SplitLines("121545\nfdsa\r\n23242\rsdf8967sdf\nes5dsfsd").size(), static_cast<uint32_t>(5));
		EXPECT_EQ(tinyToolkit::String::Transform<int32_t>("123456789.123456789"), 123456789);
		EXPECT_EQ(tinyToolkit::String::Transform<int32_t>(std::string("123456789.123456789")), 123456789);
		EXPECT_EQ(tinyToolkit::String::Transform<double>("123456789"), 123456789.0);
		EXPECT_EQ(tinyToolkit::String::Transform<double>(std::string("123456789")), 123456789.0);

		EXPECT_STR_EQ(tinyToolkit::String::Trim(" 123 456 789 ").c_str(), "123 456 789");
		EXPECT_STR_EQ(tinyToolkit::String::Lower("ABCDEfgHIjklm").c_str(), "abcdefghijklm");
		EXPECT_STR_EQ(tinyToolkit::String::Upper("ABCDEfgHIjklm").c_str(), "ABCDEFGHIJKLM");
		EXPECT_STR_EQ(tinyToolkit::String::Erase("ABCDEfgHIjklm", 'j').c_str(), "ABCDEfgHIklm");
		EXPECT_STR_EQ(tinyToolkit::String::Erase("ABCDEfgHIjklm", 'j', 9).c_str(), "ABCDEfgHIklm");
		EXPECT_STR_EQ(tinyToolkit::String::Erase("ABCDEfgHIjklm", 'j', 10).c_str(), "ABCDEfgHIjklm");
		EXPECT_STR_EQ(tinyToolkit::String::Erase("ABCDEfgHIjklm", "CD").c_str(), "ABEfgHIjklm");
		EXPECT_STR_EQ(tinyToolkit::String::Erase("ABCDEfgHIjklm", "CD", 2).c_str(), "ABEfgHIjklm");
		EXPECT_STR_EQ(tinyToolkit::String::Erase("ABCDEfgHIjklm", "CD", 3).c_str(), "ABCDEfgHIjklm");
		EXPECT_STR_EQ(tinyToolkit::String::Erase("ABCDEfgHIjklm", [](char value) -> bool { return value == 'f'; }).c_str(), "ABCDEgHIjklm");
		EXPECT_STR_EQ(tinyToolkit::String::Erase("ABCDEfgHIjklm", [](char value) -> bool { return value == 'f'; }, 5).c_str(), "ABCDEgHIjklm");
		EXPECT_STR_EQ(tinyToolkit::String::Erase("ABCDEfgHIjklm", [](char value) -> bool { return value == 'f'; }, 6).c_str(), "ABCDEfgHIjklm");
		EXPECT_STR_EQ(tinyToolkit::String::Replace("1221212212", "12", "21").c_str(), "2122121221");
		EXPECT_STR_EQ(tinyToolkit::String::Replace("1221212212", "12", "21", 3).c_str(), "1222121221");
		EXPECT_STR_EQ(tinyToolkit::String::Replace("1221212212", "12", "21", 4).c_str(), "1221221221");
		EXPECT_STR_EQ(tinyToolkit::String::ReplaceAll("1221212212", "12", "21").c_str(), "2222221111");
		EXPECT_STR_EQ(tinyToolkit::String::ReplaceAll("1221212212", "12", "21", 3).c_str(), "1222222111");
		EXPECT_STR_EQ(tinyToolkit::String::ReplaceAll("1221212212", "12", "21", 4).c_str(), "1221222211");
		EXPECT_STR_EQ(tinyToolkit::String::FilterNote("123456//1234565abcd").c_str(), "123456");
		EXPECT_STR_EQ(tinyToolkit::String::Format("{} is {}", 1, true).c_str(), "1 is true");
		EXPECT_STR_EQ(tinyToolkit::String::Format("{0} is {0}, {1} is {1}, {2} is {2}", 1, 0.01, true).c_str(), "1 is 1, 0.01 is 0.01, true is true");
		EXPECT_STR_EQ(tinyToolkit::String::Join("this is ", "new string, value=", 0).c_str(), "this is new string, value=0");
		EXPECT_STR_EQ(tinyToolkit::String::AsHexString("123456789").c_str(), "313233343536373839");
		EXPECT_STR_EQ(tinyToolkit::String::AsHexString("123456789", true).c_str(), "393837363534333231");
		EXPECT_STR_EQ(tinyToolkit::String::AsHexString("123456789", 5, true).c_str(), "3534333231");
		EXPECT_STR_EQ(tinyToolkit::String::AsHexString("123456789", 5, false).c_str(), "3132333435");
		EXPECT_STR_EQ(tinyToolkit::String::AsHexString(std::string("123456789")).c_str(), "313233343536373839");
		EXPECT_STR_EQ(tinyToolkit::String::AsHexString(std::string("123456789"), true).c_str(), "393837363534333231");
		EXPECT_STR_EQ(tinyToolkit::String::AsHexString(std::string("123456789"), 5, true).c_str(), "3534333231");
		EXPECT_STR_EQ(tinyToolkit::String::AsHexString(std::string("123456789"), 5, false).c_str(), "3132333435");

		{
			std::stringstream stream;

			tinyToolkit::String::Print(stream, "{} is {}", 1, true);

			EXPECT_STR_EQ(stream.str().c_str(), "1 is true");
		}

		{
			std::stringstream stream;

			tinyToolkit::String::Print(stream, "{0} is {0}, {1} is {1}, {2} is {2}", 1, 0.01, true);

			EXPECT_STR_EQ(stream.str().c_str(), "1 is 1, 0.01 is 0.01, true is true");
		}

		{
			uint8_t val[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '\0' };

			EXPECT_STR_EQ(tinyToolkit::String::AsHexString(val).c_str(), "313233343536373839");
			EXPECT_STR_EQ(tinyToolkit::String::AsHexString(val, true).c_str(), "393837363534333231");
			EXPECT_STR_EQ(tinyToolkit::String::AsHexString(val, 5, true).c_str(), "3534333231");
			EXPECT_STR_EQ(tinyToolkit::String::AsHexString(val, 5, false).c_str(), "3132333435");
		}

		EXPECT_EQ(tinyToolkit::HexString::AsByte("0x0A"), 10);
		EXPECT_EQ(tinyToolkit::HexString::AsByte(std::string("0x0F")), 15);

		EXPECT_STR_EQ(tinyToolkit::HexString::AsString("313233343536373839").c_str(), "123456789");
		EXPECT_STR_EQ(tinyToolkit::HexString::AsString("313233343536373839", true).c_str(), "987654321");
		EXPECT_STR_EQ(tinyToolkit::HexString::AsString(std::string("313233343536373839")).c_str(), "123456789");
		EXPECT_STR_EQ(tinyToolkit::HexString::AsString(std::string("313233343536373839"), true).c_str(), "987654321");
		EXPECT_STR_EQ(tinyToolkit::HexString::AsString("313233343536373839", 10, true).c_str(), "54321");
		EXPECT_STR_EQ(tinyToolkit::HexString::AsString("313233343536373839", 10, false).c_str(), "12345");
		EXPECT_STR_EQ(tinyToolkit::HexString::AsString(std::string("313233343536373839"), 10, true).c_str(), "54321");
		EXPECT_STR_EQ(tinyToolkit::HexString::AsString(std::string("313233343536373839"), 10, false).c_str(), "12345");
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Utilities, Operator)
{
	try
	{
		std::vector<int32_t> vec;

		EXPECT_EQ(vec.size(), static_cast<std::size_t>(0));

		vec.push_back(1);
		vec.push_back(2);

		EXPECT_EQ(vec.size(), static_cast<std::size_t>(2));

		tinyToolkit::Operator::Clear(vec);

		EXPECT_EQ(vec.size(), static_cast<std::size_t>(0));

		vec.push_back(3);
		vec.push_back(4);
		vec.push_back(5);
		vec.push_back(6);

		EXPECT_EQ(vec.size(), static_cast<std::size_t>(4));
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


class SingletonTest
{
public:
	SingletonTest(int, int)
	{

	}
};


TEST(Utilities, Singleton)
{
	try
	{
		auto & t1 = tinyToolkit::Singleton<SingletonTest>::Instance(0, 1);
		auto & t2 = tinyToolkit::Singleton<SingletonTest>::Instance(0, 2);
		auto & t3 = tinyToolkit::Singleton<SingletonTest>::Instance(0, 3);

		EXPECT_EQ(std::addressof(t1), std::addressof(t2));
		EXPECT_EQ(std::addressof(t1), std::addressof(t3));
		EXPECT_EQ(std::addressof(t2), std::addressof(t3));
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


TEST(Utilities, Filesystem)
{
	try
	{
		tinyToolkit::StringVector strVector{ "1", "2", "3", "", "", "6", "", "8" };

		std::string sf = tinyToolkit::String::Format("{0}{1}fileSystem", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);
		std::string s1 = tinyToolkit::String::Format("{0}{1}fileSystem{1}1.txt", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);
		std::string s2 = tinyToolkit::String::Format("{0}{1}fileSystem{1}2.txt", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);
		std::string sa = tinyToolkit::String::Format("{0}{1}fileSystem{1}a.txt", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);
		std::string sb = tinyToolkit::String::Format("{0}{1}fileSystem{1}b.txt", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP);

		EXPECT_TRUE(tinyToolkit::Filesystem::Exists(tinyToolkit::String::Format("{0}{1}f1{1}f2{1}f3{1}f4", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP)));

		EXPECT_FALSE(tinyToolkit::Filesystem::Exists(s1));
		EXPECT_FALSE(tinyToolkit::Filesystem::Exists(s2));

		EXPECT_TRUE(tinyToolkit::Filesystem::CreateFile(s1)) << tinyToolkit::OS::LastErrorMessage();
		EXPECT_TRUE(tinyToolkit::Filesystem::WriteFile(s2, strVector.begin(), strVector.end()));

		EXPECT_TRUE(tinyToolkit::Filesystem::Exists(s1));
		EXPECT_TRUE(tinyToolkit::Filesystem::Exists(s2));

		EXPECT_TRUE(tinyToolkit::Filesystem::Rename(s1, sa));
		EXPECT_TRUE(tinyToolkit::Filesystem::Rename(s2, sb));

		EXPECT_EQ(tinyToolkit::Filesystem::Size(sa), static_cast<std::size_t>(0));
		EXPECT_EQ(tinyToolkit::Filesystem::Size(sb), 5 + strlen(TINY_TOOLKIT_EOL) * strVector.size());

		EXPECT_EQ(tinyToolkit::Filesystem::ReadFile(sa).size(), static_cast<std::size_t>(0));
		EXPECT_EQ(tinyToolkit::Filesystem::ReadFile(sb, true).size(), strVector.size());

		EXPECT_TRUE(tinyToolkit::Filesystem::WriteFile(sa, "this is a.txt"));
		EXPECT_TRUE(tinyToolkit::Filesystem::WriteFile(sb, "this is b.txt"));

		EXPECT_STR_EQ(tinyToolkit::Filesystem::Content(sa).c_str(), "this is a.txt");
		EXPECT_STR_EQ(tinyToolkit::Filesystem::Content(sb).c_str(), "this is b.txt");

		EXPECT_STR_EQ(tinyToolkit::Filesystem::Name(sa).c_str(), "a.txt");
		EXPECT_STR_EQ(tinyToolkit::Filesystem::Steam(sa).c_str(), "a");
		EXPECT_STR_EQ(tinyToolkit::Filesystem::Extension(sb).c_str(), ".txt");
		EXPECT_STR_EQ(tinyToolkit::Filesystem::ParentDirectory(sb).c_str(), sf.c_str());
		EXPECT_STR_EQ(tinyToolkit::Filesystem::Canonical(".").c_str(), tinyToolkit::Filesystem::CurrentDirectory().c_str());

		EXPECT_TRUE(tinyToolkit::Filesystem::IsDirectory(sf));

		EXPECT_FALSE(tinyToolkit::Filesystem::IsDirectory(sa));
		EXPECT_FALSE(tinyToolkit::Filesystem::IsDirectory(sb));

		EXPECT_EQ(tinyToolkit::Filesystem::TraverseFile(sf).size(), static_cast<std::size_t>(2));
		EXPECT_EQ(tinyToolkit::Filesystem::TraverseFile(sf, true).size(), static_cast<std::size_t>(2));
		EXPECT_EQ(tinyToolkit::Filesystem::TraverseFile(sf, std::regex(".*.txt")).size(), static_cast<std::size_t>(2));
		EXPECT_EQ(tinyToolkit::Filesystem::TraverseFile(sf, std::regex(".*.txt"), true).size(), static_cast<std::size_t>(2));

		EXPECT_EQ(tinyToolkit::Filesystem::TraverseDirectory(tinyToolkit::Application::Directory()).size(), static_cast<std::size_t>(4));
		EXPECT_EQ(tinyToolkit::Filesystem::TraverseDirectory(tinyToolkit::Application::Directory(), true).size(), static_cast<std::size_t>(7));
		EXPECT_EQ(tinyToolkit::Filesystem::TraverseDirectory(tinyToolkit::Application::Directory(), std::regex(".*log")).size(), static_cast<std::size_t>(1));
		EXPECT_EQ(tinyToolkit::Filesystem::TraverseDirectory(tinyToolkit::Application::Directory(), std::regex(".*log"), true).size(), static_cast<std::size_t>(1));
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


class TestEnvironment : public tinyToolkit::TestEnvironment
{
public:
	void SetUp() override
	{
		try
		{
			tinyToolkit::Filesystem::Remove(tinyToolkit::String::Format("{0}{1}f1", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
			tinyToolkit::Filesystem::Remove(tinyToolkit::String::Format("{0}{1}log", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
			tinyToolkit::Filesystem::Remove(tinyToolkit::String::Format("{0}{1}file", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
			tinyToolkit::Filesystem::Remove(tinyToolkit::String::Format("{0}{1}fileSystem", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));

			tinyToolkit::Filesystem::CreateDirectory(tinyToolkit::String::Format("{0}{1}log", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
			tinyToolkit::Filesystem::CreateDirectory(tinyToolkit::String::Format("{0}{1}file", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
			tinyToolkit::Filesystem::CreateDirectory(tinyToolkit::String::Format("{0}{1}fileSystem", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
			tinyToolkit::Filesystem::CreateDirectories(tinyToolkit::String::Format("{0}{1}f1{1}f2{1}f3{1}f4", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
		}
		catch (const std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	void TearDown() override
	{
		try
		{
			tinyToolkit::Filesystem::Remove(tinyToolkit::String::Format("{0}{1}f1", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
			tinyToolkit::Filesystem::Remove(tinyToolkit::String::Format("{0}{1}log", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
			tinyToolkit::Filesystem::Remove(tinyToolkit::String::Format("{0}{1}file", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
			tinyToolkit::Filesystem::Remove(tinyToolkit::String::Format("{0}{1}fileSystem", tinyToolkit::Application::Directory(), TINY_TOOLKIT_FOLDER_SEP));
		}
		catch (const std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
	}
};


int main(int argc, char const * argv[])
{
	tinyToolkit::OptionManager::Instance().Parse(argc, argv);

	TINY_TOOLKIT_ADD_TEST_ENVIRONMENT(TestEnvironment);

	return RUN_ALL_TEST();
}
