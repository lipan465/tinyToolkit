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
	EXPECT_LE(tinyToolkit::Random::IntUniformDistribution(0, 65535), 65535);
	EXPECT_LE(tinyToolkit::Random::FloatUniformDistribution(0.0, 65535.0), 65535.0);
}


TEST(ID, UniqueID)
{
	EXPECT_LE(tinyToolkit::UniqueID::Get(), tinyToolkit::UniqueID::Get() + 1);
}


TEST(ID, Snowflake)
{
	EXPECT_LE(tinyToolkit::Snowflake::Get(0, 0), tinyToolkit::Snowflake::Get(0, 0) + 1);
	EXPECT_LE(tinyToolkit::Snowflake::Get(0, 1), tinyToolkit::Snowflake::Get(0, 1) + 1);
	EXPECT_LE(tinyToolkit::Snowflake::Get(1, 0), tinyToolkit::Snowflake::Get(1, 0) + 1);
	EXPECT_LE(tinyToolkit::Snowflake::Get(1, 1), tinyToolkit::Snowflake::Get(1, 1) + 1);
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
	std::size_t count = 100;

	std::string fileName = tinyToolkit::String::Format("log{}syncFile.log", TINY_TOOLKIT_FOLDER_SEP);
	std::string realFileName = fileName;

	auto sink = std::make_shared<tinyToolkit::FileLogSink>("syncFile", fileName, true);

	SyncLogger(sink, count);

	EXPECT_EQ(tinyToolkit::Filesystem::Size(realFileName),
			  ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
}


TEST(LogSync, OStreamSink)
{
	std::size_t count = 100;

	auto stream = std::make_shared<std::stringstream>();

	auto sink = std::make_shared<tinyToolkit::OStreamLogSink>("syncOStream", stream);

	SyncLogger(sink, count);

	EXPECT_EQ(stream->str().size(), ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
}


#if TINY_TOOLKIT_PLATFORM != TINY_TOOLKIT_PLATFORM_WINDOWS

TEST(LogSync, Syslog)
{
	std::size_t count = 100;

	auto sink = std::make_shared<tinyToolkit::SyslogLogSink>("syncSyslog");

	SyncLogger(sink, count);
}

#endif


TEST(LogSync, DailyFileSink)
{
	std::size_t count = 100;

	std::string fileName = tinyToolkit::String::Format("log{}syncDailyFile.log", TINY_TOOLKIT_FOLDER_SEP);
	std::string realFileName = tinyToolkit::String::Format
	(
		"{}_{}{}",
		fileName.substr(0, fileName.rfind('.')),
		tinyToolkit::Time::CurrentLocalTimeString("%04d_%02d_%02d_%02d_%02d_%02d"),
		fileName.substr(fileName.rfind('.'))
	);

	auto sink = std::make_shared<tinyToolkit::DailyFileLogSink>("syncDailyFile", fileName);

	SyncLogger(sink, count);

	EXPECT_EQ(tinyToolkit::Filesystem::Size(realFileName),
			  ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
}


TEST(LogSync, StringQueueSink)
{
	std::size_t count = 100;

	auto sink = std::make_shared<tinyToolkit::StringQueueLogSink>("syncStringQueue");

	SyncLogger(sink, count);

	EXPECT_EQ(sink->Size(), 4 * count);
}


TEST(LogSync, RotatingFileSink)
{
	std::size_t count = 100;

	std::string fileName = tinyToolkit::String::Format("log{}syncRotatingFile.log", TINY_TOOLKIT_FOLDER_SEP);
	std::string realFileName = fileName;

	auto sink = std::make_shared<tinyToolkit::RotatingFileLogSink>("syncRotatingFile", fileName, TINY_TOOLKIT_MB);

	SyncLogger(sink, count);

	EXPECT_LE(tinyToolkit::Filesystem::Size(realFileName), static_cast<std::size_t>(TINY_TOOLKIT_MB));
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
	std::size_t count = 100;

	std::string fileName = tinyToolkit::String::Format("log{}asyncFile.log", TINY_TOOLKIT_FOLDER_SEP);
	std::string realFileName = fileName;

	auto sink = std::make_shared<tinyToolkit::FileLogSink>("asyncFile", fileName, true);

	AsyncLogger(sink, count, std::thread::hardware_concurrency());

	EXPECT_EQ(tinyToolkit::Filesystem::Size(realFileName),
			  ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
}


#if TINY_TOOLKIT_PLATFORM != TINY_TOOLKIT_PLATFORM_WINDOWS

TEST(LogAsync, Syslog)
{
	std::size_t count = 100;

	auto sink = std::make_shared<tinyToolkit::SyslogLogSink>("syncSyslog");

	AsyncLogger(sink, count, std::thread::hardware_concurrency());
}

#endif


TEST(LogAsync, OStreamSink)
{
	std::size_t count = 100;

	auto stream = std::make_shared<std::stringstream>();

	auto sink = std::make_shared<tinyToolkit::OStreamLogSink>("asyncOStream", stream);

	AsyncLogger(sink, count, std::thread::hardware_concurrency());

	EXPECT_EQ(stream->str().size(), ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
}


TEST(LogAsync, DailyFileSink)
{
	std::size_t count = 100;

	std::string fileName = tinyToolkit::String::Format("log{}asyncDailyFile.log", TINY_TOOLKIT_FOLDER_SEP);
	std::string realFileName = tinyToolkit::String::Format
			(
					"{}_{}{}",
					fileName.substr(0, fileName.rfind('.')),
					tinyToolkit::Time::CurrentLocalTimeString("%04d_%02d_%02d_%02d_%02d_%02d"),
					fileName.substr(fileName.rfind('.'))
			);

	auto sink = std::make_shared<tinyToolkit::DailyFileLogSink>("asyncDailyFile", fileName);

	AsyncLogger(sink, count, std::thread::hardware_concurrency());

	EXPECT_EQ(tinyToolkit::Filesystem::Size(realFileName),
			  ((49 + tinyToolkit::Application::Name().size() + strlen(TINY_TOOLKIT_EOL)) * 4 + (4 + 5) * 2) * count);
}


TEST(LogAsync, StringQueueSink)
{
	std::size_t count = 10000;

	auto sink = std::make_shared<tinyToolkit::StringQueueLogSink>("asyncStringQueue");

	AsyncLogger(sink, count, std::thread::hardware_concurrency());

	EXPECT_EQ(sink->Size(), 4 * count);
}


TEST(LogAsync, RotatingFileSink)
{
	std::size_t count = 100;

	std::string fileName = tinyToolkit::String::Format("log{}asyncRotatingFile.log", TINY_TOOLKIT_FOLDER_SEP);
	std::string realFileName = fileName;

	auto sink = std::make_shared<tinyToolkit::RotatingFileLogSink>("asyncRotatingFile", fileName, TINY_TOOLKIT_MB);

	AsyncLogger(sink, count, std::thread::hardware_concurrency());

	EXPECT_LE(tinyToolkit::Filesystem::Size(realFileName), static_cast<std::size_t>(TINY_TOOLKIT_MB));
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
}


TEST(Timer, EventCricle)
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
}


TEST(Timer, EventLessCount)
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
}


TEST(Timer, EventGreaterCount)
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
}


TEST(Timer, FunctionNone)
{
	int32_t count = 0;

	tinyToolkit::TimerManager manager;

	manager.Start([&](){ ++count; }, 0, 10);

	EXPECT_EQ(count, 0);

	TINY_TOOLKIT_SLEEP_MS(30);

	EXPECT_EQ(count, 0);
}


TEST(Timer, FunctionCricle)
{
	int32_t count = 0;

	tinyToolkit::TimerManager manager;

	manager.Start([&](){ ++count; }, -1, 10);

	EXPECT_EQ(count, 0);

	TINY_TOOLKIT_SLEEP_MS(30);

	EXPECT_GE(count, 2);
}


TEST(Timer, FunctionLessCount)
{
	int32_t count = 0;

	tinyToolkit::TimerManager manager;

	manager.Start([&](){ ++count; }, 2, 10);

	EXPECT_EQ(count, 0);

	TINY_TOOLKIT_SLEEP_MS(30);

	EXPECT_EQ(count, 2);
}

TEST(Timer, FunctionGreaterCount)
{
	int32_t count = 0;

	tinyToolkit::TimerManager manager;

	manager.Start([&](){ ++count; }, 15, 10);

	EXPECT_EQ(count, 0);

	TINY_TOOLKIT_SLEEP_MS(30);

	EXPECT_GE(count, 2);
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
	std::atomic<int32_t> result = 0;

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


TEST(Crypto, MD5)
{
	{
		tinyToolkit::MD5 md5;

		md5.Update("123");

		EXPECT_STR_EQ(md5.Value().c_str(), "123");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ac59075b964b0715");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "202cb962ac59075b964b07152d234b70");

		md5.Update("abc");

		EXPECT_STR_EQ(md5.Value().c_str(), "123abc");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "5769fa7361d7ecc6");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a906449d5769fa7361d7ecc6aa3f6d28");

		md5.Reset();
		md5.Update("xyz");

		EXPECT_STR_EQ(md5.Value().c_str(), "xyz");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "0911f878998c1361");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "d16fb36f0911f878998c136191af705e");

		md5.Reset();
		md5.Update("4567890");

		EXPECT_STR_EQ(md5.Value().c_str(), "4567890");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ad4c127657d9c8d9");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a14b1bbaad4c127657d9c8d907fc6a75");
	}

	{
		tinyToolkit::MD5 md5("123");

		EXPECT_STR_EQ(md5.Value().c_str(), "123");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ac59075b964b0715");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "202cb962ac59075b964b07152d234b70");

		md5.Update("abc");

		EXPECT_STR_EQ(md5.Value().c_str(), "123abc");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "5769fa7361d7ecc6");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a906449d5769fa7361d7ecc6aa3f6d28");

		md5.Reset();
		md5.Update("xyz");

		EXPECT_STR_EQ(md5.Value().c_str(), "xyz");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "0911f878998c1361");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "d16fb36f0911f878998c136191af705e");

		md5.Reset();
		md5.Update("4567890");

		EXPECT_STR_EQ(md5.Value().c_str(), "4567890");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ad4c127657d9c8d9");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a14b1bbaad4c127657d9c8d907fc6a75");
	}

	{
		uint8_t str[4] = { '1', '2', '3', '\0' };

		tinyToolkit::MD5 md5(str);

		EXPECT_STR_EQ(md5.Value().c_str(), "123");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ac59075b964b0715");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "202cb962ac59075b964b07152d234b70");

		md5.Update("abc");

		EXPECT_STR_EQ(md5.Value().c_str(), "123abc");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "5769fa7361d7ecc6");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a906449d5769fa7361d7ecc6aa3f6d28");

		md5.Reset();
		md5.Update("xyz");

		EXPECT_STR_EQ(md5.Value().c_str(), "xyz");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "0911f878998c1361");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "d16fb36f0911f878998c136191af705e");

		md5.Reset();
		md5.Update("4567890");

		EXPECT_STR_EQ(md5.Value().c_str(), "4567890");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ad4c127657d9c8d9");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a14b1bbaad4c127657d9c8d907fc6a75");
	}

	{
		tinyToolkit::MD5 md5(std::string("123"));

		EXPECT_STR_EQ(md5.Value().c_str(), "123");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ac59075b964b0715");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "202cb962ac59075b964b07152d234b70");

		md5.Update("abc");

		EXPECT_STR_EQ(md5.Value().c_str(), "123abc");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "5769fa7361d7ecc6");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a906449d5769fa7361d7ecc6aa3f6d28");

		md5.Reset();
		md5.Update("xyz");

		EXPECT_STR_EQ(md5.Value().c_str(), "xyz");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "0911f878998c1361");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "d16fb36f0911f878998c136191af705e");

		md5.Reset();
		md5.Update("4567890");

		EXPECT_STR_EQ(md5.Value().c_str(), "4567890");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ad4c127657d9c8d9");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a14b1bbaad4c127657d9c8d907fc6a75");
	}

	{
		tinyToolkit::MD5 md5("123456789", 3);

		EXPECT_STR_EQ(md5.Value().c_str(), "123");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ac59075b964b0715");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "202cb962ac59075b964b07152d234b70");

		md5.Update("abc");

		EXPECT_STR_EQ(md5.Value().c_str(), "123abc");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "5769fa7361d7ecc6");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a906449d5769fa7361d7ecc6aa3f6d28");

		md5.Reset();
		md5.Update("xyz");

		EXPECT_STR_EQ(md5.Value().c_str(), "xyz");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "0911f878998c1361");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "d16fb36f0911f878998c136191af705e");

		md5.Reset();
		md5.Update("4567890");

		EXPECT_STR_EQ(md5.Value().c_str(), "4567890");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ad4c127657d9c8d9");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a14b1bbaad4c127657d9c8d907fc6a75");
	}

	{
		uint8_t str[7] = { '1', '2', '3', '4', '5', '6', '\0' };

		tinyToolkit::MD5 md5(str, 3);

		EXPECT_STR_EQ(md5.Value().c_str(), "123");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ac59075b964b0715");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "202cb962ac59075b964b07152d234b70");

		md5.Update("abc");

		EXPECT_STR_EQ(md5.Value().c_str(), "123abc");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "5769fa7361d7ecc6");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a906449d5769fa7361d7ecc6aa3f6d28");

		md5.Reset();
		md5.Update("xyz");

		EXPECT_STR_EQ(md5.Value().c_str(), "xyz");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "0911f878998c1361");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "d16fb36f0911f878998c136191af705e");

		md5.Reset();
		md5.Update("4567890");

		EXPECT_STR_EQ(md5.Value().c_str(), "4567890");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ad4c127657d9c8d9");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a14b1bbaad4c127657d9c8d907fc6a75");
	}

	{
		tinyToolkit::MD5 md5(std::string("123456789"), 3);

		EXPECT_STR_EQ(md5.Value().c_str(), "123");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ac59075b964b0715");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "202cb962ac59075b964b07152d234b70");

		md5.Update("abc");

		EXPECT_STR_EQ(md5.Value().c_str(), "123abc");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "5769fa7361d7ecc6");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a906449d5769fa7361d7ecc6aa3f6d28");

		md5.Reset();
		md5.Update("xyz");

		EXPECT_STR_EQ(md5.Value().c_str(), "xyz");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "0911f878998c1361");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "d16fb36f0911f878998c136191af705e");

		md5.Reset();
		md5.Update("4567890");

		EXPECT_STR_EQ(md5.Value().c_str(), "4567890");
		EXPECT_STR_EQ(md5.Hex16().c_str(), "ad4c127657d9c8d9");
		EXPECT_STR_EQ(md5.Hex32().c_str(), "a14b1bbaad4c127657d9c8d907fc6a75");
	}
}


