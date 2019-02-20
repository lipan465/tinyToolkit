/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


void ParseOption(int argc, char const * argv[])
{
	tinyToolkit::OptionManager::Instance().DefineArg("path", "logic module path", nullptr, tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("name", "logic module name", nullptr, tinyToolkit::Application::Name().c_str());

	tinyToolkit::OptionManager::Instance().DefineVersion(tinyToolkit::Application::CompileTimeString());

	tinyToolkit::OptionManager::Instance().Parse(argc, argv);
}


void StartApp()
{
	auto modulePath = tinyToolkit::OptionManager::Instance().Get("path");
	auto moduleName = tinyToolkit::OptionManager::Instance().Get("name");

	if (LogicFactory::Create(modulePath, moduleName))
	{
		tinyToolkit::String::Print("Create [{}] logic module [{}] template success{}", modulePath, moduleName, TINY_TOOLKIT_EOL);
	}
	else
	{
		tinyToolkit::String::Print("Create [{}] logic module [{}] template failed{}", modulePath, moduleName, TINY_TOOLKIT_EOL);
	}
}


int main(int argc, char const * argv[])
{
	ParseOption(argc, argv);

	tinyToolkit::Signal::RegisterIgnore();
	tinyToolkit::Signal::RegisterStackTrace(tinyToolkit::Backtrace::Print);

	StartApp();

	return 0;
}