#ifndef __TINY_TOOLKIT__LOG__SYSLOG_SINK__H__
#define __TINY_TOOLKIT__LOG__SYSLOG_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志系统节点
 *
 */


#if TINY_TOOLKIT_PLATFORM != TINY_TOOLKIT_PLATFORM_WINDOWS


#include "sink.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API SyslogLogSink : public ILogSink
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param name 节点名称
		 * @param idents 身份识别
		 * @param facility 消息程序类型
		 *
		 */
		explicit SyslogLogSink(std::string name, const char * idents = nullptr, int32_t facility = LOG_USER);

		/**
		 *
		 * 析构函数
		 *
		 */
		~SyslogLogSink() override;

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
	};
}


#endif // TINY_TOOLKIT_PLATFORM != TINY_TOOLKIT_PLATFORM_WINDOWS


#endif // __TINY_TOOLKIT__LOG__SYSLOG_SINK__H__
