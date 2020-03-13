/**
 *
 *  作者: hm
 *
 *  说明: 网络处理
 *
 */


#include "adaptor.h"

#include "../ip/socket.h"
#include "../ip/address.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <MSWSock.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#
#
#endif


namespace tinyToolkit
{
	namespace net
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/**
		 *
		 * 异步接收处理
		 *
		 * @param sessionSocket 会话套接字
		 * @param acceptSocket 监听套接字
		 * @param buffer 缓冲区
		 * @param overlapped 结构指针
		 *
		 * @return 是否处理成功
		 *
		 */
		static int32_t AcceptEx(TINY_TOOLKIT_SOCKET_TYPE sessionSocket, TINY_TOOLKIT_SOCKET_TYPE acceptSocket, PVOID buffer, LPOVERLAPPED overlapped)
		{
			static LPFN_ACCEPTEX function = nullptr;

			if (function == nullptr)
			{
				GUID guid = WSAID_ACCEPTEX;

				DWORD byte = 0;

				WSAIoctl(sessionSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &function, sizeof(function), &byte, nullptr, nullptr);
			}

			if (function)
			{
				DWORD byte = 0;

				if (!function(sessionSocket, acceptSocket, buffer, 0, sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16, &byte, overlapped))
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
		 * @param socket 套接字
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
		 * @param socket 套接字
		 *
		 */
		Adaptor::Adaptor(TINY_TOOLKIT_SOCKET_TYPE socket) : _socket(socket)
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		Adaptor::~Adaptor()
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
		bool Adaptor::Close()
		{
			if (!ip::Socket::Close(_socket))
			{
				return false;
			}

			_socket = TINY_TOOLKIT_SOCKET_INVALID;

			return true;
		}

		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool Adaptor::IsValid()
		{
			return ip::Socket::IsValid(_socket);
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
		bool Adaptor::SetLinger(bool on, int32_t timeout)
		{
			return ip::Socket::SetLinger(_socket, on, timeout);
		}

		/**
		 *
		 * 设置是否堵塞
		 *
		 * @param on 状态
		 *
		 * @return 是否设置成功
		 *
		 */
		bool Adaptor::SetBlock(bool on)
		{
			return ip::Socket::SetBlock(_socket, on);
		}

		/**
		 *
		 * 设置是否开启Nagle算法
		 *
		 * @param on 状态
		 *
		 * @return 是否设置成功
		 *
		 */
		bool Adaptor::SetDelay(bool on)
		{
			return ip::Socket::SetDelay(_socket, on);
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
		bool Adaptor::SetReusePort(bool on)
		{
			return ip::Socket::SetReusePort(_socket, on);
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
		bool Adaptor::SetReuseAddress(bool on)
		{
			return ip::Socket::SetReuseAddress(_socket, on);
		}

		/**
		 *
		 * 绑定地址
		 *
		 * @param endpoint 端点
		 *
		 * @return 绑定结果
		 *
		 */
		int32_t Adaptor::Bind(const Endpoint & endpoint)
		{
			struct sockaddr_in address = ip::Address::AsAddressV4(endpoint.host.c_str(), endpoint.port);

			return ::bind
			(
				_socket,
				reinterpret_cast<const struct sockaddr *>(&address),
				static_cast<socklen_t>(sizeof(struct sockaddr_in))
			);
		}

		/**
		 *
		 * 监听地址
		 *
		 * @param backlog 上限
		 *
		 * @return 监听结果
		 *
		 */
		int32_t Adaptor::Listen(int32_t backlog)
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
		int32_t Adaptor::Accept(Context * context)
		{
			(void)context;

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			auto socket = ip::Socket::TCPSocketV4();

			if (!ip::Socket::IsValid(socket))
			{
				return -1;
			}

			ip::Socket::SetDelay(socket, false);
			ip::Socket::SetBlock(socket, false);
			ip::Socket::SetReuseAddress(socket, true);

			memset(&context->overlap, 0, sizeof(OVERLAPPED));

			context->socket = socket;

			context->buffer.buf = context->output;
			context->buffer.len = sizeof(context->output);

			if (!AcceptEx(_socket, socket, context->output, reinterpret_cast<LPOVERLAPPED>(context)))
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					ip::Socket::Close(socket);

					return -1;
				}
			}

		#else

			context->socket = ::accept(_socket, nullptr, nullptr);

			if (context->socket == TINY_TOOLKIT_SOCKET_INVALID)
			{
				return -1;
			}

			ip::Socket::SetDelay(context->socket, false);
			ip::Socket::SetBlock(context->socket, false);
			ip::Socket::SetReuseAddress(context->socket, true);

		#endif

			return 0;
		}

		/**
		 *
		 * 连接
		 *
		 * @param endpoint 端点
		 * @param context 上下文
		 *
		 * @return 连接结果
		 *
		 */
		int32_t Adaptor::Connect(const Endpoint & endpoint, Context * context)
		{
			(void)context;

			struct sockaddr_in address = ip::Address::AsAddressV4(endpoint.host.c_str(), endpoint.port);

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			return ConnectEx
			(
				_socket,
				reinterpret_cast<const struct sockaddr *>(&address),
				static_cast<socklen_t>(sizeof(struct sockaddr_in)),
				reinterpret_cast<LPOVERLAPPED>(context)
			);

		#else

			return ::connect
			(
				_socket,
				reinterpret_cast<const struct sockaddr *>(&address),
				static_cast<socklen_t>(sizeof(struct sockaddr_in))
			);

		#endif
		}

		/**
		 *
		 * 发送
		 *
		 * @param buffer 内容
		 * @param length 长度
		 * @param context 上下文
		 *
		 * @return 发送字节数
		 *
		 */
		int32_t Adaptor::Send(void * buffer, std::size_t length, Context * context)
		{
			(void)context;

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			DWORD flag = 0;
			DWORD bytes = 0;

			memset(&context->overlap, 0, sizeof(OVERLAPPED));
			
			context->buffer.buf = reinterpret_cast<CHAR *>(buffer);
			context->buffer.len = static_cast<ULONG>(length);

			return static_cast<int32_t>
			(
				::WSASend
				(
					_socket,
					&context->buffer,
					1,
					&bytes,
					flag,
					reinterpret_cast<LPWSAOVERLAPPED>(context),
					nullptr
				)
			);

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			return static_cast<int32_t>(::send(_socket, buffer, length, SO_NOSIGPIPE));

		#else

			return static_cast<int32_t>(::send(_socket, buffer, length, MSG_NOSIGNAL));

		#endif
		}

		/**
		 *
		 * 接收
		 *
		 * @param buffer 内容
		 * @param length 长度
		 * @param context 上下文
		 *
		 * @return 接收字节数
		 *
		 */
		int32_t Adaptor::Receive(void * buffer, std::size_t length, Context * context)
		{
			(void)buffer;
			(void)length;
			(void)context;

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			DWORD flag = 0;
			DWORD bytes = 0;

			memset(&context->overlap, 0, sizeof(OVERLAPPED));

			context->buffer.buf = reinterpret_cast<CHAR*>(buffer);
			context->buffer.len = static_cast<ULONG>(length);

			return static_cast<int32_t>
			(
				::WSARecv
				(
					_socket,
					&context->buffer,
					1,
					&bytes,
					&flag,
					reinterpret_cast<LPWSAOVERLAPPED>(context),
					nullptr
				)
			);

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			return static_cast<int32_t>(::recv(_socket, buffer, length, SO_NOSIGPIPE));

		#else

			return static_cast<int32_t>(::recv(_socket, buffer, length, MSG_NOSIGNAL));

		#endif
		}

		/**
		 *
		 * 套接字
		 *
		 * @return 套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE Adaptor::Socket() const
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
		TCPAdaptor::TCPAdaptor()
		{
			_socket = ip::Socket::TCPSocketV4();
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 构造函数
		 *
		 * @return 构造函数
		 *
		 */
		UDPAdaptor::UDPAdaptor()
		{
			_socket = ip::Socket::UDPSocketV4();
		}
	}
}
