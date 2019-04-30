/**
 *
 *  作者: hm
 *
 *  说明: 日志时间范围过滤器
 *
 */


#include "timeRangeFilter.h"

#include "../utilities/time.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param head 时间点
	 * @param tail 时间点
	 *
	 */
	TimeRangeLogFilter::TimeRangeLogFilter(std::time_t head, std::time_t tail)
	{
		SetTime(head, tail);
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param head 时间点
	 * @param tail 时间点
	 *
	 */
	TimeRangeLogFilter::TimeRangeLogFilter(const SystemClockTimePoint & head, const SystemClockTimePoint & tail)
	{
		SetTime(head, tail);
	}

	/**
	 *
	 * 设置时间
	 *
	 * @param head 时间点
	 * @param tail 时间点
	 *
	 */
	void TimeRangeLogFilter::SetTime(std::time_t head, std::time_t tail)
	{
		_time.first = Time::TimePoint(head);
		_time.second = Time::TimePoint(tail);
	}

	/**
	 *
	 * 设置时间
	 *
	 * @param head 时间点
	 * @param tail 时间点
	 *
	 */
	void TimeRangeLogFilter::SetTime(const SystemClockTimePoint & head, const SystemClockTimePoint & tail)
	{
		_time.first = head;
		_time.second = tail;
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
	bool TimeRangeLogFilter::Filter(const LogEvent & event)
	{
		return event.time < _time.first || _time.second < event.time;
	}
}
