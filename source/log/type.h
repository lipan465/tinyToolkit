#ifndef __TINY_TOOLKIT__LOG__TYPE__H__
#define __TINY_TOOLKIT__LOG__TYPE__H__


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
	namespace log
	{
		/**
		 *
		 * 日志操作
		 *
		 */
		enum class TINY_TOOLKIT_API LOG_OPTION_TYPE : uint8_t
		{
			WRITE,
			FLUSH,
			TERMINATE,
		};

		/**
		 *
		 * 日志优先级
		 *
		 */
		enum class TINY_TOOLKIT_API LOG_PRIORITY_TYPE : uint8_t
		{
			DEBUGS,
			INFO,
			NOTICE,
			WARNING,
			ERRORS,
			CRITICAL,
			ALERT,
			FATAL,
			EMERG,
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__TYPE__H__
