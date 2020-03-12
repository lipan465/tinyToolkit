#ifndef __TINY_TOOLKIT__LOG__FILTER__H__
#define __TINY_TOOLKIT__LOG__FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 过滤器
 *
 */


#include "context.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <memory>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <memory>
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API IFilter
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~IFilter() = default;

			/**
			 *
			 * 过滤处理
			 *
			 * @param context 上下文
			 *
			 * @return 是否过滤
			 *
			 */
			bool Decide(const Context & context);

			/**
			 *
			 * 添加过滤器
			 *
			 * @param filter 过滤器
			 *
			 * @return 过滤器
			 *
			 */
			std::shared_ptr<IFilter> AddFilter(std::shared_ptr<IFilter> filter);

		protected:
			/**
			 *
			 * 过滤处理
			 *
			 * @param context 上下文
			 *
			 * @return 是否过滤
			 *
			 */
			virtual bool Filter(const Context & context) = 0;

		private:
			/**
			 *
			 * 下一个过滤器
			 *
			 * @return 过滤器
			 *
			 */
			const std::shared_ptr<IFilter> & NextFilter();

		private:
			std::shared_ptr<IFilter> _nextFilter{ };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__FILTER__H__
