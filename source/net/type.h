#ifndef __TINY__TOOLKIT__NET__TYPE__H__
#define __TINY__TOOLKIT__NET__TYPE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 类型
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
	namespace net
	{
		enum class TINY_TOOLKIT_API NET_OPTION_TYPE : uint8_t
		{
			IO,
			SEND,
			ACCEPT,
			CONNECT,
			RECEIVE,
			INVALID,
		};
	}
}


#endif // __TINY__TOOLKIT__NET__TYPE__H__
