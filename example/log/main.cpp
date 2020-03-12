/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"

#include "util/time.h"


static void SyncLog()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		auto time = util::Time::Seconds();

		auto fileSink = std::make_shared<log::FileSink>("fileSink", "syncFileSink.log", true);
		auto syslogSink = std::make_shared<log::SyslogSink>("syslogSink");
		auto consoleSink = std::make_shared<log::ConsoleSink>("consoleFile");
		auto dailyFileSink = std::make_shared<log::DailyFileSink>("dailyFileSink", "syncDailyFileSink.log");
		auto rotatingFileLogSink = std::make_shared<log::RotatingFileSink>("rotatingFileSink", "syncRotatingFileSink.log", 1024, 10);

		auto simpleLayout = std::make_shared<log::SimpleLayout>();
		auto patternLayout = std::make_shared<log::PatternLayout>("[%c][%J][%L][%i][%K] %v%n");

		auto timeFilter = std::make_shared<log::TimeFilter>(time + 1);
		auto regexFilter = std::make_shared<log::RegexFilter>(".*Critical.*");
		auto priorityFilter = std::make_shared<log::PriorityFilter>("DEBUG");
		auto timeRangeFilter = std::make_shared<log::TimeRangeFilter>(time, time + 3);
		auto priorityRangeFilter = std::make_shared<log::PriorityRangeFilter>("INFO", "ERROR");

		fileSink->SetLayout(simpleLayout)->AddFilter(regexFilter);
		syslogSink->SetLayout(simpleLayout)->AddFilter(priorityRangeFilter);
		consoleSink->SetLayout(patternLayout)->AddFilter(priorityFilter);
		dailyFileSink->SetLayout(patternLayout)->AddFilter(timeFilter);
		rotatingFileLogSink->SetLayout(simpleLayout)->AddFilter(timeRangeFilter);

		std::shared_ptr<log::ILogger> logger = std::make_shared<log::SyncLogger>();

		logger->AddSink(fileSink);
		logger->AddSink(syslogSink);
		logger->AddSink(consoleSink);
		logger->AddSink(dailyFileSink);
		logger->AddSink(rotatingFileLogSink);

		for (int i = 0; i < 10; ++i)
		{
			logger->Debug("Message {}", "Debug");
			logger->Info("Message {}", "Info");
			logger->Notice("Message {}", "Notice");
			logger->Warning("Message {}", "Warning");
			logger->Error("Message {}", "Error");
			logger->Critical("Message {}", "Critical");
			logger->Alert("Message {}", "Alert");
			logger->Fatal("Message {}", "Fatal");
			logger->Emerg("Message {}", "Emerg");

			TINY_TOOLKIT_LOG_DEBUG(logger, "Message {}", "Debug");
			TINY_TOOLKIT_LOG_INFO(logger, "Message {}", "Info");
			TINY_TOOLKIT_LOG_NOTICE(logger, "Message {}", "Notice");
			TINY_TOOLKIT_LOG_WARNING(logger, "Message {}", "Warning");
			TINY_TOOLKIT_LOG_ERROR(logger, "Message {}", "Error");
			TINY_TOOLKIT_LOG_CRITICAL(logger, "Message {}", "Critical");
			TINY_TOOLKIT_LOG_ALERT(logger, "Message {}", "Alert");
			TINY_TOOLKIT_LOG_FATAL(logger, "Message {}", "Fatal");
			TINY_TOOLKIT_LOG_EMERG(logger, "Message {}", "Emerg");
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
		auto time = util::Time::Seconds();

		auto fileSink = std::make_shared<log::FileSink>("fileSink", "asyncFileSink.log", true);
		auto syslogSink = std::make_shared<log::SyslogSink>("syslogSink");
		auto consoleSink = std::make_shared<log::ConsoleSink>("consoleFile");
		auto dailyFileSink = std::make_shared<log::DailyFileSink>("dailyFileSink", "asyncDailyFileSink.log");
		auto rotatingFileLogSink = std::make_shared<log::RotatingFileSink>("rotatingFileSink", "asyncRotatingFileSink.log", 1024, 10);

		auto simpleLayout = std::make_shared<log::SimpleLayout>();
		auto patternLayout = std::make_shared<log::PatternLayout>("[%c][%J][%L][%i][%K] %v%n");

		auto timeFilter = std::make_shared<log::TimeFilter>(time + 2);
		auto regexFilter = std::make_shared<log::RegexFilter>(".*Critical.*");
		auto priorityFilter = std::make_shared<log::PriorityFilter>("DEBUG");
		auto timeRangeFilter = std::make_shared<log::TimeRangeFilter>(time, time + 3);
		auto priorityRangeFilter = std::make_shared<log::PriorityRangeFilter>("INFO", "ERROR");

		fileSink->SetLayout(simpleLayout)->AddFilter(regexFilter);
		syslogSink->SetLayout(simpleLayout)->AddFilter(priorityRangeFilter);
		consoleSink->SetLayout(patternLayout)->AddFilter(priorityFilter);
		dailyFileSink->SetLayout(patternLayout)->AddFilter(timeFilter);
		rotatingFileLogSink->SetLayout(simpleLayout)->AddFilter(timeRangeFilter);

		std::shared_ptr<log::ILogger> logger = std::make_shared<log::AsyncLogger>();

		logger->AddSink(fileSink);
		logger->AddSink(syslogSink);
		logger->AddSink(consoleSink);
		logger->AddSink(dailyFileSink);
		logger->AddSink(rotatingFileLogSink);

		for (int i = 0; i < 10; ++i)
		{
			logger->Debug("Message {}", "Debug");
			logger->Info("Message {}", "Info");
			logger->Notice("Message {}", "Notice");
			logger->Warning("Message {}", "Warning");
			logger->Error("Message {}", "Error");
			logger->Critical("Message {}", "Critical");
			logger->Alert("Message {}", "Alert");
			logger->Fatal("Message {}", "Fatal");
			logger->Emerg("Message {}", "Emerg");

			TINY_TOOLKIT_LOG_DEBUG(logger, "Message {}", "Debug");
			TINY_TOOLKIT_LOG_INFO(logger, "Message {}", "Info");
			TINY_TOOLKIT_LOG_NOTICE(logger, "Message {}", "Notice");
			TINY_TOOLKIT_LOG_WARNING(logger, "Message {}", "Warning");
			TINY_TOOLKIT_LOG_ERROR(logger, "Message {}", "Error");
			TINY_TOOLKIT_LOG_CRITICAL(logger, "Message {}", "Critical");
			TINY_TOOLKIT_LOG_ALERT(logger, "Message {}", "Alert");
			TINY_TOOLKIT_LOG_FATAL(logger, "Message {}", "Fatal");
			TINY_TOOLKIT_LOG_EMERG(logger, "Message {}", "Emerg");
		}
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
