/**
 *
 *  作者: hm
 *
 *  说明: socket
 *
 */


#include "socket.h"

#include "../utilities/net.h"


namespace tinyToolkit
{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	/**
	 *
	 * 异步接收处理
	 *
	 * @param socket 会话套接字
	 * @param acceptSocket 监听套接字
	 * @param buffer 缓冲区
	 * @param overlapped 结构指针
	 *
	 * @return 是否处理成功
	 *
	 */
	static int32_t AcceptEx(TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_TYPE acceptSocket, PVOID buffer, LPOVERLAPPED overlapped)
	{
		static LPFN_ACCEPTEX function = nullptr;

		if (function == nullptr)
		{
			GUID guid = WSAID_ACCEPTEX;

			DWORD byte = 0;

			WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &function, sizeof(function), &byte, nullptr, nullptr);
		}

		if (function)
		{
			DWORD byte = 0;

			if (!function(socket, acceptSocket, buffer, 0, sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16, &byte, overlapped))
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					return -1;
				}
			}

			return 0;
		}
		else
		{
			return -1;
		}
	}

	/**
	 *
	 * 异步连接处理
	 *
	 * @param socket 句柄
	 * @param address 地址
	 * @param addressLength 地址长度
	 * @param overlapped 结构指针
	 *
	 * @return 是否处理成功
	 *
	 */
	static int32_t ConnectEx(TINY_TOOLKIT_SOCKET_TYPE socket, const struct sockaddr * address, int32_t addressLength, LPOVERLAPPED overlapped)
	{
		static LPFN_CONNECTEX function = nullptr;

		if (function == nullptr)
		{
			GUID guid = WSAID_CONNECTEX;

			DWORD byte = 0;

			WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &function, sizeof(function), &byte, nullptr, nullptr);
		}

		if (function)
		{
			DWORD byte = 0;

			if (!function(socket, address, addressLength, nullptr, 0, &byte, overlapped))
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					return -1;
				}
			}

			return 0;
		}
		else
		{
			return -1;
		}
	}

#endif

	/**
	 *
	 * 构造函数
	 *
	 * @param socket 句柄
	 *
	 */
	NetSocket::NetSocket(TINY_TOOLKIT_SOCKET_TYPE socket) : _socket(socket)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	NetSocket::~NetSocket()
	{
		Close();
	}

	/**
	 *
	 * 关闭
	 *
	 * @return 是否关闭成功
	 *
	 */
	bool NetSocket::Close()
	{
		bool ret = true;

		if (_socket != TINY_TOOLKIT_SOCKET_INVALID)
		{
			ret = Net::CloseSocket(_socket);

			_socket = TINY_TOOLKIT_SOCKET_INVALID;
		}

		return ret;
	}

	/**
	 *
	 * 是否是有效句柄
	 *
	 * @return 是否是有效句柄
	 *
	 */
	bool NetSocket::IsValid()
	{
		return _socket != TINY_TOOLKIT_SOCKET_INVALID;
	}

	/**
	 *
	 * 设置是否开启延时关闭
	 *
	 * @param on 状态
	 * @param timeout 超时时长
	 *
	 * @return 是否设置成功
	 *
	 */
	bool NetSocket::SetLinger(bool on, int32_t timeout)
	{
		return Net::SetLinger(_socket, on, timeout);
	}

	/**
	 *
	 * 设置是否非堵塞
	 *
	 * @param on 状态
	 *
	 * @return 是否设置成功
	 *
	 */
	bool NetSocket::SetNoBlock(bool on)
	{
		return Net::SetNoBlock(_socket, on);
	}

	/**
	 *
	 * 设置是否关闭Nagle算法
	 *
	 * @param on 状态
	 *
	 * @return 是否设置成功
	 *
	 */
	bool NetSocket::SetNoDelay(bool on)
	{
		return Net::SetNoDelay(_socket, on);
	}

	/**
	 *
	 * 设置是否启用端口复用
	 *
	 * @param on 状态
	 *
	 * @return 是否设置成功
	 *
	 */
	bool NetSocket::SetReusePort(bool on)
	{
		return Net::SetReusePort(_socket, on);
	}

	/**
	 *
	 * 设置是否启用地址复用
	 *
	 * @param on 状态
	 *
	 * @return 是否设置成功
	 *
	 */
	bool NetSocket::SetReuseAddress(bool on)
	{
		return Net::SetReuseAddress(_socket, on);
	}

	/**
	 *
	 * 绑定句柄
	 *
	 * @param port 端口
	 *
	 * @return 绑定结果
	 *
	 */
	int32_t NetSocket::Bind(const char * host, uint16_t port)
	{
		struct sockaddr_in address{ };

		address.sin_port = htons(port);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Net::AsNetByte(host);

		return ::bind(_socket, reinterpret_cast<const struct sockaddr *>(&address), static_cast<socklen_t>(sizeof(struct sockaddr_in)));
	}

	/**
	 *
	 * 监听句柄
	 *
	 * @param backlog 上限
	 *
	 * @return 监听结果
	 *
	 */
	int32_t NetSocket::Listen(int32_t backlog)
	{
		return ::listen(_socket, backlog);
	}

	/**
	 *
	 * 接受连接
	 *
	 * @param context 上下文
	 *
	 * @return 接受结果
	 *
	 */
	int32_t NetSocket::Accept(NetContext * context)
	{
		(void)context;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		TINY_TOOLKIT_SOCKET_TYPE socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

		if (socket == TINY_TOOLKIT_SOCKET_INVALID)
		{
			Net::CloseSocket(socket);

			return -1;
		}

		context->acceptSocket = socket;

		context->buffer.buf = context->cache;
		context->buffer.len = sizeof(context->cache);

		memset(&context->overlap, 0, sizeof(OVERLAPPED));

		if (!AcceptEx(_socket, socket, context->cache, reinterpret_cast<LPOVERLAPPED>(context)))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				Net::CloseSocket(socket);

				return -1;
			}
		}

