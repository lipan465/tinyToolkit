/**
 *
 *  作者: hm
 *
 *  说明: 日志时间过滤器
 *
 */


#include "timeFilter.h"

#include "../utilities/time.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param time 秒数时间戳
	 *
	 */
	TimeLogFilter::TimeLogFilter(std::time_t time)
	{
		SetTime(time);
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param time 时间点
	 *
	 */
	TimeLogFilter::TimeLogFilter(const ClockTimePoint & time)
	{
		SetTime(time);
	}

	/**
	 *
	 * 设置时间
	 *
	 * @param time 秒数时间戳
	 *
	 */
	void TimeLogFilter::SetTime(std::time_t time)
	{
		_time = Time::TimePoint(time);
	}

	/**
	 *
	 * 设置时间
	 *
	 * @param time 时间点
	 *
	 */
	void TimeLogFilter::SetTime(const ClockTimePoint & time)
	{
		_time = time;
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
	bool TimeLogFilter::Filter(const LogEvent & event)
	{
		return _time > event.time;
	}
}
