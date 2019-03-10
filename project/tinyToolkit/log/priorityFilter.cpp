/**
 *
 *  作者: hm
 *
 *  说明: 日志优先级过滤器
 *
 */


#include "transition.h"
#include "priorityFilter.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param priority 日志优先级
	 *
	 */
	PriorityLogFilter::PriorityLogFilter(const char * priority)
	{
		SetPriority(priority);
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param priority 日志优先级
	 *
	 */
	PriorityLogFilter::PriorityLogFilter(const std::string & priority)
	{
		SetPriority(priority);
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param priority 日志优先级
	 *
	 */
	PriorityLogFilter::PriorityLogFilter(LOG_PRIORITY_TYPE priority)
	{
		SetPriority(priority);
	}

	/**
	 *
	 * 设置日志优先级
	 *
	 * @param priority 日志优先级
	 *
	 */
	void PriorityLogFilter::SetPriority(const char * priority)
	{
		_priority = LogPriority::Value(priority);
	}

	/**
	 *
	 * 设置日志优先级
	 *
	 * @param priority 日志优先级
	 *
	 */
	void PriorityLogFilter::SetPriority(const std::string & priority)
	{
		_priority = LogPriority::Value(priority);
	}

	/**
	 *
	 * 设置日志优先级
	 *
	 * @param priority 日志优先级
	 *
	 */
	void PriorityLogFilter::SetPriority(LOG_PRIORITY_TYPE priority)
	{
		_priority = priority;
	}

	/**
	 *
	 * 过滤处理
	 *
	 * @param event 日志事件
	 *
	 * @return 是否过滤
	 *
	 */
	bool PriorityLogFilter::Filter(const LogEvent & event)
	{
		return _priority > event.priority;
	}
}
