#ifndef __TINY_TOOLKIT__DEBUG__STACK_TRACE__H__
#define __TINY_TOOLKIT__DEBUG__STACK_TRACE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 堆栈信息
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
	namespace debug
	{
		class TINY_TOOLKIT_API StackTrace
		{
		public:
			/**
			 *
			 * 内容
			 *
			 * @return 内容
			 *
			 */
			static std::string Content();
		};
	}
}


#endif // __TINY_TOOLKIT__DEBUG__STACK_TRACE__H__
