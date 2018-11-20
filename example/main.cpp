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
	auto stream = std::make_shared<std::ostringstream>();
	auto layout = std::make_shared<PatternLogLayout>("[%c][%J][%L][%K][%i]%v%%%n");
	auto filter = std::make_shared<PriorityRangeLogFilter>(LOG_PRIORITY_TYPE::CRITICAL, LOG_PRIORITY_TYPE::FATAL);

	SyncLogger syncLogger;

	syncLogger.AddSink<FileLogSink>("file", "file.txt");
	syncLogger.AddSink<DailyFileLogSink>("daily", "daily.txt");
	syncLogger.AddSink<RotatingLogFileSink>("rotating", "rotating.txt", 100, 1);
	syncLogger.AddSink<SyslogLogSink>("syslog");
	syncLogger.AddSink<ConsoleLogSink>("console");
	syncLogger.AddSink<OStreamLogSink>("oStream", stream);
	syncLogger.AddSink<StringQueueLogSink>("stringQueue");

	syncLogger.Debug("debug message");
	syncLogger.Info("info message");
	syncLogger.Notice("notice message");
	syncLogger.Warning("warning message");
	syncLogger.Error("error message");
	syncLogger.Critical("critical message");
	syncLogger.Alert("alert message");
	syncLogger.Fatal("fatal message");
	syncLogger.Emerg("emerg message");

//	ID::Test();
//	Pool::Test();
//	Debug::Test();
//	System::Test();
//	Crypto::Test();
//	Utilities::Test();

//	getchar();

	return 0;
}

