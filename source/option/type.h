#ifndef __TINY_TOOLKIT__OPTION__TYPE__H__
#define __TINY_TOOLKIT__OPTION__TYPE__H__


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
	namespace option
	{
		enum class TINY_TOOLKIT_API OPTION_TYPE : uint8_t
		{
			BOOL,
			FLOAT,
			DOUBLE,
			LONG_DOUBLE,
			INT8,
			UINT8,
			INT16,
			UINT16,
			INT32,
			UINT32,
			INT64,
			UINT64,
			STRING,
			UNKNOWN,
		};
	}
}


#endif // __TINY_TOOLKIT__OPTION__TYPE__H__
