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
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <sys/errno.h>
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

		/**
		 *
		 * 构造函数
		 *
		 * @param socket 套接字
		 *
		 */
		TCPAdaptor::TCPAdaptor(TINY_TOOLKIT_SOCKET_TYPE socket) : Adaptor(socket)
		{

		}

		/**
		 *
		 * 关闭
		 *
		 * @return 是否关闭成功
		 *
		 */
		bool TCPAdaptor::Close()
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
		bool TCPAdaptor::IsValid()
		{
			return ip::Socket::IsValid(_socket);
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
		int32_t TCPAdaptor::BindV4(const Endpoint & endpoint)
		{
			return ip::Socket::BindV4(_socket, endpoint.host.c_str(), endpoint.port);
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
		int32_t TCPAdaptor::BindV6(const Endpoint & endpoint)
		{
			return ip::Socket::BindV6(_socket, endpoint.host.c_str(), endpoint.port);
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
		int32_t TCPAdaptor::Listen(int32_t backlog)
		{
			return ip::Socket::Listen(_socket, backlog);
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
		int32_t TCPAdaptor::Accept(Context * context)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			memset(&context->overlap, 0, sizeof(OVERLAPPED));

			context->socket = ip::Socket::TCPSocketV4();

			context->buffer.buf = reinterpret_cast<CHAR *>(context->temp);
			context->buffer.len = static_cast<ULONG>(sizeof(context->temp));

			if (ip::Socket::Accept(_socket, context->socket, context->temp, context) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				ip::Socket::Close(context->socket);

				return TINY_TOOLKIT_SOCKET_ERROR;
			}

		#else

			context->socket = ip::Socket::Accept
			(
				_socket,
				TINY_TOOLKIT_SOCKET_INVALID,
				nullptr,
				nullptr
			);

			if (context->socket == TINY_TOOLKIT_SOCKET_INVALID)
			{
				return TINY_TOOLKIT_SOCKET_ERROR;
			}

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
		int32_t TCPAdaptor::ConnectV4(const Endpoint & endpoint, Context * context)
		{
			return ip::Socket::ConnectV4(_socket, endpoint.host.c_str(), endpoint.port, context);
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
		int32_t TCPAdaptor::ConnectV6(const Endpoint & endpoint, Context * context)
		{
			return ip::Socket::ConnectV6(_socket, endpoint.host.c_str(), endpoint.port, context);
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
		int32_t TCPAdaptor::Send(void * buffer, std::size_t length, Context * context)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			memset(&context->overlap, 0, sizeof(OVERLAPPED));

			context->buffer.buf = reinterpret_cast<CHAR *>(buffer);
			context->buffer.len = static_cast<ULONG>(length);

			return ip::Socket::Send(_socket, &context->buffer, 1, context);

		#else

			return ip::Socket::Send(_socket, buffer, length, context);

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
		int32_t TCPAdaptor::Receive(void * buffer, std::size_t length, Context * context)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			memset(&context->overlap, 0, sizeof(OVERLAPPED));

			context->buffer.buf = reinterpret_cast<CHAR *>(buffer);
			context->buffer.len = static_cast<ULONG>(length);

			return ip::Socket::Receive(_socket, &context->buffer, 1, context);

		#else

			return ip::Socket::Receive(_socket, buffer, length, context);

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
		UDPAdaptor::UDPAdaptor()
		{
			_socket = ip::Socket::UDPSocketV4();
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param socket 套接字
		 *
		 */
		UDPAdaptor::UDPAdaptor(TINY_TOOLKIT_SOCKET_TYPE socket) : Adaptor(socket)
		{

		}

		/**
		 *
		 * 关闭
		 *
		 * @return 是否关闭成功
		 *
		 */
		bool UDPAdaptor::Close()
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
		bool UDPAdaptor::IsValid()
		{
			return ip::Socket::IsValid(_socket);
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
		int32_t UDPAdaptor::BindV4(const Endpoint & endpoint)
		{
			return ip::Socket::BindV4(_socket, endpoint.host.c_str(), endpoint.port);
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
		int32_t UDPAdaptor::BindV6(const Endpoint & endpoint)
		{
			return ip::Socket::BindV6(_socket, endpoint.host.c_str(), endpoint.port);
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
		int32_t UDPAdaptor::Listen(int32_t backlog)
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
		int32_t UDPAdaptor::Accept(Context * context)
		{
			context->socket = ip::Socket::UDPSocketV4();

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			memset(&context->overlap, 0, sizeof(OVERLAPPED));

			context->buffer.buf = reinterpret_cast<CHAR *>(context->temp);
			context->buffer.len = static_cast<ULONG>(sizeof(context->temp));

			auto ret = ip::Socket::RecvFrom
			(
				_socket,
				&context->buffer,
				1,
				reinterpret_cast<struct sockaddr *>(&context->address.v4),
				static_cast<socklen_t>(sizeof(struct sockaddr_in)),
				context
			);

			if (ret == TINY_TOOLKIT_SOCKET_ERROR)
			{
				ip::Socket::Close(context->socket);

				return TINY_TOOLKIT_SOCKET_ERROR;
			}

		#else

			auto ret = ip::Socket::RecvFrom
			(
				_socket,
				context->temp,
				sizeof(context->temp),
				reinterpret_cast<struct sockaddr *>(&context->address.v4),
				static_cast<socklen_t>(sizeof(struct sockaddr_in)),
				context
			);

			if (ret > 0)
			{
				context->bytes = static_cast<std::size_t>(ret);
			}
			else
			{
				context->bytes = 0;

				if (errno != EINTR && errno != EAGAIN)
				{
					ip::Socket::Close(context->socket);

					return TINY_TOOLKIT_SOCKET_ERROR;
				}
			}

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
		int32_t UDPAdaptor::ConnectV4(const Endpoint & endpoint, Context * context)
		{
			(void)context;

			struct sockaddr_in address = ip::Address::AsAddressV4(endpoint.host.c_str(), endpoint.port);

			return ::connect
			(
				_socket,
				reinterpret_cast<const struct sockaddr *>(&address),
				static_cast<socklen_t>(sizeof(struct sockaddr_in))
			);
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
		int32_t UDPAdaptor::ConnectV6(const Endpoint & endpoint, Context * context)
		{
			(void)context;

			struct sockaddr_in6 address = ip::Address::AsAddressV6(endpoint.host.c_str(), endpoint.port);

			return ::connect
			(
				_socket,
				reinterpret_cast<const struct sockaddr *>(&address),
				static_cast<socklen_t>(sizeof(struct sockaddr_in6))
			);
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
		int32_t UDPAdaptor::Send(void * buffer, std::size_t length, Context * context)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			memset(&context->overlap, 0, sizeof(OVERLAPPED));

			context->buffer.buf = reinterpret_cast<CHAR *>(buffer);
			context->buffer.len = static_cast<ULONG>(length);

			return ip::Socket::Send(_socket, &context->buffer, 1, context);

		#else

			return ip::Socket::Send(_socket, buffer, length, context);

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
		int32_t UDPAdaptor::Receive(void * buffer, std::size_t length, Context * context)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			memset(&context->overlap, 0, sizeof(OVERLAPPED));

			context->buffer.buf = reinterpret_cast<CHAR *>(buffer);
			context->buffer.len = static_cast<ULONG>(length);

			return ip::Socket::Receive(_socket, &context->buffer, 1, context);

		#else

			return ip::Socket::Receive(_socket, buffer, length, context);

		#endif
		}
	}
}
