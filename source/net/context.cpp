/**
 *
 *  作者: hm
 *
 *  说明: 上下文
 *
 */


#include "context.h"


namespace tinyToolkit
{
	namespace net
	{
		/**
		 *
		 * 构造函数
		 *
		 */
		Context::Context()
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			memset(&buffer,  0, sizeof(WSABUF));
			memset(&overlap, 0, sizeof(OVERLAPPED));

		#else

			(void)nullptr;

		#endif
		}
	}
}
