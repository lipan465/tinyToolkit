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
	LogEvent eventDebug;
	LogEvent eventInfo;
	LogEvent eventNotice;
	LogEvent eventWarning;
	LogEvent eventError;
	LogEvent eventCritical;
	LogEvent eventAlert;
	LogEvent eventFatal;
	LogEvent eventEmerg;

	eventDebug.name = "main";
	eventDebug.priority = LOG_PRIORITY_TYPE::DEBUG;
	eventDebug.message = "debug message";
	eventDebug.time = tinyToolkit::Time::TimePoint();

	eventInfo.name = "main";
	eventInfo.priority = LOG_PRIORITY_TYPE::INFO;
	eventInfo.message = "info message";
	eventInfo.time = Time::TimePoint();

	eventNotice.name = "main";
	eventNotice.priority = LOG_PRIORITY_TYPE::NOTICE;
	eventNotice.message = "notice message";
	eventNotice.time = Time::TimePoint();

	eventWarning.name = "main";
	eventWarning.priority = LOG_PRIORITY_TYPE::WARNING;
	eventWarning.message = "warning message";
	eventWarning.time = Time::TimePoint();

	eventError.name = "main";
	eventError.priority = LOG_PRIORITY_TYPE::ERROR;
	eventError.message = "error message";
	eventError.time = Time::TimePoint();

	eventCritical.name = "main";
	eventCritical.priority = LOG_PRIORITY_TYPE::CRITICAL;
	eventCritical.message = "critical message";
	eventCritical.time = Time::TimePoint();

	eventAlert.name = "main";
	eventAlert.priority = LOG_PRIORITY_TYPE::ALERT;
	eventAlert.message = "alert message";
	eventAlert.time = Time::TimePoint();

	eventFatal.name = "main";
	eventFatal.priority = LOG_PRIORITY_TYPE::FATAL;
	eventFatal.message = "fatal message";
	eventFatal.time = Time::TimePoint();

	eventEmerg.name = "main";
	eventEmerg.priority = LOG_PRIORITY_TYPE::EMERG;
	eventEmerg.message = "emerg message";
	eventEmerg.time = Time::TimePoint();

	FileSink fileSink("file.txt");
	DailyFileSink dailyFileSink("daily.txt", 17, 48, 0);
	RotatingFileSink rotatingFileSink("rotating.txt", 170, 1);
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

	std::cout << "size : " << fileSink.Size() << std::endl;
	std::cout << "path : " << fileSink.Path() << std::endl;

	dailyFileSink.Write(eventDebug);
	dailyFileSink.Write(eventInfo);
	dailyFileSink.Write(eventNotice);
	dailyFileSink.Write(eventWarning);
	dailyFileSink.Write(eventError);
	dailyFileSink.Write(eventCritical);
	dailyFileSink.Write(eventAlert);
	dailyFileSink.Write(eventFatal);
	dailyFileSink.Write(eventEmerg);

	std::cout << "size : " << dailyFileSink.Size() << std::endl;
	std::cout << "path : " << dailyFileSink.Path() << std::endl;

	rotatingFileSink.Write(eventDebug);
	rotatingFileSink.Write(eventInfo);
	rotatingFileSink.Write(eventNotice);
	rotatingFileSink.Write(eventWarning);
	rotatingFileSink.Write(eventError);
	rotatingFileSink.Write(eventCritical);
	rotatingFileSink.Write(eventAlert);
	rotatingFileSink.Write(eventFatal);
	rotatingFileSink.Write(eventEmerg);

	std::cout << "size : " << rotatingFileSink.Size() << std::endl;
	std::cout << "path : " << rotatingFileSink.Path() << std::endl;

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

	std::cout << "stream : " << stream->str().size() << std::endl;
	std::cout << stream->str();

	stringQueueSink.Write(eventDebug);
	stringQueueSink.Write(eventInfo);
	stringQueueSink.Write(eventNotice);
	stringQueueSink.Write(eventWarning);
	stringQueueSink.Write(eventError);
	stringQueueSink.Write(eventCritical);
	stringQueueSink.Write(eventAlert);
	stringQueueSink.Write(eventFatal);
	stringQueueSink.Write(eventEmerg);

	std::cout << "queue : " << stringQueueSink.Size() << std::endl;

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

