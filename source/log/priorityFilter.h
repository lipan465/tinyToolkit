#ifndef __TINY_TOOLKIT__LOG__PRIORITY_FILTER__H__
#define __TINY_TOOLKIT__LOG__PRIORITY_FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 优先级过滤器
 *
 */


#include "filter.h"


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API PriorityFilter : public IFilter
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param priority 优先级
			 *
			 */
			explicit PriorityFilter(LOG_PRIORITY_TYPE priority);

			/**
			 *
			 * 构造函数
			 *
			 * @param priority 优先级
			 *
			 */
			explicit PriorityFilter(const std::string & priority);

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
			LOG_PRIORITY_TYPE _priority{ LOG_PRIORITY_TYPE::INFO };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__PRIORITY_FILTER__H__
