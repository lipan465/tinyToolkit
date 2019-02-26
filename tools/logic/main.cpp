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

void test1()
{
	std::cout << TINY_TOOLKIT_FUNC << std::endl;
}

void test2(int a)
{
	std::cout << TINY_TOOLKIT_FUNC << ", a : " << a << std::endl;
}

int32_t test3()
{
	std::cout << TINY_TOOLKIT_FUNC << std::endl;

	return 0;
}

int32_t test4(int a)
{
	std::cout << TINY_TOOLKIT_FUNC << ", a : " << a << std::endl;

	return 0;
}

#define TINY_TOOLKIT_ANONYMOUS(type)				type TINY_TOOLKIT_JOIN_VALUE(_anonymous, __LINE__)


void __deferCleanUp(std::function<void()> * func)
{
	(*func)();
}

#define defer TINY_TOOLKIT_ANONYMOUS(std::function<void()>) __attribute__((cleanup(__deferCleanUp), unused)) =

void StartApp()
{
	int i = 0;

	defer test1;
	defer [i](){ test2(i); };

	return;
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