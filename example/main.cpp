/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


using namespace tinyToolkit;


static void TestSync(const char * description, const std::shared_ptr<ILogger> & logger, const std::size_t count)
{
	std::cout << description << "..." << std::endl;

	////////////////////////////////////////////////////////////////////////////////////////////////////

	TimeWatcher timeWatcher;

	for (std::size_t i = 0; i < count; ++i)
	{
		timeWatcher.Start();

		logger->Info("Hello logger: msg number [thread={} id={}]", OS::ThreadID(), i);

		timeWatcher.Stop();
	}

	std::cout << "max   : " << String::AsLocalString(timeWatcher.Max()) << " us" << std::endl;
	std::cout << "min   : " << String::AsLocalString(timeWatcher.Min()) << " us" << std::endl;
	std::cout << "all   : " << String::AsLocalString(timeWatcher.All()) << " us" << std::endl;
	std::cout << "avg   : " << String::AsLocalString(timeWatcher.All() / static_cast<std::double_t>(timeWatcher.Count())) << " us" << std::endl;
	std::cout << "rate  : " << String::AsLocalString(timeWatcher.Count() / (static_cast<std::double_t>(timeWatcher.All()) / 1000000), 6) << "/sec" << std::endl;
	std::cout << "count : " << String::AsLocalString(timeWatcher.Count()) << std::endl;
}

static void TestAsync(const char * description, const std::shared_ptr<ILogger> & logger, const std::size_t count, const std::size_t threadCount)
{
	std::cout << description << "..." << std::endl;

	////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<std::size_t> result;
	std::vector<std::thread> threads;

	std::atomic<std::size_t> counter{ count };

	std::map<std::size_t, std::vector<std::time_t>> threadsResult;

	auto start = Time::TimePoint();

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

						auto start_time = Time::TimePoint();

						logger->Info("Hello logger: msg number [thread={} id={}]", OS::ThreadID(), id);

						auto stop_time = Time::TimePoint();

						res.emplace_back(Time::Microseconds(stop_time - start_time));
					}
				}, std::ref(threadsResult[i])
			)
		);
	}

	for (auto &t : threads)
	{
		t.join();
	};

	logger->Wait();

	auto stop = Time::TimePoint();

	for (auto &iter : threadsResult)
	{
		result.insert(result.end(), iter.second.begin(), iter.second.end());
	}

	auto all = Time::Microseconds(stop - start);
	auto max = *std::max_element(result.begin(), result.end());
	auto min = *std::min_element(result.begin(), result.end());
	auto avg = accumulate(begin(result), end(result), 0.0, std::plus<>()) / result.size();

	////////////////////////////////////////////////////////////////////////////////////////////////////

	std::cout << "max  : " << String::AsLocalString(max) << " us" << std::endl;
	std::cout << "min  : " << String::AsLocalString(min) << " us" << std::endl;
	std::cout << "avg  : " << String::AsLocalString(avg) << " us" << std::endl;
	std::cout << "all  : " << String::AsLocalString(all) << " us" << std::endl;
	std::cout << "rate : " << String::AsLocalString(count / Time::Seconds(stop - start)) << "/sec" << std::endl;
}

int main(int argc, const char * argv[])
{
	auto syncLogger = std::make_shared<SyncLogger>();
	auto asyncLogger = std::make_shared<AsyncLogger>();

	syncLogger->AddSink<FileLogSink>("syncFile", "syncFile.txt");
	asyncLogger->AddSink<FileLogSink>("asyncFile", "asyncFile.txt");

//	syncLogger->AddSink<DailyFileLogSink>("daily", "daily.txt");
//	asyncLogger->AddSink<DailyFileLogSink>("daily", "daily.txt");

//	syncLogger->AddSink<RotatingLogFileSink>("rotating", "rotating.txt", 100, 1);
//	asyncLogger->AddSink<RotatingLogFileSink>("rotating", "rotating.txt", 100, 1);

//	syncLogger->AddSink<SyslogLogSink>("syslog");
//	asyncLogger->AddSink<SyslogLogSink>("syslog");

//	syncLogger->AddSink<ConsoleLogSink>("console");
//	asyncLogger->AddSink<ConsoleLogSink>("console");

//	syncLogger->AddSink<OStreamLogSink>("oStream", std::make_shared<std::ostringstream>());
//	asyncLogger->AddSink<OStreamLogSink>("oStream", std::make_shared<std::ostringstream>());

//	syncLogger->AddSink<StringQueueLogSink>("stringQueue");
//	asyncLogger->AddSink<StringQueueLogSink>("stringQueue");

	syncLogger->SetSinkLayout(std::make_shared<SimpleLogLayout>());
	asyncLogger->SetSinkLayout(std::make_shared<SimpleLogLayout>());

//	syncLogger->SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::CRITICAL, LOG_PRIORITY_TYPE::FATAL));
//	asyncLogger->SetSinkFilter(std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::CRITICAL, LOG_PRIORITY_TYPE::FATAL));

	TestSync("sync", syncLogger, 1000000);
	TestAsync("async", asyncLogger, 1000000, 2);

//	ID::Test();
//	Pool::Test();
//	Debug::Test();
//	System::Test();
//	Crypto::Test();
//	Utilities::Test();

//	getchar();

	return 0;
}

