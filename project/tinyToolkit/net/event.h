#ifndef __TINY_TOOLKIT__NET__EVENT__H__
#define __TINY_TOOLKIT__NET__EVENT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯事件
 *
 */


#include "type.h"
#include "completer.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetEvent
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		NetEvent() = default;

		/**
		 *
		 * 构造函数
		 *
		 * @param type 事件类型
		 * @param socket 句柄
		 * @param completer 完成者
		 *
		 */
		NetEvent(NET_EVENT_TYPE type, TINY_TOOLKIT_SOCKET_TYPE socket, INetCompleter * completer);

	public:
		std::size_t _bytes{ 0 };

		INetCompleter * _completer{ nullptr };

		NET_EVENT_TYPE _type{ NET_EVENT_TYPE::TRANSMIT };

		TINY_TOOLKIT_SOCKET_TYPE _socket{ TINY_TOOLKIT_SOCKET_INVALID };
	};
}


#endif // __TINY_TOOLKIT__NET__EVENT__H__
