#ifndef __TINY_TOOLKIT__LOG__CONSOLE_SINK__H__
#define __TINY_TOOLKIT__LOG__CONSOLE_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志控制台节点
 *
 */


#include "sink.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ConsoleLogSink : public ILogSink
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param name 节点名称
		 * @param file 控制台对象
		 *
		 */
		explicit ConsoleLogSink(std::string name, FILE * console = stdout) : ILogSink(std::move(name)),
																			 _console(console)
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~ConsoleLogSink() override
		{
			Close();
		}

		/**
		 *
		 * 关闭日志
		 *
		 */
		void Close() override
		{
			Flush();
		}

		/**
		 *
		 * 刷新日志
		 *
		 */
		void Flush() override
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
		void Reopen() override
		{

		}

		/**
		 *
		 * 写入日志
		 *
		 * @param event 日志事件
		 *
		 */
		void Write(const LogEvent & event) override
		{
			if (Filter() && Filter()->Decide(event))
			{
				return;
			}

			if (_console)
			{
				std::string value(Layout() ? Layout()->Format(event) : event.message);

				::fwrite(value.data(), sizeof(char), value.size(), _console);
			}

			if (_autoFlush)
			{
				Flush();
			}
		}

	protected:
		FILE * _console{ nullptr };
	};
}


#endif // __TINY_TOOLKIT__LOG__CONSOLE_SINK__H__
