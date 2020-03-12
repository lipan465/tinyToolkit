/**
 *
 *  作者: hm
 *
 *  说明: 全局唯一标识码
 *
 */


#include "unique.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <chrono>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <chrono>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <chrono>
#
#endif


namespace tinyToolkit
{
	namespace id
	{
		/**
		 *
		 * 构造函数
		 *
		 */
		Unique::Unique()
		{
			/// 这里需要用稳定时间, 防止系统时间变化
			_id.key.count = 0;
			_id.key.times = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
		}

		/**
		 *
		 * 生成
		 *
		 * @return 唯一标识码
		 *
		 */
		uint64_t Unique::Generate()
		{
			/// 这里需要用稳定时间, 防止系统时间变化
			auto times = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());

			if (times == _id.key.times)
			{
				++_id.key.count;
			}
			else
			{
				_id.key.count = 0;
				_id.key.times = times;
			}

			return _id.value;
		}
	}
}