#else

		context->acceptSocket = ::accept(_socket, nullptr, nullptr);

		if (context->acceptSocket == TINY_TOOLKIT_SOCKET_INVALID)
		{
			return -1;
		}

#endif

		return 0;
	}

	/**
	 *
	 * 连接
	 *
	 * @param host 地址
	 * @param port 端口
	 * @param context 上下文
	 *
	 * @return 连接结果
	 *
	 */
	int32_t NetSocket::Connect(const char * host, uint16_t port, NetContext * context)
	{
		(void)context;

		struct sockaddr_in address{ };

		address.sin_port = htons(port);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Net::AsNetByte(host);

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		return ConnectEx
		(
			_socket,
			reinterpret_cast<const struct sockaddr *>(&address),
			static_cast<socklen_t>(sizeof(struct sockaddr_in)),
			reinterpret_cast<LPOVERLAPPED>(context)
		);

#else

		return ::connect(_socket, reinterpret_cast<const struct sockaddr *>(&address), static_cast<socklen_t>(sizeof(struct sockaddr_in)));

#endif
	}

	/**
	 *
	 * 发送
	 *
	 * @param buffer 待发送缓冲区指针
	 * @param length 待发送缓冲区长度
	 * @param context 上下文
	 *
	 * @return 发送字节数
	 *
	 */
	int32_t NetSocket::Send(const char * buffer, std::size_t length, NetContext * context)
	{
		(void)context;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		DWORD flag = 0;
		DWORD bytes = 0;

		memset(&context->overlap, 0, sizeof(OVERLAPPED));

		context->buffer.buf = const_cast<CHAR *>(buffer);
		context->buffer.len = static_cast<ULONG>(length);

		return WSASend(_socket, &context->buffer, 1, &bytes, flag, reinterpret_cast<LPWSAOVERLAPPED>(context), nullptr);

#else

		return static_cast<int32_t>(::send(_socket, buffer, length, MSG_NOSIGNAL));  /// 忽略SIGPIPE信号

#endif
	}

	/**
	 *
	 * 接收
	 *
	 * @param buffer 待接收缓冲区指针
	 * @param length 待接收缓冲区长度
	 * @param context 上下文
	 *
	 * @return 接收字节数
	 *
	 */
	int32_t NetSocket::Receive(char * buffer, std::size_t length, NetContext * context)
	{
		(void)buffer;
		(void)length;
		(void)context;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		DWORD flag = 0;
		DWORD bytes = 0;

		memset(&context->overlap, 0, sizeof(OVERLAPPED));

		context->buffer.buf = buffer;
		context->buffer.len = static_cast<ULONG>(length);

		return WSARecv(_socket, &context->buffer, 1, &bytes, &flag, reinterpret_cast<LPWSAOVERLAPPED>(context), nullptr);

#else

		return static_cast<int32_t>(::recv(_socket, buffer, length, MSG_NOSIGNAL));  /// 忽略SIGPIPE信号

#endif
	}

	/**
	 *
	 * 句柄
	 *
	 * @return 句柄
	 *
	 */
	TINY_TOOLKIT_SOCKET_TYPE NetSocket::Handle()
	{
		return _socket;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 构造函数
	 *
	 * @return 构造函数
	 *
	 */
	TCPSocket::TCPSocket()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

#else

		_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#endif
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 构造函数
	 *
	 * @return 构造函数
	 *
	 */
	UDPSocket::UDPSocket()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		_socket = WSASocketW(AF_INET, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);

#else

		_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

#endif
	}
}
