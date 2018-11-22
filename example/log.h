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

		std::cout << std::endl;
		std::cout << "*******************************************************************************" << std::endl;
		std::cout << "Write " << messageCount << " message to logger" << std::endl;
		std::cout << "*******************************************************************************" << std::endl;
		std::cout << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////

		sSyncLogger.AddSink<tinyToolkit::FileLogSink>("syncFile", "syncFile.txt", true);
		sAsyncLogger.AddSink<tinyToolkit::FileLogSink>("asyncFile", "asyncFile.txt", true);

		sSyncLogger.SetSinkLayout(std::make_shared<SimpleLogLayout>());
		sAsyncLogger.SetSinkLayout(std::make_shared<SimpleLogLayout>());

		sSyncLogger.SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));
		sAsyncLogger.SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));

		TestSync("file sync logger", messageCount);
		TestAsync("file async logger", messageCount, threadCount);

		sSyncLogger.DelSink("syncFile");
		sAsyncLogger.DelSink("asyncFile");

		std::cout << std::endl << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////

		sAsyncLogger.AddSink<tinyToolkit::DailyFileLogSink>("syncDaily", "syncDaily.txt");
		sAsyncLogger.AddSink<tinyToolkit::DailyFileLogSink>("asyncDaily", "asyncDaily.txt");

		sSyncLogger.SetSinkLayout(std::make_shared<SimpleLogLayout>());
		sAsyncLogger.SetSinkLayout(std::make_shared<SimpleLogLayout>());

		sSyncLogger.SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));
		sAsyncLogger.SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));

		TestSync("daily file sync logger", messageCount);
		TestAsync("daily file async logger", messageCount, threadCount);

		sSyncLogger.DelSink("syncDaily");
		sAsyncLogger.DelSink("asyncDaily");

		std::cout << std::endl << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////

		sAsyncLogger.AddSink<tinyToolkit::RotatingLogFileSink>("syncRotating", "syncRotating.txt");
		sAsyncLogger.AddSink<tinyToolkit::RotatingLogFileSink>("asyncRotating", "asyncRotating.txt");

		sSyncLogger.SetSinkLayout(std::make_shared<SimpleLogLayout>());
		sAsyncLogger.SetSinkLayout(std::make_shared<SimpleLogLayout>());

		sSyncLogger.SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));
		sAsyncLogger.SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));

		TestSync("rotating file sync logger", messageCount);
		TestAsync("rotating file async logger", messageCount, threadCount);

		sSyncLogger.DelSink("syncRotating");
		sAsyncLogger.DelSink("asyncRotating");

		std::cout << std::endl << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////

		sAsyncLogger.AddSink<tinyToolkit::OStreamLogSink>("syncOStream", std::make_shared<std::ostringstream>());
		sAsyncLogger.AddSink<tinyToolkit::OStreamLogSink>("asyncOStream", std::make_shared<std::ostringstream>());

		sSyncLogger.SetSinkLayout(std::make_shared<SimpleLogLayout>());
		sAsyncLogger.SetSinkLayout(std::make_shared<SimpleLogLayout>());

		sSyncLogger.SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));
		sAsyncLogger.SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));

		TestSync("stream sync logger", messageCount);
		TestAsync("stream async logger", messageCount, threadCount);

		sSyncLogger.DelSink("syncOStream");
		sAsyncLogger.DelSink("asyncOStream");

		std::cout << std::endl << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////

		sAsyncLogger.AddSink<tinyToolkit::StringQueueLogSink>("syncStringQueue");
		sAsyncLogger.AddSink<tinyToolkit::StringQueueLogSink>("asyncStringQueue");

		sSyncLogger.SetSinkLayout(std::make_shared<SimpleLogLayout>());
		sAsyncLogger.SetSinkLayout(std::make_shared<SimpleLogLayout>());

		sSyncLogger.SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));
		sAsyncLogger.SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL));

		TestSync("string queue sync logger", messageCount);
		TestAsync("string queue async logger", messageCount, threadCount);

		sSyncLogger.DelSink("syncStringQueue");
		sAsyncLogger.DelSink("asyncStringQueue");

		std::cout << std::endl << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////
	}

