/**
 *
 *  作者: hm
 *
 *  说明: 系统节点
 *
 */


#include "syslogSink.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <map>
#
#  include "../util/string.h"
#  include "../util/application.h"
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <map>
#
#  include <syslog.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <map>
#
#  include <syslog.h>
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 *
		 */
		SyslogSink::SyslogSink(std::string name) : ISink(std::move(name))
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			_handle = ::RegisterEventSource(nullptr, util::Application::Name().c_str());

		#else

			::openlog(nullptr, LOG_CONS, LOG_USER);

		#endif
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		SyslogSink::~SyslogSink()
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (_handle)
			{
				::DeregisterEventSource(_handle);
			}

		#else

			::closelog();

		#endif
		}

		/**
		 *
		 * 关闭节点
		 *
		 */
		void SyslogSink::Close()
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (_handle)
			{
				::DeregisterEventSource(_handle);
			}

		#else

			::closelog();

		#endif
		}

		/**
		 *
		 * 刷新节点
		 *
		 */
		void SyslogSink::Flush()
		{

		}

		/**
		 *
		 * 写入内容
		 *
		 * @param context 上下文
		 *
		 */
		void SyslogSink::Write(const Context & context)
		{
			if (Filter() && Filter()->Decide(context))
			{
				return;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			static std::map<LOG_PRIORITY_TYPE, int32_t> priorities
			{
				{ LOG_PRIORITY_TYPE::DEBUGS, EVENTLOG_INFORMATION_TYPE },
				{ LOG_PRIORITY_TYPE::INFO, EVENTLOG_INFORMATION_TYPE },
				{ LOG_PRIORITY_TYPE::NOTICE, EVENTLOG_INFORMATION_TYPE },
				{ LOG_PRIORITY_TYPE::WARNING, EVENTLOG_WARNING_TYPE },
				{ LOG_PRIORITY_TYPE::ERRORS, EVENTLOG_ERROR_TYPE },
				{ LOG_PRIORITY_TYPE::CRITICAL, EVENTLOG_ERROR_TYPE },
				{ LOG_PRIORITY_TYPE::ALERT, EVENTLOG_ERROR_TYPE },
				{ LOG_PRIORITY_TYPE::FATAL, EVENTLOG_ERROR_TYPE },
				{ LOG_PRIORITY_TYPE::EMERG, EVENTLOG_ERROR_TYPE },
			};

			const char * ps[1]{ 0 };

			ps[0] = Layout() ? Layout()->Format(context).c_str() : context.content.c_str();

			::ReportEvent
			(
				_handle,
				priorities[context.priority],
				0,
				static_cast<DWORD>(context.processID),
				nullptr,
				1,
				0,
				ps,
				nullptr
			);

		#else

			static std::map<LOG_PRIORITY_TYPE, int32_t> priorities
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

			::syslog
			(
				priorities[context.priority],
				"%s",
				Layout() ? Layout()->Format(context).c_str() : context.content.c_str()
			);

		#endif

			if (IsAutoFlush())
			{
				Flush();
			}
		}
	}
}
