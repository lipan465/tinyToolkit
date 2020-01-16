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
	/**
	 *
	 * 构造函数
	 *
	 */
	NetContext::NetContext()
	{
		memset(cache, 0, sizeof(cache));

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		memset(&buffer, 0, sizeof(WSABUF));
		memset(&overlap, 0, sizeof(OVERLAPPED));

#endif
	}
}
