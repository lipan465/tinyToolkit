/**
 *
 *  作者: hm
 *
 *  说明: 时间过滤器
 *
 */


#include "timeRangeFilter.h"

#include "../util/time.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param head 时间点
		 * @param tail 时间点
		 *
		 */
		TimeRangeFilter::TimeRangeFilter(std::time_t head, std::time_t tail) : _head(util::Time::TimePoint(head)),
		                                                                       _tail(util::Time::TimePoint(tail + 1))
		{

		}

		/**
		 *
		 * 过滤处理
		 *
		 * @param context 上下文
		 *
		 * @return 是否过滤
		 *
		 */
		bool TimeRangeFilter::Filter(const Context & context)
		{
			return _head <= context.time && context.time < _tail;
		}
	}
}