TEST(Crypto, Base64)
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


TEST(Crypto, UrlTransform)
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


TEST(System, OS)
{
	EXPECT_TRUE(tinyToolkit::OS::ThreadID());
	EXPECT_TRUE(tinyToolkit::OS::ProcessID());
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
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	EXPECT_TRUE(tinyToolkit::Application::Exist()) << strerror(errno);

	EXPECT_STR_EQ(tinyToolkit::Application::Name().c_str(), "example.exe");
	EXPECT_STR_EQ(tinyToolkit::Application::Extension().c_str(), ".exe");

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

	EXPECT_TRUE(tinyToolkit::Application::Exist()) << tinyToolkit::OS::LastErrorMessage();

	EXPECT_STR_EQ(tinyToolkit::Application::Name().c_str(), "example");
	EXPECT_STR_EQ(tinyToolkit::Application::Extension().c_str(), "");

#else

	EXPECT_FALSE(tinyToolkit::Application::Exist()) << strerror(errno);

	EXPECT_STR_EQ(tinyToolkit::Application::Name().c_str(), "example");
	EXPECT_STR_EQ(tinyToolkit::Application::Extension().c_str(), "");

#endif

	EXPECT_LE(tinyToolkit::Application::CompileTime(), tinyToolkit::Time::Seconds());
	EXPECT_EQ(tinyToolkit::Application::CompileTime(), tinyToolkit::Time::FromTimeString(tinyToolkit::Application::CompileTimeString()));

	EXPECT_STR_EQ(tinyToolkit::Application::Steam().c_str(), "example");
	EXPECT_STR_EQ(tinyToolkit::Application::Path().c_str(),
				  (tinyToolkit::Application::Directory() + TINY_TOOLKIT_FOLDER_SEP + tinyToolkit::Application::Name()).c_str());
}


