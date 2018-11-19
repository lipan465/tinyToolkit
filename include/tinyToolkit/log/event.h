#ifndef __TINY_TOOLKIT__LOG__EVENT__H__
#define __TINY_TOOLKIT__LOG__EVENT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志事件
 *
 */


#include "detail.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API LogEvent
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		LogEvent() = default;

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logPriority 日志优先级
		 *
		 */
		explicit LogEvent(std::string logName, LOG_PRIORITY_TYPE logPriority) : name(std::move(logName)), priority(logPriority)
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param rhs 实例化对象
		 *
		 */
		LogEvent(LogEvent && rhs) noexcept : tm(rhs.tm),
											 threadID(rhs.threadID),
											 processID(rhs.processID),
											 name(std::move(rhs.name)),
											 message(std::move(rhs.message)),
											 time(rhs.time),
											 option(rhs.option),
											 priority(rhs.priority)
		{

		}

		/**
		 *
		 * 重载=操作
		 *
		 * @param rhs 实例化对象
		 *
		 */
		LogEvent & operator=(LogEvent && rhs) noexcept
		{
			tm = rhs.tm;

			threadID = rhs.threadID;
			processID = rhs.processID;

			name = std::move(rhs.name);
			message = std::move(rhs.message);

			time = rhs.time;

			option = rhs.option;
			priority = rhs.priority;

			return *this;
		}

	public:
		std::tm tm{ };

		std::size_t threadID{ static_cast<size_t>(tinyToolkit::OS::ThreadID()) };
		std::size_t processID{ static_cast<size_t>(tinyToolkit::OS::ProcessID()) };

		std::string name{ };
		std::string message{ };

		ClockTimePoint time{ tinyToolkit::Time::TimePoint() };

		LOG_OPTION_TYPE option{ LOG_OPTION_TYPE::WRITE };
		LOG_PRIORITY_TYPE priority{ LOG_PRIORITY_TYPE::INFO };
	};
}


#endif // __TINY_TOOLKIT__LOG__EVENT__H__
