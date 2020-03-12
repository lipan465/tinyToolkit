#ifndef __TINY_TOOLKIT__LOG__TIME_RANGE_FILTER__H__
#define __TINY_TOOLKIT__LOG__TIME_RANGE_FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 时间范围过滤器
 *
 */


#include "filter.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <ctime>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API TimeRangeFilter : public IFilter
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param head 时间点
			 * @param tail 时间点
			 *
			 */
			TimeRangeFilter(std::time_t head, std::time_t tail);

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
			std::chrono::system_clock::time_point _head{ };
			std::chrono::system_clock::time_point _tail{ };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__TIME_RANGE_FILTER__H__
