/**
 *
 *  作者: hm
 *
 *  说明: 日志优先级范围过滤器
 *
 */


#include "transition.h"
#include "priorityRangeFilter.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param head 日志优先级
	 * @param tail 日志优先级
	 *
	 */
	PriorityRangeLogFilter::PriorityRangeLogFilter(LOG_PRIORITY_TYPE head, LOG_PRIORITY_TYPE tail)
	{
		SetPriority(head, tail);
	}

	/**
	 *
	 * 设置日志优先级
	 *
	 * @param head 日志优先级
	 * @param tail 日志优先级
	 *
	 */
	void PriorityRangeLogFilter::SetPriority(const char * head, const char * tail)
	{
		_priority.first = LogPriority::Value(head);
		_priority.second = LogPriority::Value(tail);
	}

	/**
	 *
	 * 设置日志优先级
	 *
	 * @param head 日志优先级
	 * @param tail 日志优先级
	 *
	 */
	void PriorityRangeLogFilter::SetPriority(LOG_PRIORITY_TYPE head, LOG_PRIORITY_TYPE tail)
	{
		_priority.first = head;
		_priority.second = tail;
	}

	/**
	 *
	 * 设置日志优先级
	 *
	 * @param head 日志优先级
	 * @param tail 日志优先级
	 *
	 */
	void PriorityRangeLogFilter::SetPriority(const std::string & head, const std::string & tail)
	{
		_priority.first = LogPriority::Value(head);
		_priority.second = LogPriority::Value(tail);
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
	bool PriorityRangeLogFilter::Filter(const LogEvent & event)
	{
		return event.priority < _priority.first || _priority.second < event.priority;
	}
}
