/**
 *
 *  作者: hm
 *
 *  说明: 通讯事件
 *
 */


#include "event.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	NetEvent::NetEvent()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		memset(&_overlap, 0, sizeof(OVERLAPPED));

#else

		_type = NET_EVENT_TYPE::INVALID;

#endif
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param type 事件类型
	 * @param socket 句柄
	 * @param completer 完成者
	 *
	 */
	NetEvent::NetEvent(NET_EVENT_TYPE type, TINY_TOOLKIT_SOCKET_TYPE socket, INetCompleter * completer) : _completer(completer),
																										  _type(type),
																										  _socket(socket)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		memset(&_overlap, 0, sizeof(OVERLAPPED));

#endif
	}
}