protected:
	static void TestSync(const char * description, const std::size_t count)
	{
		std::cout << description << "..." << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////

		tinyToolkit::TimeWatcher timeWatcher;

		timeWatcher.Start();

		for (std::size_t i = 0; i < count; ++i)
		{
			timeWatcher.ElapsedStart();

			TINY_TOOLKIT_SYNC_LOG_INFO("Hello logger : this is  sync message, you can fast write")

			timeWatcher.ElapsedStop();
		}

		sAsyncLogger.Wait();

		timeWatcher.Stop();

		std::cout << "max   : " << tinyToolkit::String::AsLocalString(timeWatcher.Max(), 6) << " us" << std::endl;
		std::cout << "min   : " << tinyToolkit::String::AsLocalString(timeWatcher.Min(), 6) << " us" << std::endl;
		std::cout << "all   : " << tinyToolkit::String::AsLocalString(timeWatcher.All(), 6) << " us" << std::endl;
		std::cout << "avg   : " << tinyToolkit::String::AsLocalString(timeWatcher.Avg(), 6) << " us" << std::endl;
		std::cout << "rate  : " << tinyToolkit::String::AsLocalString(timeWatcher.Rate(), 6) << " / us" << std::endl;
		std::cout << "count : " << tinyToolkit::String::AsLocalString(timeWatcher.Count()) << std::endl;
	}

	static void TestAsync(const char * description, const std::size_t count, const std::size_t threadCount)
	{
		std::cout << description << "..." << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////////////

		std::vector<std::size_t> result;
		std::vector<std::thread> threads;

		std::atomic<std::size_t> counter{ count };

		std::map<std::size_t, std::vector<std::time_t>> threadsResult;

		for (std::size_t i = 0; i < threadCount; ++i)
		{
			threads.emplace_back
			(
				std::thread
				(
					[&](std::vector<std::time_t> & res)
					{
						while (true)
						{
							std::size_t id = ++counter;

							if (id >= 2 * count)
							{
								break;
							}

							auto start_time = tinyToolkit::Time::TimePoint();

							TINY_TOOLKIT_ASYNC_LOG_INFO("Hello logger : this is async message, you can fast write")

							auto stop_time = tinyToolkit::Time::TimePoint();

							res.emplace_back(tinyToolkit::Time::Microseconds(stop_time - start_time));
						}
					}, std::ref(threadsResult[i])
				)
			);
		}

		for (auto &t : threads)
		{
			t.join();
		};

		sAsyncLogger.Wait();

		for (auto &iter : threadsResult)
		{
			result.insert(result.end(), iter.second.begin(), iter.second.end());
		}

		auto all = accumulate(begin(result), end(result), 0.0, std::plus<>());
		auto max = *std::max_element(result.begin(), result.end());
		auto min = *std::min_element(result.begin(), result.end());

		////////////////////////////////////////////////////////////////////////////////////////////////////

		std::cout << "max   : " << tinyToolkit::String::AsLocalString(max, 6) << " us" << std::endl;
		std::cout << "min   : " << tinyToolkit::String::AsLocalString(min, 6) << " us" << std::endl;
		std::cout << "all   : " << tinyToolkit::String::AsLocalString(all, 6) << " us" << std::endl;
		std::cout << "avg   : " << tinyToolkit::String::AsLocalString(all / count, 6) << " us" << std::endl;
		std::cout << "rate  : " << tinyToolkit::String::AsLocalString(count / all, 6) << " / us" << std::endl;
		std::cout << "count : " << tinyToolkit::String::AsLocalString(count, 6) << std::endl;
	}
};


#endif // __EXAMPLE__TEST__LOG__H__
