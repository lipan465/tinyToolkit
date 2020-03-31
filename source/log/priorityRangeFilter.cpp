/**
 *
 *  作者: hm
 *
 *  说明: 优先级范围过滤器
 *
 */


#include "correspond.h"
#include "priorityRangeFilter.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param head 优先级
		 * @param tail 优先级
		 *
		 */
		PriorityRangeFilter::PriorityRangeFilter(LOG_PRIORITY_TYPE head, LOG_PRIORITY_TYPE tail) : _head(head),
		                                                                                           _tail(tail)
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param head 优先级
		 * @param tail 优先级
		 *
		 */
		PriorityRangeFilter::PriorityRangeFilter(const std::string & head, const std::string & tail) : PriorityRangeFilter(PriorityCorrespond::Priority(head),
		                                                                                                                   PriorityCorrespond::Priority(tail))
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
		bool PriorityRangeFilter::Filter(const Context & context)
		{
			return _head <= context.priority && context.priority <= _tail;
		}
	}
}
