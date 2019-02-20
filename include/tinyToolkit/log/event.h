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

#include "../system/os.h"


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
		explicit LogEvent(LOG_OPTION_TYPE logOption);

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logOption 日志操作
		 *
		 */
		explicit LogEvent(std::string logName, LOG_OPTION_TYPE logOption);

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logPriority 日志优先级
		 *
		 */
		explicit LogEvent(std::string logName, LOG_PRIORITY_TYPE logPriority);

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logMessage 日志信息
		 * @param logOption 日志操作
		 *
		 */
		explicit LogEvent(std::string logName, std::string logMessage, LOG_OPTION_TYPE logOption);

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logMessage 日志信息
		 * @param logPriority 日志优先级
		 *
		 */
		explicit LogEvent(std::string logName, std::string logMessage, LOG_PRIORITY_TYPE logPriority);

		/**
		 *
		 * 构造函数
		 *
		 * @param logName 事件名称
		 * @param logOption 日志操作
		 * @param logPriority 日志优先级
		 *
		 */
		explicit LogEvent(std::string logName, LOG_OPTION_TYPE logOption, LOG_PRIORITY_TYPE logPriority);

		/**
		 *
		 * 构造函数
		 *
		 * @param rhs 实例化对象
		 *
		 */
		LogEvent(LogEvent && rhs) noexcept;

		/**
		 *
		 * 构造函数
		 *
		 * @param lhs 实例化对象
		 *
		 */
		LogEvent(const LogEvent & lhs) = default;

		/**
		 *
		 * 重载=操作
		 *
		 * @param rhs 实例化对象
		 *
		 * @return 实例化对象
		 *
		 */
		LogEvent & operator=(LogEvent && rhs) noexcept;

		/**
		 *
		 * 重载=操作
		 *
		 * @param lhs 实例化对象
		 *
		 * @return 实例化对象
		 *
		 */
		LogEvent & operator=(const LogEvent & lhs) = default;

	public:
		std::tm tm{ };

		uint64_t threadID{ OS::ThreadID() };
		uint64_t processID{ OS::ProcessID() };

		std::string name{ };
		std::string message{ };

		ClockTimePoint time{ };

		LOG_OPTION_TYPE option{ LOG_OPTION_TYPE::WRITE };
		LOG_PRIORITY_TYPE priority{ LOG_PRIORITY_TYPE::INFO };
	};
}


#endif // __TINY_TOOLKIT__LOG__EVENT__H__
