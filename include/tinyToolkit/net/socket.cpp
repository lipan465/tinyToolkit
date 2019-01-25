/**
 *
 *  作者: hm
 *
 *  说明: 通讯设置
 *
 */


#include "socket.h"

#include "../debug/trace.h"


namespace tinyToolkit
{
	/**
	 *
	 * 设置非堵塞
	 *
	 * @param socket 句柄
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Socket::SetNonBlocking(int32_t socket)
	{
		int32_t opt = fcntl(socket, F_GETFL, 0);

		if (opt == -1)
		{
			TINY_TOOLKIT_DEBUG(strerror(errno))

			return false;
		}

		if (fcntl(socket, F_SETFL, opt | O_NONBLOCK) == -1)
		{
			TINY_TOOLKIT_DEBUG(strerror(errno))

			return false;
		}

		return true;

	}

	/**
	 *
	 * 设置Nagle算法
	 *
	 * @param socket 句柄
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Socket::SetNodelay(int32_t socket)
	{
		int32_t val = 1l;

		if (setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&val, sizeof(val)) != 0)
		{
			TINY_TOOLKIT_DEBUG(strerror(errno))

			return false;
		}

		return true;
	}

	/**
	 *
	 * 设置延迟确认
	 *
	 * @param socket 句柄
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Socket::SetQuickAck(int32_t socket)
	{
		int32_t val = 1l;

		if (setsockopt(socket, IPPROTO_TCP, TCP_QUICKACK, (const char *)&val, sizeof(val)) != 0)
		{
			TINY_TOOLKIT_DEBUG(strerror(errno))

			return false;
		}

		return true;
	}

	/**
	 *
	 * 设置端口复用
	 *
	 * @param socket 句柄
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Socket::SetReuseAddress(int32_t socket)
	{
		int32_t val = 1l;

		if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&val, sizeof(val)) != 0)
		{
			TINY_TOOLKIT_DEBUG(strerror(errno))

			return false;
		}

		return true;
	}
}
