#ifndef __TINY_TOOLKIT__NET__MESSAGE__H__
#define __TINY_TOOLKIT__NET__MESSAGE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 消息
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <cstring>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <cstring>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstring>
#
#endif


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API Message
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param buf 内容
			 * @param len 长度
			 *
			 */
			Message(const void * buf, std::size_t len);

			/**
			 *
			 * 析构函数
			 *
			 */
			~Message();

		public:
			char * buffer{ nullptr };

			std::size_t offset{ 0 };
			std::size_t length{ 0 };
		};
	}
}


#endif // __TINY_TOOLKIT__NET__MESSAGE__H__
