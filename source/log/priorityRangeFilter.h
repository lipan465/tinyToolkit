#ifndef __TINY_TOOLKIT__LOG__PRIORITY_RANGE_FILTER__H__
#define __TINY_TOOLKIT__LOG__PRIORITY_RANGE_FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 优先级范围过滤器
 *
 */


#include "filter.h"


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API PriorityRangeFilter : public IFilter
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param head 优先级
			 * @param tail 优先级
			 *
			 */
			PriorityRangeFilter(LOG_PRIORITY_TYPE head, LOG_PRIORITY_TYPE tail);

			/**
			 *
			 * 构造函数
			 *
			 * @param head 优先级
			 * @param tail 优先级
			 *
			 */
			PriorityRangeFilter(const std::string & head, const std::string & tail);

		private:
			/**
			 *
			 * 过滤处理
			 *
			 * @param context 上下文
			 *
			 * @return 是否过滤
			 *
			 */
			bool Filter(const Context & context) override;

		private:
			LOG_PRIORITY_TYPE _head{ };
			LOG_PRIORITY_TYPE _tail{ };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__PRIORITY_RANGE_FILTER__H__
