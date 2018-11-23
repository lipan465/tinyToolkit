#ifndef __EXAMPLE__TEST__LOG__H__
#define __EXAMPLE__TEST__LOG__H__


#include "test.h"


class Log
{
public:
	static void Test()
	{
		std::size_t threadCount = 1;
		std::size_t messageCount = 1000000;

		std::cout << "*******************************************************************************" << std::endl;
		std::cout << "Write " << messageCount << " message to logger" << std::endl;
		std::cout << "*******************************************************************************" << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////

		SyncLogger::Instance().AddSink<tinyToolkit::FileLogSink>("syncFile", "syncFile.txt", true);
		AsyncLogger::Instance().AddSink<tinyToolkit::FileLogSink>("asyncFile", "asyncFile.txt", true);

		SyncLogger::Instance().SetSinkLayout(std::make_shared<SimpleLogLayout>());
		AsyncLogger::Instance().SetSinkLayout(std::make_shared<SimpleLogLayout>());

		SyncLogger::Instance().SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));
		AsyncLogger::Instance().SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));

		TestSync("file sync logger", messageCount);
		TestAsync("file async logger", messageCount, threadCount);

		SyncLogger::Instance().DelSink("syncFile");
		AsyncLogger::Instance().DelSink("asyncFile");

		////////////////////////////////////////////////////////////////////////////////////////////////////

		SyncLogger::Instance().AddSink<tinyToolkit::DailyFileLogSink>("syncDaily", "syncDaily.txt");
		AsyncLogger::Instance().AddSink<tinyToolkit::DailyFileLogSink>("asyncDaily", "asyncDaily.txt");

		SyncLogger::Instance().SetSinkLayout(std::make_shared<SimpleLogLayout>());
		AsyncLogger::Instance().SetSinkLayout(std::make_shared<SimpleLogLayout>());

		SyncLogger::Instance().SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));
		AsyncLogger::Instance().SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));

		TestSync("daily file sync logger", messageCount);
		TestAsync("daily file async logger", messageCount, threadCount);

		SyncLogger::Instance().DelSink("syncDaily");
		AsyncLogger::Instance().DelSink("asyncDaily");

		////////////////////////////////////////////////////////////////////////////////////////////////////

		SyncLogger::Instance().AddSink<tinyToolkit::RotatingLogFileSink>("syncRotating", "syncRotating.txt");
		AsyncLogger::Instance().AddSink<tinyToolkit::RotatingLogFileSink>("asyncRotating", "asyncRotating.txt");

		SyncLogger::Instance().SetSinkLayout(std::make_shared<SimpleLogLayout>());
		AsyncLogger::Instance().SetSinkLayout(std::make_shared<SimpleLogLayout>());

		SyncLogger::Instance().SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));
		AsyncLogger::Instance().SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));

		TestSync("rotating file sync logger", messageCount);
		TestAsync("rotating file async logger", messageCount, threadCount);

		SyncLogger::Instance().DelSink("syncRotating");
		AsyncLogger::Instance().DelSink("asyncRotating");

		////////////////////////////////////////////////////////////////////////////////////////////////////

		SyncLogger::Instance().AddSink<tinyToolkit::OStreamLogSink>("syncOStream", std::make_shared<std::ostringstream>());
		AsyncLogger::Instance().AddSink<tinyToolkit::OStreamLogSink>("asyncOStream", std::make_shared<std::ostringstream>());

		SyncLogger::Instance().SetSinkLayout(std::make_shared<SimpleLogLayout>());
		AsyncLogger::Instance().SetSinkLayout(std::make_shared<SimpleLogLayout>());

		SyncLogger::Instance().SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));
		AsyncLogger::Instance().SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));

		TestSync("stream sync logger", messageCount);
		TestAsync("stream async logger", messageCount, threadCount);

		SyncLogger::Instance().DelSink("syncOStream");
		AsyncLogger::Instance().DelSink("asyncOStream");

		////////////////////////////////////////////////////////////////////////////////////////////////////

		SyncLogger::Instance().AddSink<tinyToolkit::StringQueueLogSink>("syncStringQueue");
		AsyncLogger::Instance().AddSink<tinyToolkit::StringQueueLogSink>("asyncStringQueue");

		SyncLogger::Instance().SetSinkLayout(std::make_shared<SimpleLogLayout>());
		AsyncLogger::Instance().SetSinkLayout(std::make_shared<SimpleLogLayout>());

		SyncLogger::Instance().SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));
		AsyncLogger::Instance().SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));

		TestSync("string queue sync logger", messageCount);
		TestAsync("string queue async logger", messageCount, threadCount);

		SyncLogger::Instance().DelSink("syncStringQueue");
		AsyncLogger::Instance().DelSink("asyncStringQueue");

		////////////////////////////////////////////////////////////////////////////////////////////////////
	}

protected:
	static void TestSync(const char * description, const std::size_t count)
	{
		std::cout << std::endl << description << "..." << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////

		auto start_time = Time::TimePoint();

		for (std::size_t i = 0; i < count; ++i)
		{
			TINY_TOOLKIT_SYNC_LOG_INFO("Hello logger : this is  sync message, you can fast write")
		}

		auto stop_time = Time::TimePoint();

		SyncLogger::Instance().Wait();

		////////////////////////////////////////////////////////////////////////////////////////////////////

		std::double_t all = Time::Microseconds(stop_time - start_time);

		std::cout << "all   : " << tinyToolkit::String::AsLocalString(all) << " us" << std::endl;
		std::cout << "avg   : " << tinyToolkit::String::AsLocalString(all / count, 6) << " us" << std::endl;
		std::cout << "rate  : " << tinyToolkit::String::AsLocalString(count / all, 6) << " / us" << std::endl;
		std::cout << "count : " << tinyToolkit::String::AsLocalString(count) << std::endl << std::endl;
	}

	static void TestAsync(const char * description, const std::size_t count, const std::size_t threadCount)
	{
		std::cout << std::endl << description << "..." << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////

		ThreadPool threadPool(threadCount);

		std::atomic<std::size_t> counter{ 0 };

		auto start_time = Time::TimePoint();

		threadPool.Commit
		(
			[&]()
			{
				while (true)
				{
					if (++counter > count)
					{
						break;
					}

					TINY_TOOLKIT_ASYNC_LOG_INFO("Hello logger : this is async message, you can fast write")
				}
			}
		);

		threadPool.Wait();

		auto stop_time = Time::TimePoint();

		AsyncLogger::Instance().Wait();

		////////////////////////////////////////////////////////////////////////////////////////////////////

		std::double_t all = Time::Microseconds(stop_time - start_time);

		std::cout << "all   : " << tinyToolkit::String::AsLocalString(all) << " us" << std::endl;
		std::cout << "avg   : " << tinyToolkit::String::AsLocalString(all / count, 6) << " us" << std::endl;
		std::cout << "rate  : " << tinyToolkit::String::AsLocalString(count / all, 6) << " / us" << std::endl;
		std::cout << "count : " << tinyToolkit::String::AsLocalString(count) << std::endl << std::endl;
	}
};


#endif // __EXAMPLE__TEST__LOG__H__
