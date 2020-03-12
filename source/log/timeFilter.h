#ifndef __TINY_TOOLKIT__LOG__TIME_FILTER__H__
#define __TINY_TOOLKIT__LOG__TIME_FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 时间过滤器
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
		class TINY_TOOLKIT_API TimeFilter : public IFilter
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param time 秒数时间戳
			 *
			 */
			explicit TimeFilter(std::time_t time);

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


#endif // __TINY_TOOLKIT__LOG__TIME_FILTER__H__
