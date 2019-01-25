/**
 *
 *  作者: hm
 *
 *  说明: 日志控制台节点
 *
 */


#include "consoleSink.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param name 节点名称
	 * @param file 控制台对象
	 *
	 */
	ConsoleLogSink::ConsoleLogSink(std::string name, FILE * console) : ILogSink(std::move(name)),
																	   _console(console)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	ConsoleLogSink::~ConsoleLogSink()
	{
		Close();
	}

	/**
	 *
	 * 关闭日志
	 *
	 */
	void ConsoleLogSink::Close()
	{
		Flush();
	}

	/**
	 *
	 * 刷新日志
	 *
	 */
	void ConsoleLogSink::Flush()
	{
		if (_console)
		{
			fflush(_console);
		}
	}

	/**
	 *
	 * 重新打开日志
	 *
	 */
	void ConsoleLogSink::Reopen()
	{

	}

	/**
	 *
	 * 写入日志
	 *
	 * @param event 日志事件
	 *
	 */
	void ConsoleLogSink::Write(const LogEvent & event)
	{
		if (Filter() && Filter()->Decide(event))
		{
			return;
		}

		if (_console)
		{
			std::string value(Layout() ? Layout()->Format(event) : event.message);

			::fwrite(value.c_str(), sizeof(char), value.size(), _console);
		}

		if (_autoFlush)
		{
			Flush();
		}
	}
}
