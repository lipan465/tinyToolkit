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
		std::shared_ptr<std::stringstream> stream = std::make_shared<std::stringstream>();

		std::shared_ptr<tinyToolkit::ILogger> logger = std::make_shared<tinyToolkit::SyncLogger>();

		auto fileSink = std::make_shared<tinyToolkit::FileLogSink>("syncFileSink", "syncFileSink.txt", true);
		auto dailyFileSink = std::make_shared<tinyToolkit::DailyFileLogSink>("syncDailyFileLogSink", "syncDailyFileLogSink.txt");
		auto rotatingFileLogSink = std::make_shared<tinyToolkit::RotatingFileLogSink>("syncRotatingFileLogSink", "syncRotatingFileLogSink.txt");
		auto consoleSink = std::make_shared<tinyToolkit::ConsoleLogSink>("syncConsoleFile");
		auto stringQueueSink = std::make_shared<tinyToolkit::StringQueueLogSink>("syncStringQueueSink");
		auto stringStreamSink = std::make_shared<tinyToolkit::OStreamLogSink>("syncOStreamLogSink", stream);

		logger->AddSink(fileSink);
		logger->AddSink(dailyFileSink);
		logger->AddSink(rotatingFileLogSink);
		logger->AddSink(consoleSink);
		logger->AddSink(stringQueueSink);
		logger->AddSink(stringStreamSink);

		logger->SetSinkLayout<tinyToolkit::SimpleLogLayout>();
		logger->EnableSinkAutoFlush();

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

		logger->Wait();

		std::cout << stream->str();

		while (!stringQueueSink->Queue().empty())
		{
			std::cout << stringQueueSink->Queue().front();

			stringQueueSink->Queue().pop();
		}
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
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
		std::shared_ptr<std::stringstream> stream = std::make_shared<std::stringstream>();

		std::shared_ptr<tinyToolkit::ILogger> logger = std::make_shared<tinyToolkit::AsyncLogger>();

		auto fileSink = std::make_shared<tinyToolkit::FileLogSink>("asyncFileSink", "asyncFileSink.txt", true);
		auto dailyFileSink = std::make_shared<tinyToolkit::DailyFileLogSink>("asyncDailyFileLogSink", "asyncDailyFileLogSink.txt");
		auto rotatingFileLogSink = std::make_shared<tinyToolkit::RotatingFileLogSink>("asyncRotatingFileLogSink", "asyncRotatingFileLogSink.txt");
		auto consoleSink = std::make_shared<tinyToolkit::ConsoleLogSink>("asyncConsoleFile");
		auto stringQueueSink = std::make_shared<tinyToolkit::StringQueueLogSink>("asyncStringQueueSink");
		auto stringStreamSink = std::make_shared<tinyToolkit::OStreamLogSink>("asyncOStreamLogSink", stream);

		logger->AddSink(fileSink);
		logger->AddSink(dailyFileSink);
		logger->AddSink(rotatingFileLogSink);
		logger->AddSink(consoleSink);
		logger->AddSink(stringQueueSink);
		logger->AddSink(stringStreamSink);

		logger->SetSinkLayout<tinyToolkit::SimpleLogLayout>();
		logger->EnableSinkAutoFlush();

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

		logger->Wait();

		std::cout << stream->str();

		while (!stringQueueSink->Queue().empty())
		{
			std::cout << stringQueueSink->Queue().front();

			stringQueueSink->Queue().pop();
		}
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
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
