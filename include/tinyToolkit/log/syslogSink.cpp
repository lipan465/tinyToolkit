/**
 *
 *  作者: hm
 *
 *  说明: 日志系统节点
 *
 */


#include "syslogSink.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param name 节点名称
	 * @param idents 身份识别
	 * @param facility 消息程序类型
	 *
	 */
	SyslogLogSink::SyslogLogSink(std::string name, const char * idents, int32_t facility) : ILogSink(std::move(name))
	{
		::openlog(idents, LOG_CONS, facility);
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	SyslogLogSink::~SyslogLogSink()
	{
		Close();
	}

	/**
	 *
	 * 关闭日志
	 *
	 */
	void SyslogLogSink::Close()
	{
		Flush();

		::closelog();
	}

	/**
	 *
	 * 刷新日志
	 *
	 */
	void SyslogLogSink::Flush()
	{

	}

	/**
	 *
	 * 重新打开日志
	 *
	 */
	void SyslogLogSink::Reopen()
	{

	}

	/**
	 *
	 * 写入日志
	 *
	 * @param event 日志事件
	 *
	 */
	void SyslogLogSink::Write(const LogEvent & event)
	{
		if (Filter() && Filter()->Decide(event))
		{
			return;
		}

		static std::unordered_map<LOG_PRIORITY_TYPE, int32_t> priorities
		{
			{ LOG_PRIORITY_TYPE::DEBUGS, LOG_DEBUG },
			{ LOG_PRIORITY_TYPE::INFO, LOG_INFO },
			{ LOG_PRIORITY_TYPE::NOTICE, LOG_NOTICE },
			{ LOG_PRIORITY_TYPE::WARNING, LOG_WARNING },
			{ LOG_PRIORITY_TYPE::ERRORS, LOG_ERR },
			{ LOG_PRIORITY_TYPE::CRITICAL, LOG_CRIT },
			{ LOG_PRIORITY_TYPE::ALERT, LOG_ALERT },
			{ LOG_PRIORITY_TYPE::FATAL, LOG_ALERT },
			{ LOG_PRIORITY_TYPE::EMERG, LOG_EMERG },
		};

		::syslog(priorities[event.priority], "%s", Layout() ? Layout()->Format(event).c_str() : event.message.c_str());

		if (_autoFlush)
		{
			Flush();
		}
	}
}