TEST(Utilities, Net)
{
	{
		std::vector<std::string> list;

		tinyToolkit::Net::TraverseAddressFromHost("127.0.0.1", list);

		EXPECT_EQ(list.size(), static_cast<std::size_t>(1));
		EXPECT_STR_EQ(list[list.size() - 1].c_str(), "127.0.0.1");
	}

	{
		std::vector<std::string> list;

		tinyToolkit::Net::TraverseAddressFromHost("192.168.2.1", list);

		EXPECT_EQ(list.size(), static_cast<std::size_t>(1));
		EXPECT_STR_EQ(list[list.size() - 1].c_str(), "192.168.2.1");
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
}


TEST(Utilities, Math)
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


TEST(Utilities, Time)
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


TEST(Utilities, Defer)
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


TEST(Utilities, Option)
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


TEST(Utilities, String)
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


TEST(Utilities, Operator)
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


class SingletonTest
{
public:
	SingletonTest(int, int)
	{

	}
};


TEST(Utilities, Singleton)
{
	auto & t1 = tinyToolkit::Singleton<SingletonTest>::Instance(0, 1);
	auto & t2 = tinyToolkit::Singleton<SingletonTest>::Instance(0, 2);
	auto & t3 = tinyToolkit::Singleton<SingletonTest>::Instance(0, 3);

	EXPECT_EQ(std::addressof(t1), std::addressof(t2));
	EXPECT_EQ(std::addressof(t1), std::addressof(t3));
	EXPECT_EQ(std::addressof(t2), std::addressof(t3));
}


TEST(Utilities, Filesystem)
{
	tinyToolkit::StringVector strVector{ "1", "2", "3", "", "", "6", "", "8" };

	std::string sf = tinyToolkit::String::Format("fileSystem");
	std::string s1 = tinyToolkit::String::Format("fileSystem{}1.txt", TINY_TOOLKIT_FOLDER_SEP);
	std::string s2 = tinyToolkit::String::Format("fileSystem{}2.txt", TINY_TOOLKIT_FOLDER_SEP);
	std::string sa = tinyToolkit::String::Format("fileSystem{}a.txt", TINY_TOOLKIT_FOLDER_SEP);
	std::string sb = tinyToolkit::String::Format("fileSystem{}b.txt", TINY_TOOLKIT_FOLDER_SEP);

	EXPECT_TRUE(tinyToolkit::Filesystem::Exists(tinyToolkit::String::Format("f1{0}f2{0}f3{0}f4", TINY_TOOLKIT_FOLDER_SEP)));

	EXPECT_FALSE(tinyToolkit::Filesystem::Exists(s1));
	EXPECT_FALSE(tinyToolkit::Filesystem::Exists(s2));

	EXPECT_TRUE(tinyToolkit::Filesystem::CreateFile(s1));
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

	EXPECT_EQ(tinyToolkit::Filesystem::TraverseDirectory(".").size(), static_cast<std::size_t>(4));
	EXPECT_EQ(tinyToolkit::Filesystem::TraverseDirectory(".", true).size(), static_cast<std::size_t>(7));
	EXPECT_EQ(tinyToolkit::Filesystem::TraverseDirectory(".", std::regex(".*log")).size(), static_cast<std::size_t>(1));
	EXPECT_EQ(tinyToolkit::Filesystem::TraverseDirectory(".", std::regex(".*log"), true).size(), static_cast<std::size_t>(1));
}


class TestEnvironment : public tinyToolkit::TestEnvironment
{
public:
	void SetUp() override
	{
		tinyToolkit::Filesystem::CreateDirectory("log");
		tinyToolkit::Filesystem::CreateDirectory("file");
		tinyToolkit::Filesystem::CreateDirectory("fileSystem");
		tinyToolkit::Filesystem::CreateDirectories(tinyToolkit::String::Format("f1{0}f2{0}f3{0}f4", TINY_TOOLKIT_FOLDER_SEP));
	}

	void TearDown() override
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		std::system("rd /s /q f1");
		std::system("rd /s /q log");
		std::system("rd /s /q file");
		std::system("rd /s /q fileSystem");

#else

		std::system("rm -rf f1");
		std::system("rm -rf log");
		std::system("rm -rf file");
		std::system("rm -rf fileSystem");

#endif
	}
};


int main(int argc, char const * argv[])
{
	tinyToolkit::OptionManager::Instance().Parse(argc, argv);

	TINY_TOOLKIT_ADD_TEST_ENVIRONMENT(TestEnvironment);

	int32_t res = RUN_ALL_TEST();

	while (true)
	{
		if (getchar() == 'q')
		{
			return res;
		}
	}
}
