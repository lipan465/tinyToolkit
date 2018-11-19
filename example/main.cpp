/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


using namespace tinyToolkit;

int main(int argc, const char * argv[])
{
	tinyToolkit::LogEvent eventDebug;
	tinyToolkit::LogEvent eventInfo;
	tinyToolkit::LogEvent eventNotice;
	tinyToolkit::LogEvent eventWarning;
	tinyToolkit::LogEvent eventError;
	tinyToolkit::LogEvent eventCritical;
	tinyToolkit::LogEvent eventAlert;
	tinyToolkit::LogEvent eventFatal;
	tinyToolkit::LogEvent eventEmerg;

	eventDebug.name = "main";
	eventDebug.priority = tinyToolkit::LOG_PRIORITY_TYPE::DEBUG;
	eventDebug.message = "debug message";
	eventDebug.time = tinyToolkit::Time::TimePoint();

	eventInfo.name = "main";
	eventInfo.priority = tinyToolkit::LOG_PRIORITY_TYPE::INFO;
	eventInfo.message = "info message";
	eventInfo.time = tinyToolkit::Time::TimePoint();

	eventNotice.name = "main";
	eventNotice.priority = tinyToolkit::LOG_PRIORITY_TYPE::NOTICE;
	eventNotice.message = "notice message";
	eventNotice.time = tinyToolkit::Time::TimePoint();

	eventWarning.name = "main";
	eventWarning.priority = tinyToolkit::LOG_PRIORITY_TYPE::WARNING;
	eventWarning.message = "warning message";
	eventWarning.time = tinyToolkit::Time::TimePoint();

	eventError.name = "main";
	eventError.priority = tinyToolkit::LOG_PRIORITY_TYPE::ERROR;
	eventError.message = "error message";
	eventError.time = tinyToolkit::Time::TimePoint();

	eventCritical.name = "main";
	eventCritical.priority = tinyToolkit::LOG_PRIORITY_TYPE::CRITICAL;
	eventCritical.message = "critical message";
	eventCritical.time = tinyToolkit::Time::TimePoint();

	eventAlert.name = "main";
	eventAlert.priority = tinyToolkit::LOG_PRIORITY_TYPE::ALERT;
	eventAlert.message = "alert message";
	eventAlert.time = tinyToolkit::Time::TimePoint();

	eventFatal.name = "main";
	eventFatal.priority = tinyToolkit::LOG_PRIORITY_TYPE::FATAL;
	eventFatal.message = "fatal message";
	eventFatal.time = tinyToolkit::Time::TimePoint();

	eventEmerg.name = "main";
	eventEmerg.priority = tinyToolkit::LOG_PRIORITY_TYPE::EMERG;
	eventEmerg.message = "emerg message";
	eventEmerg.time = tinyToolkit::Time::TimePoint();

	FileSink fileSink("1.txt");
	DailyFileSink dailyFileSink("daily.txt", 17, 48, 0);
	RotatingFileSink rotatingFileSink("rotating.txt", 10, 1);
	SyslogSink syslogSink;
	ConsoleSink consoleSink;
	OStreamSink oStreamSink;
	StringQueueSink stringQueueSink;

	auto stream = std::make_shared<std::ostringstream>();
	auto filter = std::make_shared<LogPriorityRangeFilter>(LOG_PRIORITY_TYPE::CRITICAL, LOG_PRIORITY_TYPE::FATAL);
	auto layout = std::make_shared<LogPatternLayout>("[%c][%J][%L][%K][%i]%v%%%n");

	fileSink.SetFilter(filter);
	fileSink.SetLayout(layout);

	dailyFileSink.SetFilter(filter);
	dailyFileSink.SetLayout(layout);

	rotatingFileSink.SetFilter(filter);
	rotatingFileSink.SetLayout(layout);

	syslogSink.SetFilter(filter);
	syslogSink.SetLayout(layout);

	consoleSink.SetFilter(filter);
	consoleSink.SetLayout(layout);

	oStreamSink.SetFilter(filter);
	oStreamSink.SetLayout(layout);
	oStreamSink.SetStream(stream);

	stringQueueSink.SetFilter(filter);
	stringQueueSink.SetLayout(layout);

	fileSink.Write(eventDebug);
	fileSink.Write(eventInfo);
	fileSink.Write(eventNotice);
	fileSink.Write(eventWarning);
	fileSink.Write(eventError);
	fileSink.Write(eventCritical);
	fileSink.Write(eventAlert);
	fileSink.Write(eventFatal);
	fileSink.Write(eventEmerg);

	std::cout << "size1 : " << fileSink.Size() << std::endl;
	std::cout << "path1 : " << fileSink.Path() << std::endl;

//	fileSink.Reopen();

	std::cout << "size2 : " << fileSink.Size() << std::endl;
	std::cout << "path2 : " << fileSink.Path() << std::endl;

	dailyFileSink.Write(eventDebug);
	dailyFileSink.Write(eventInfo);
	dailyFileSink.Write(eventNotice);
	dailyFileSink.Write(eventWarning);
	dailyFileSink.Write(eventError);
	dailyFileSink.Write(eventCritical);
	dailyFileSink.Write(eventAlert);
	dailyFileSink.Write(eventFatal);
	dailyFileSink.Write(eventEmerg);

	std::cout << "size1 : " << dailyFileSink.Size() << std::endl;
	std::cout << "path1 : " << dailyFileSink.Path() << std::endl;

//	dailyFileSink.Reopen();

	std::cout << "size2 : " << dailyFileSink.Size() << std::endl;
	std::cout << "path2 : " << dailyFileSink.Path() << std::endl;

	rotatingFileSink.Write(eventDebug);
	rotatingFileSink.Write(eventInfo);
	rotatingFileSink.Write(eventNotice);
	rotatingFileSink.Write(eventWarning);
	rotatingFileSink.Write(eventError);
	rotatingFileSink.Write(eventCritical);
	rotatingFileSink.Write(eventAlert);
	rotatingFileSink.Write(eventFatal);
	rotatingFileSink.Write(eventEmerg);

	std::cout << "size1 : " << rotatingFileSink.Size() << std::endl;
	std::cout << "path1 : " << rotatingFileSink.Path() << std::endl;

//	rotatingFileSink.Reopen();

	std::cout << "size2 : " << rotatingFileSink.Size() << std::endl;
	std::cout << "path2 : " << rotatingFileSink.Path() << std::endl;

	syslogSink.Write(eventDebug);
	syslogSink.Write(eventInfo);
	syslogSink.Write(eventNotice);
	syslogSink.Write(eventWarning);
	syslogSink.Write(eventError);
	syslogSink.Write(eventCritical);
	syslogSink.Write(eventAlert);
	syslogSink.Write(eventFatal);
	syslogSink.Write(eventEmerg);

	consoleSink.Write(eventDebug);
	consoleSink.Write(eventInfo);
	consoleSink.Write(eventNotice);
	consoleSink.Write(eventWarning);
	consoleSink.Write(eventError);
	consoleSink.Write(eventCritical);
	consoleSink.Write(eventAlert);
	consoleSink.Write(eventFatal);
	consoleSink.Write(eventEmerg);

	oStreamSink.Write(eventDebug);
	oStreamSink.Write(eventInfo);
	oStreamSink.Write(eventNotice);
	oStreamSink.Write(eventWarning);
	oStreamSink.Write(eventError);
	oStreamSink.Write(eventCritical);
	oStreamSink.Write(eventAlert);
	oStreamSink.Write(eventFatal);
	oStreamSink.Write(eventEmerg);

	std::cout << "stream1 : " << std::endl;
	std::cout << stream->str() << std::endl;

	oStreamSink.Reopen();

	std::cout << "stream2 : " << std::endl;
	std::cout << stream->str() << std::endl;

	stringQueueSink.Write(eventDebug);
	stringQueueSink.Write(eventInfo);
	stringQueueSink.Write(eventNotice);
	stringQueueSink.Write(eventWarning);
	stringQueueSink.Write(eventError);
	stringQueueSink.Write(eventCritical);
	stringQueueSink.Write(eventAlert);
	stringQueueSink.Write(eventFatal);
	stringQueueSink.Write(eventEmerg);

	std::cout << "queue1 : " << std::endl;

	while (!stringQueueSink.Empty())
	{
		std::cout << stringQueueSink.Queue().front();

		stringQueueSink.Queue().pop();
	}

	stringQueueSink.Reopen();

	std::cout << "queue2 : " << std::endl;

	while (!stringQueueSink.Empty())
	{
		std::cout << stringQueueSink.Queue().front();

		stringQueueSink.Queue().pop();
	}

//	ID::Test();
//	Pool::Test();
//	Debug::Test();
//	System::Test();
//	Crypto::Test();
//	Utilities::Test();

//	getchar();

	return 0;
}

