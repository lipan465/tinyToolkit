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
		explicit ConsoleLogSink(std::string name, FILE * console = stdout);

		/**
		 *
		 * 析构函数
		 *
		 */
		~ConsoleLogSink() override;

		/**
		 *
		 * 关闭日志
		 *
		 */
		void Close() override;

		/**
		 *
		 * 刷新日志
		 *
		 */
		void Flush() override;

		/**
		 *
		 * 重新打开日志
		 *
		 */
		void Reopen() override;

		/**
		 *
		 * 写入日志
		 *
		 * @param event 日志事件
		 *
		 */
		void Write(const LogEvent & event) override;

	protected:
		FILE * _console{ nullptr };
	};
}


#endif // __TINY_TOOLKIT__LOG__CONSOLE_SINK__H__
