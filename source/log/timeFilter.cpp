/**
 *
 *  作者: hm
 *
 *  说明: 时间过滤器
 *
 */


#include "timeFilter.h"

#include "../util/time.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param time 秒数时间戳
		 *
		 */
		TimeFilter::TimeFilter(std::time_t time) : _head(util::Time::TimePoint(time)),
		                                           _tail(util::Time::TimePoint(time + 1))
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
		bool TimeFilter::Filter(const Context & context)
		{
			return _head <= context.time && context.time < _tail;
		}
	}
}
