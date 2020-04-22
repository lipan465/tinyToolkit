#ifndef __TINY__TOOLKIT__NET__CONTEXT__H__
#define __TINY__TOOLKIT__NET__CONTEXT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 上下文
 *
 */


#include "type.h"
#include "completer.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <cstring>
#
#  include <WS2tcpip.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <cstring>
#
#  include <netinet/in.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstring>
#
#  include <netinet/in.h>
#
#endif


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API Context
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			Context();

		public:
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			OVERLAPPED overlap{ 0 };

			WSABUF buffer{ 0 };

		#endif

			union
			{
				struct sockaddr_in v4;
				struct sockaddr_in6 v6;
			}address;

			char temp[1460]{ 0 };

			int32_t bytes{ 0 };

			ICompleter * completer{ nullptr };

			NET_OPTION_TYPE optionType{ NET_OPTION_TYPE::INVALID };

			TINY_TOOLKIT_SOCKET_TYPE socket{ TINY_TOOLKIT_SOCKET_INVALID };
		};
	}
}


#endif // __TINY__TOOLKIT__NET__CONTEXT__H__
