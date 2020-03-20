#ifndef __TINY__TOOLKIT__NET__ENDPOINT__H__
#define __TINY__TOOLKIT__NET__ENDPOINT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 端点
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <string>
#
#endif


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API Endpoint
		{
		public:
			/**
			 *
			 * 清空
			 *
			 */
			void Clear();

			/**
			 *
			 * 是否有效
			 *
			 * @return 是否有效
			 *
			 */
			bool IsValid();

		public:
			uint16_t port{ 0 };

			std::string host{ };
		};
	}
}


#endif // __TINY__TOOLKIT__NET__ENDPOINT__H__
