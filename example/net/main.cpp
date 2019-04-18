/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


#define sOption	tinyToolkit::OptionManager::Instance()


void ParseOption(int argc, char const * argv[])
{
	sOption.DefineArg("type", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("mode", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("count", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("localHost", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("localPort", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("remoteHost", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("remotePort", nullptr, nullptr, tinyToolkit::Application::Name().c_str());

	sOption.DefineVersion(tinyToolkit::Application::CompileTimeString());

	sOption.Parse(argc, argv);
}


void StartApp()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("Start App : {}", 0);

	Logic::Launch();
}


void CloseApp(int32_t signalNo = 0)
{
	TINY_TOOLKIT_SYNC_LOG_INFO("Close App : {}", signalNo);
}


void KillApp(int32_t signalNo = 0)
{
	TINY_TOOLKIT_SYNC_LOG_INFO("Kill App : {}", signalNo);

	CloseApp(signalNo);

	exit(0);
}


int main(int argc, char const * argv[])
{
	ParseOption(argc, argv);

	tinyToolkit::Signal::RegisterIgnore();
	tinyToolkit::Signal::RegisterTerminate(KillApp);
	tinyToolkit::Signal::RegisterStackTrace(tinyToolkit::Backtrace::Print);

	tinyToolkit::SyncLogger::Instance().AddSink<tinyToolkit::ConsoleLogSink>("console");
	tinyToolkit::SyncLogger::Instance().SetSinkLayout<tinyToolkit::SimpleLogLayout>();
	tinyToolkit::SyncLogger::Instance().EnableSinkAutoFlush();

	StartApp();
	CloseApp();

	return 0;
}
