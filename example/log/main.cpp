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
		}

		log::SyncLogger::Instance().AddSink(fileSink);
		log::SyncLogger::Instance().AddSink(syslogSink);
		log::SyncLogger::Instance().AddSink(consoleSink);
		log::SyncLogger::Instance().AddSink(dailyFileSink);
		log::SyncLogger::Instance().AddSink(rotatingFileLogSink);

		for (int i = 0; i < 10; ++i)
		{
			TINY_TOOLKIT_SYNC_LOG_DEBUG("Message {}", "Debug");
			TINY_TOOLKIT_SYNC_LOG_INFO("Message {}", "Info");
			TINY_TOOLKIT_SYNC_LOG_NOTICE("Message {}", "Notice");
			TINY_TOOLKIT_SYNC_LOG_WARNING("Message {}", "Warning");
			TINY_TOOLKIT_SYNC_LOG_ERROR("Message {}", "Error");
			TINY_TOOLKIT_SYNC_LOG_CRITICAL("Message {}", "Critical");
			TINY_TOOLKIT_SYNC_LOG_ALERT("Message {}", "Alert");
			TINY_TOOLKIT_SYNC_LOG_FATAL("Message {}", "Fatal");
			TINY_TOOLKIT_SYNC_LOG_EMERG("Message {}", "Emerg");
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
		}

		logger->Wait();

		log::AsyncLogger::Instance().AddSink(fileSink);
		log::AsyncLogger::Instance().AddSink(syslogSink);
		log::AsyncLogger::Instance().AddSink(consoleSink);
		log::AsyncLogger::Instance().AddSink(dailyFileSink);
		log::AsyncLogger::Instance().AddSink(rotatingFileLogSink);

		for (int i = 0; i < 10; ++i)
		{
			TINY_TOOLKIT_ASYNC_LOG_DEBUG("Message {}", "Debug");
			TINY_TOOLKIT_ASYNC_LOG_INFO("Message {}", "Info");
			TINY_TOOLKIT_ASYNC_LOG_NOTICE("Message {}", "Notice");
			TINY_TOOLKIT_ASYNC_LOG_WARNING("Message {}", "Warning");
			TINY_TOOLKIT_ASYNC_LOG_ERROR("Message {}", "Error");
			TINY_TOOLKIT_ASYNC_LOG_CRITICAL("Message {}", "Critical");
			TINY_TOOLKIT_ASYNC_LOG_ALERT("Message {}", "Alert");
			TINY_TOOLKIT_ASYNC_LOG_FATAL("Message {}", "Fatal");
			TINY_TOOLKIT_ASYNC_LOG_EMERG("Message {}", "Emerg");
		}

		log::AsyncLogger::Instance().Wait();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Configurator()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::string content = "#\n"
		                      "# 文件节点\n"
		                      "#\n"
		                      "sink.file.type=FileSink\n"
		                      "sink.file.path=fileSink.log\n"
		                      "sink.file.truncate=false\n"
		                      "sink.file.layout=simple\n"
		                      "sink.file.filter=regex\n"
		                      "\n"
		                      "#\n"
		                      "# 系统节点\n"
		                      "#\n"
		                      "sink.syslog.type=SyslogSink\n"
		                      "sink.syslog.layout=simple\n"
		                      "sink.syslog.filter=priorityRange\n"
		                      "\n"
		                      "#\n"
		                      "# 控制台节点\n"
		                      "#\n"
		                      "sink.console.type=ConsoleSink\n"
		                      "sink.console.layout=pattern\n"
		                      "sink.console.filter=priority\n"
		                      "\n"
		                      "#\n"
		                      "# 时间文件节点\n"
		                      "#\n"
		                      "sink.dailyFile.type=DailyFileSink\n"
		                      "sink.dailyFile.path=dailyFileSink.log\n"
		                      "sink.dailyFile.hour=0\n"
		                      "sink.dailyFile.minute=0\n"
		                      "sink.dailyFile.second=0\n"
		                      "sink.dailyFile.layout=pattern\n"
		                      "sink.dailyFile.filter=priority\n"
		                      "\n"
		                      "#\n"
		                      "# 旋转文件节点\n"
		                      "#\n"
		                      "sink.rotatingFile.type=RotatingFileSink\n"
		                      "sink.rotatingFile.path=rotatingFileSink.log\n"
		                      "sink.rotatingFile.size=1024\n"
		                      "sink.rotatingFile.count=5\n"
		                      "sink.rotatingFile.layout=simple\n"
		                      "sink.rotatingFile.filter=priorityRange\n"
		                      "\n"
		                      "#\n"
		                      "# 简单布局\n"
		                      "#\n"
		                      "layout.simple.type=SimpleLayout\n"
		                      "\n"
		                      "#\n"
		                      "# 模式布局\n"
		                      "#\n"
		                      "layout.pattern.type=PatternLayout\n"
		                      "layout.pattern.rule=[%c][%J][%L][%i][%K] %v%n\n"
		                      "\n"
		                      "#\n"
		                      "# 正则过滤器\n"
		                      "#\n"
		                      "filter.regex.type=RegexFilter\n"
		                      "filter.regex.rule=.*Critical.*\n"
		                      "\n"
		                      "#\n"
		                      "# 优先级过滤器\n"
		                      "#\n"
		                      "filter.priority.type=PriorityFilter\n"
		                      "filter.priority.rule=DEBUG\n"
		                      "\n"
		                      "#\n"
		                      "# 优先级范围过滤器\n"
		                      "#\n"
		                      "filter.priorityRange.type=PriorityRangeFilter\n"
		                      "filter.priorityRange.rule=INFO, ERROR\n"
		                      "\n"
		                      "#\n"
		                      "# 日志对象\n"
		                      "#\n"
		                      "logger.async.type=AsyncLogger\n"
		                      "logger.async.sink=file, syslog, console, dailyFile, rotatingFile";

		auto configurator = log::Configurator::Instance();

		configurator.ParseContent(content);

		for (auto &logger : configurator.LoggerGroup())
		{
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
			}

			logger->Wait();
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
	Configurator();

	return 0;
}
