#ifndef __TINY_TOOLKIT__ID__UNIQUE__H__
#define __TINY_TOOLKIT__ID__UNIQUE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 全局唯一标识码
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <cstdint>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <cstdint>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstdint>
#
#endif


namespace tinyToolkit
{
	namespace id
	{
		class TINY_TOOLKIT_API Unique
		{
			union ONLY_ID
			{
				struct
				{
					uint32_t count;
					uint32_t times;
				}key;

				uint64_t value{ 0 };
			};

		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			Unique();

			/**
			 *
			 * 生成
			 *
			 * @return 唯一标识码
			 *
			 */
			uint64_t Generate();

		private:
			ONLY_ID _id{ };
		};
	}
}


#endif // __TINY_TOOLKIT__ID__UNIQUE__H__
