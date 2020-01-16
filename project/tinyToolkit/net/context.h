#ifndef __TINY__TOOLKIT__NET__CONTEXT__H__
#define __TINY__TOOLKIT__NET__CONTEXT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 上下文
 *
 */


#include "completer.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetContext
	{
	public:
		enum class TYPE : uint8_t
		{
			IO,
			SEND,
			ACCEPT,
			CONNECT,
			RECEIVE,
			INVALID,
		};

	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		NetContext();

	public:
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		OVERLAPPED overlap{ 0 };

		WSABUF buffer{ 0 };

#endif

		char cache[TINY_TOOLKIT_SOCKET_CACHE_SIZE]{ 0 };

		TYPE type{ TYPE::INVALID };

		INetCompleter * completer{ nullptr };

		std::size_t bytes{ 0 };

		TINY_TOOLKIT_SOCKET_TYPE socket{ TINY_TOOLKIT_SOCKET_INVALID };
		TINY_TOOLKIT_SOCKET_TYPE acceptSocket{ TINY_TOOLKIT_SOCKET_INVALID };
	};
}


#endif // __TINY__TOOLKIT__NET__CONTEXT__H__
