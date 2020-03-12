/**
 *
 *  作者: hm
 *
 *  说明: 过滤器
 *
 */


#include "filter.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 过滤处理
		 *
		 * @param context 上下文
		 *
		 * @return 是否过滤
		 *
		 */
		bool IFilter::Decide(const Context & context)
		{
			if (Filter(context))
			{
				return true;
			}

			if (NextFilter())
			{
				return NextFilter()->Decide(context);
			}

			return false;
		}

		/**
		 *
		 * 添加过滤器
		 *
		 * @param filter 过滤器
		 *
		 * @return 过滤器
		 *
		 */
		std::shared_ptr<IFilter> IFilter::AddFilter(std::shared_ptr<IFilter> filter)
		{
			IFilter * end = this;

			while (end->NextFilter())
			{
				end = end->NextFilter().get();
			}

			end->_nextFilter = std::move(filter);

			return end->_nextFilter;
		}

		/**
		 *
		 * 下一个过滤器
		 *
		 * @return 过滤器
		 *
		 */
		const std::shared_ptr<IFilter> & IFilter::NextFilter()
		{
			return _nextFilter;
		}
	}
}
