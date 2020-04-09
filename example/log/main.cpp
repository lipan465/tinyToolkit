/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void SyncLog()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		auto logger = std::make_shared<log::SyncLogger>();

		auto fileSink = std::make_shared<log::FileSink>("fileSink", "syncFileSink.log", true);
		auto syslogSink = std::make_shared<log::SyslogSink>("syslogSink");
		auto consoleSink = std::make_shared<log::ConsoleSink>("consoleFile");
		auto dailyFileSink = std::make_shared<log::DailyFileSink>("dailyFileSink", "syncDailyFileSink.log");
		auto rotatingFileLogSink = std::make_shared<log::RotatingFileSink>("rotatingFileSink", "syncRotatingFileSink.log", 1024, 10);

		auto simpleLayout = std::make_shared<log::SimpleLayout>();
		auto patternLayout = std::make_shared<log::PatternLayout>("[%c][%J][%L][%i][%K] %v%n");

		auto regexFilter = std::make_shared<log::RegexFilter>(".*Critical.*");
		auto priorityFilter = std::make_shared<log::PriorityFilter>("DEBUG");
		auto priorityRangeFilter = std::make_shared<log::PriorityRangeFilter>("INFO", "ERROR");

		fileSink->SetLayout(simpleLayout)->AddFilter(regexFilter);
		syslogSink->SetLayout(simpleLayout)->AddFilter(priorityRangeFilter);
		consoleSink->SetLayout(patternLayout)->AddFilter(priorityFilter);
		dailyFileSink->SetLayout(patternLayout)->AddFilter(priorityFilter);
		rotatingFileLogSink->SetLayout(simpleLayout)->AddFilter(priorityRangeFilter);

		logger->AddSink(fileSink);
		logger->AddSink(syslogSink);
		logger->AddSink(consoleSink);
		logger->AddSink(dailyFileSink);
		logger->AddSink(rotatingFileLogSink);

		for (int i = 0; i < 10; ++i)
		{
			logger->Debug("Sync Message {}", "Debug");
			logger->Info("Sync Message {}", "Info");
			logger->Notice("Sync Message {}", "Notice");
			logger->Warning("Sync Message {}", "Warning");
			logger->Error("Sync Message {}", "Error");
			logger->Critical("Sync Message {}", "Critical");
			logger->Alert("Sync Message {}", "Alert");
			logger->Fatal("Sync Message {}", "Fatal");
			logger->Emerg("Sync Message {}", "Emerg");
		}
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void AsyncLog()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		auto logger = std::make_shared<log::AsyncLogger>();

		auto fileSink = std::make_shared<log::FileSink>("fileSink", "asyncFileSink.log", true);
		auto syslogSink = std::make_shared<log::SyslogSink>("syslogSink");
		auto consoleSink = std::make_shared<log::ConsoleSink>("consoleFile");
		auto dailyFileSink = std::make_shared<log::DailyFileSink>("dailyFileSink", "asyncDailyFileSink.log");
		auto rotatingFileLogSink = std::make_shared<log::RotatingFileSink>("rotatingFileSink", "asyncRotatingFileSink.log", 1024, 10);

		auto simpleLayout = std::make_shared<log::SimpleLayout>();
		auto patternLayout = std::make_shared<log::PatternLayout>("[%c][%J][%L][%i][%K] %v%n");

		auto regexFilter = std::make_shared<log::RegexFilter>(".*Critical.*");
		auto priorityFilter = std::make_shared<log::PriorityFilter>("DEBUG");
		auto priorityRangeFilter = std::make_shared<log::PriorityRangeFilter>("INFO", "ERROR");

		fileSink->SetLayout(simpleLayout)->AddFilter(regexFilter);
		syslogSink->SetLayout(simpleLayout)->AddFilter(priorityRangeFilter);
		consoleSink->SetLayout(patternLayout)->AddFilter(priorityFilter);
		dailyFileSink->SetLayout(patternLayout)->AddFilter(priorityFilter);
		rotatingFileLogSink->SetLayout(simpleLayout)->AddFilter(priorityRangeFilter);

		logger->AddSink(fileSink);
		logger->AddSink(syslogSink);
		logger->AddSink(consoleSink);
		logger->AddSink(dailyFileSink);
		logger->AddSink(rotatingFileLogSink);

		for (int i = 0; i < 10; ++i)
		{
			logger->Debug("Async Message {}", "Debug");
			logger->Info("Async Message {}", "Info");
			logger->Notice("Async Message {}", "Notice");
			logger->Warning("Async Message {}", "Warning");
			logger->Error("Async Message {}", "Error");
			logger->Critical("Async Message {}", "Critical");
			logger->Alert("Async Message {}", "Alert");
			logger->Fatal("Async Message {}", "Fatal");
			logger->Emerg("Async Message {}", "Emerg");
		}

		logger->Wait();
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

	SyncLog();
	AsyncLog();

	return 0;
}
