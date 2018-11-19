#ifndef __TINY_TOOLKIT__LOG__SYSLOG_SINK__H__
#define __TINY_TOOLKIT__LOG__SYSLOG_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 系统日志节点
 *
 */


#include "sink.h"


namespace tinyToolkit
{
	#if TINY_TOOLKIT_PLATFORM != TINY_TOOLKIT_PLATFORM_WINDOWS

	class TINY_TOOLKIT_API SyslogSink : public ILogSink
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		SyslogSink()
		{
			::openlog(_idents, LOG_CONS | LOG_PID, _facility);
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param facility 消息程序类型
		 *
		 */
		explicit SyslogSink(int32_t facility) : _facility(facility)
		{
			::openlog(_idents, LOG_CONS | LOG_PID, _facility);
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param idents 身份识别
		 *
		 */
		explicit SyslogSink(const char * idents) : _idents(idents)
		{
			::openlog(_idents, LOG_CONS | LOG_PID, _facility);
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param idents 身份识别
		 * @param facility 消息程序类型
		 *
		 */
		explicit SyslogSink(const char * idents, int32_t facility) : _facility(facility), _idents(idents)
		{
			::openlog(_idents, LOG_CONS | LOG_PID, _facility);
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~SyslogSink() override
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

			::closelog();
		}

		/**
		 *
		 * 刷新日志
		 *
		 */
		void Flush() override
		{

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

			static std::unordered_map<LOG_PRIORITY_TYPE, int32_t> priorities
			{
				{ LOG_PRIORITY_TYPE::DEBUG, LOG_DEBUG },
				{ LOG_PRIORITY_TYPE::INFO, LOG_INFO },
				{ LOG_PRIORITY_TYPE::NOTICE, LOG_NOTICE },
				{ LOG_PRIORITY_TYPE::WARNING, LOG_WARNING },
				{ LOG_PRIORITY_TYPE::ERROR, LOG_ERR },
				{ LOG_PRIORITY_TYPE::CRITICAL, LOG_CRIT },
				{ LOG_PRIORITY_TYPE::ALERT, LOG_ALERT },
				{ LOG_PRIORITY_TYPE::FATAL, LOG_ALERT },
				{ LOG_PRIORITY_TYPE::EMERG, LOG_EMERG },
			};

			::syslog(priorities[event.priority] | _facility, "%s", Layout() ? Layout()->Format(event).data() : event.message.data());

			if (_autoFlush)
			{
				Flush();
			}
		}

	protected:
		int32_t _facility{ LOG_USER };

		const char * _idents{ nullptr };
	};

	#endif
}


#endif // __TINY_TOOLKIT__LOG__SYSLOG_SINK__H__
