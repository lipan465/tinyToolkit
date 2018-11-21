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
		 * @param logOption 日志操作
		 *
		 */
		explicit LogEvent(LOG_OPTION_TYPE logOption) : option(logOption)
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logOption 日志操作
		 *
		 */
		explicit LogEvent(std::string logName, LOG_OPTION_TYPE logOption) : name(std::move(logName)),
																			option(logOption)
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logPriority 日志优先级
		 *
		 */
		explicit LogEvent(std::string logName, LOG_PRIORITY_TYPE logPriority) : name(std::move(logName)),
																				priority(logPriority)
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logOption 日志操作
		 * @param logPriority 日志优先级
		 *
		 */
		explicit LogEvent(std::string logName, LOG_OPTION_TYPE logOption, LOG_PRIORITY_TYPE logPriority) : name(std::move(logName)),
																										   option(logOption),
																										   priority(logPriority)
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logMessage 日志信息
		 * @param logOption 日志操作
		 *
		 */
		explicit LogEvent(std::string logName, std::string logMessage, LOG_OPTION_TYPE logOption) : name(std::move(logName)),
																									message(std::move(logMessage)),
																									option(logOption)
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logMessage 日志信息
		 * @param logPriority 日志优先级
		 *
		 */
		explicit LogEvent(std::string logName, std::string logMessage, LOG_PRIORITY_TYPE logPriority) : name(std::move(logName)),
																										message(std::move(logMessage)),
																										priority(logPriority)
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param rhs 实例化对象
		 *
		 */
		LogEvent(const LogEvent & rhs) = default;

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
		LogEvent & operator=(const LogEvent & rhs) = default;

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

		std::size_t threadID{ static_cast<size_t>(OS::ThreadID()) };
		std::size_t processID{ static_cast<size_t>(OS::ProcessID()) };

		std::string name{ };
		std::string message{ };

		ClockTimePoint time{ Time::TimePoint() };

		LOG_OPTION_TYPE option{ LOG_OPTION_TYPE::WRITE };
		LOG_PRIORITY_TYPE priority{ LOG_PRIORITY_TYPE::INFO };
	};
}


#endif // __TINY_TOOLKIT__LOG__EVENT__H__
