/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


/**
 *
 * 异步日志管理
 *
 */
#define sAsyncLogger		tinyToolkit::AsyncLogger::Instance()

/**
 *
 * 选项管理
 *
 */
#define OptionManager		tinyToolkit::OptionManager::Instance()


/**
 *
 * 解析参数
 *
 * @param argc 参数个数
 * @param argv 参数数组
 *
 */
void ParseOption(int argc, char const * argv[])
{
	OptionManager.DefineArg("type", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	OptionManager.DefineArg("mode", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	OptionManager.DefineArg("count", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	OptionManager.DefineArg("localHost", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	OptionManager.DefineArg("localPort", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	OptionManager.DefineArg("remoteHost", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	OptionManager.DefineArg("remotePort", nullptr, nullptr, tinyToolkit::Application::Name().c_str());

	OptionManager.DefineVersion(tinyToolkit::Application::CompileTimeString());

	OptionManager.Parse(argc, argv);
}


/**
 *
 * 开启日志
 *
 */
void StartLog()
{
	sAsyncLogger.AddSink<tinyToolkit::ConsoleLogSink>("console");
	sAsyncLogger.SetSinkLayout<tinyToolkit::SimpleLogLayout>();
	sAsyncLogger.EnableSinkAutoFlush();
}


/**
 *
 * 关闭日志
 *
 */
void CloseLog()
{
	sAsyncLogger.Wait();

	sAsyncLogger.CloseSink();
}


void StartApp()
{
	StartLog();

	TINY_TOOLKIT_ASYNC_LOG_INFO("Start App : {}", 0);

	Logic::Launch();
}


void CloseApp(int32_t signalNo = 0)
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("Close App : {}", signalNo);

	CloseLog();
}


void KillApp(int32_t signalNo = 0)
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("Kill App : {}", signalNo);

	CloseApp(signalNo);

	exit(0);
}


int main(int argc, char const * argv[])
{
	ParseOption(argc, argv);

	tinyToolkit::Signal::RegisterIgnore();
	tinyToolkit::Signal::RegisterTerminate(KillApp);
	tinyToolkit::Signal::RegisterStackTrace(tinyToolkit::Backtrace::Print);

	StartApp();
	CloseApp();

	return 0;
}
