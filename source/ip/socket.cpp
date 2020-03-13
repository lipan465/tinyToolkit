/**
 *
 *  作者: hm
 *
 *  说明: 套接字处理
 *
 */


#include "socket.h"
#include "address.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <fcntl.h>
#  include <unistd.h>
#
#  include <sys/event.h>
#  include <sys/ioctl.h>
#
#  include <netinet/tcp.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <fcntl.h>
#  include <unistd.h>
#
#  include <sys/epoll.h>
#  include <sys/ioctl.h>
#
#  include <netinet/tcp.h>
#
#endif


namespace tinyToolkit
{
	namespace ip
	{
		/**
		 *
		 * 是否有效
		 *
		 * @param socket 套接字
		 *
		 * @return 是否有效
		 *
		 */
		bool Socket::IsValid(TINY_TOOLKIT_SOCKET_TYPE socket)
		{
			return socket != TINY_TOOLKIT_SOCKET_INVALID;
		}

		/**
		 *
		 * 是否有效
		 *
		 * @param socket 套接字
		 *
		 * @return 是否有效
		 *
		 */
		bool Socket::IsValidPoll(TINY_TOOLKIT_SOCKET_POLL_TYPE socket)
		{
			return socket != TINY_TOOLKIT_SOCKET_POLL_INVALID;
		}

		/**
		 *
		 * 关闭套接字
		 *
		 * @param socket 套接字
		 *
		 * @return 是否关闭成功
		 *
		 */
		bool Socket::Close(TINY_TOOLKIT_SOCKET_TYPE socket)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			WSACleanup();

			return ::closesocket(socket) == 0;

		#else

			return ::close(socket) == 0;

		#endif
		}

		/**
		 *
		 * 关闭轮询套接字
		 *
		 * @param socket 套接字
		 *
		 * @return 是否关闭成功
		 *
		 */
		bool Socket::ClosePoll(TINY_TOOLKIT_SOCKET_POLL_TYPE socket)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			WSACleanup();

			return ::CloseHandle(socket) == TRUE;

		#else

			return ::close(socket) == 0;

		#endif
		}

		/**
		 *
		 * 获取目标地址信息
		 *
		 * @param socket 套接字
		 * @param address 地址信息
		 *
		 * @return 是否获取成功
		 *
		 */
		bool Socket::GetPeerAddress(TINY_TOOLKIT_SOCKET_TYPE socket, struct sockaddr & address)
		{
			socklen_t len = sizeof(struct sockaddr);

			return ::getpeername(socket, &address, &len) == 0;
		}

		/**
		 *
		 * 获取本地地址信息
		 *
		 * @param socket 套接字
		 * @param address 地址信息
		 *
		 * @return 是否获取成功
		 *
		 */
		bool Socket::GetLocalAddress(TINY_TOOLKIT_SOCKET_TYPE socket, struct sockaddr & address)
		{
			socklen_t len = sizeof(struct sockaddr);

			return ::getsockname(socket, &address, &len) == 0;
		}

		/**
		 *
		 * 设置是否延时关闭
		 *
		 * @param socket 套接字
		 * @param on 状态
		 * @param timeout 超时时长
		 *
		 * @return 是否设置成功
		 *
		 */
		bool Socket::SetLinger(TINY_TOOLKIT_SOCKET_TYPE socket, bool on, int32_t timeout)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			LINGER val{ };

		#else

			struct linger val{ };

		#endif

			val.l_onoff = on ? 1 : 0;
			val.l_linger = timeout;

			return ::setsockopt
			(
				socket,
				SOL_SOCKET,
				SO_LINGER,
				reinterpret_cast<const char *>(&val),
				static_cast<socklen_t>(sizeof(val))
			) == 0;
		}

		/**
		 *
		 * 设置是否非堵塞
		 *
		 * @param socket 套接字
		 * @param on 状态
		 *
		 * @return 是否设置成功
		 *
		 */
		bool Socket::SetBlock(TINY_TOOLKIT_SOCKET_TYPE socket, bool on)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			u_long opt = on ? 0 : 1;

			return ::ioctlsocket(socket, FIONBIO, &opt) == 0;

		#else

			if (on)
			{
				return ::fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) & ~O_NONBLOCK) == 0;
			}
			else
			{
				return ::fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) |  O_NONBLOCK) == 0;
			}

		#endif
		}

		/**
		 *
		 * 设置是否关闭Nagle算法
		 *
		 * @param socket 套接字
		 * @param on 状态
		 *
		 * @return 是否设置成功
		 *
		 */
		bool Socket::SetDelay(TINY_TOOLKIT_SOCKET_TYPE socket, bool on)
		{
			int32_t opt = on ? 0 : 1;

			return ::setsockopt
			(
				socket,
				IPPROTO_TCP,
				TCP_NODELAY,
				reinterpret_cast<const char *>(&opt),
				static_cast<socklen_t>(sizeof(opt))
			) == 0;
		}

		/**
		 *
		 * 设置是否启用端口复用
		 *
		 * @param socket 套接字
		 * @param on 状态
		 *
		 * @return 是否设置成功
		 *
		 */
		bool  Socket::SetReusePort(TINY_TOOLKIT_SOCKET_TYPE socket, bool on)
		{
		#ifdef SO_REUSEPORT

			int32_t opt = on ? 1 : 0;

			return ::setsockopt
			(
				socket,
				SOL_SOCKET,
				SO_REUSEPORT,
				reinterpret_cast<const char *>(&opt),
				static_cast<socklen_t>(sizeof(opt))
			) == 0;

		#else

			return false;

		#endif
		}

		/**
		 *
		 * 设置是否启用地址复用
		 *
		 * @param socket 套接字
		 * @param on 状态
		 *
		 * @return 是否设置成功
		 *
		 */
		bool  Socket::SetReuseAddress(TINY_TOOLKIT_SOCKET_TYPE socket, bool on)
		{
			int32_t opt = on ? 1 : 0;

			return ::setsockopt
			(
				socket,
				SOL_SOCKET,
				SO_REUSEADDR,
				reinterpret_cast<const char *>(&opt),
				static_cast<socklen_t>(sizeof(opt))
			) == 0;
		}

		/**
		 *
		 * 设置发送超时时间
		 *
		 * @param socket 套接字
		 * @param second 超时秒数
		 *
		 * @return 是否设置成功
		 *
		 */
		bool Socket::SetSendTimeout(TINY_TOOLKIT_SOCKET_TYPE socket, std::time_t second)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			std::time_t timeout = second * 1000;

			return ::setsockopt
			(
				socket,
				SOL_SOCKET,
				SO_SNDTIMEO,
				reinterpret_cast<const char *>(&timeout),
				sizeof(timeout)
			) == 0;

		#else

			struct timeval timeout
			{
				.tv_sec = second,
				.tv_usec = 0
			};

			return ::setsockopt
			(
				socket,
				SOL_SOCKET,
				SO_SNDTIMEO,
				reinterpret_cast<const char *>(&timeout),
				static_cast<socklen_t>(sizeof(timeout))
			) == 0;

		#endif
		}

		/**
		 *
		 * 设置接收超时时间
		 *
		 * @param socket 套接字
		 * @param timeout 超时时间
		 *
		 * @return 是否设置成功
		 *
		 */
		bool Socket::SetReceiveTimeout(TINY_TOOLKIT_SOCKET_TYPE socket, std::time_t timeout)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			std::time_t val = timeout * 1000;

		#else

			struct timeval val
			{
				.tv_sec = timeout,
				.tv_usec = 0
			};

		#endif

			return ::setsockopt
			(
				socket,
				SOL_SOCKET,
				SO_RCVTIMEO,
				reinterpret_cast<const char *>(&val),
				static_cast<socklen_t>(sizeof(val))
			) == 0;
		}

		/**
		 *
		 * 设置发送缓冲区大小
		 *
		 * @param socket 套接字
		 * @param size 缓冲区大小
		 *
		 * @return 是否设置成功
		 *
		 */
		bool Socket::SetSendBufferSize(TINY_TOOLKIT_SOCKET_TYPE socket, int32_t size)
		{
			return ::setsockopt
			(
				socket,
				SOL_SOCKET,
				SO_SNDBUF,
				reinterpret_cast<const char *>(&size),
				static_cast<socklen_t>(sizeof(size))
			) == 0;
		}

		/**
		 *
		 * 设置接收缓冲区大小
		 *
		 * @param socket 套接字
		 * @param size 缓冲区大小
		 *
		 * @return 是否设置成功
		 *
		 */
		bool Socket::SetReceiveBufferSize(TINY_TOOLKIT_SOCKET_TYPE socket, int32_t size)
		{
			return ::setsockopt
			(
				socket,
				SOL_SOCKET,
				SO_RCVBUF,
				reinterpret_cast<const char *>(&size),
				static_cast<socklen_t>(sizeof(size))
			) == 0;
		}

		/**
		 *
		 * 协议族
		 *
		 * @param socket 套接字
		 *
		 * @return 协议族
		 *
		 */
		int32_t Socket::Family(TINY_TOOLKIT_SOCKET_TYPE socket)
		{
			socklen_t len = sizeof(struct sockaddr_storage);

			struct sockaddr_storage storage{ };

			return ::getsockname
			(
				socket,
				reinterpret_cast<struct sockaddr *>(&storage),
				&len
			) == 0 ? storage.ss_family : -1;
		}

		/**
		 *
		 * 可读数据大小
		 *
		 * @param socket 套接字
		 *
		 * @return 可读数据大小
		 *
		 */
		int32_t Socket::ReadableSize(TINY_TOOLKIT_SOCKET_TYPE socket)
		{
			int32_t val = 0;

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (::ioctlsocket(socket, FIONBIO, reinterpret_cast<u_long *>(&val)) != 0)
			{
				val = 0;
			}

		#else

			if(::ioctl(socket, FIONREAD, &val) != 0)
			{
				val = 0;
			}

		#endif

			return val;
		}

		/**
		 *
		 * 发送缓冲区大小
		 *
		 * @param socket 套接字
		 *
		 * @return 发送缓冲区大小
		 *
		 */
		int32_t Socket::SendBufferSize(TINY_TOOLKIT_SOCKET_TYPE socket)
		{
			int32_t val = -1;

			socklen_t len = sizeof(val);

			return ::getsockopt
			(
				socket,
				SOL_SOCKET,
				SO_SNDBUF,
				reinterpret_cast<char *>(&val),
				&len
			) == 0 ? val : -1;
		}

		/**
		 *
		 * 接收缓冲区大小
		 *
		 * @param socket 套接字
		 *
		 * @return 接收缓冲区大小
		 *
		 */
		int32_t Socket::ReceiveBufferSize(TINY_TOOLKIT_SOCKET_TYPE socket)
		{
			int32_t val = -1;

			socklen_t len = sizeof(val);

			return ::getsockopt
			(
				socket,
				SOL_SOCKET,
				SO_RCVBUF,
				reinterpret_cast<char *>(&val),
				&len
			) == 0 ? val : -1;
		}

		/**
		 *
		 * 对端端点
		 *
		 * @param socket 套接字
		 *
		 * @return 本地端点
		 *
		 */
		std::pair<std::string, uint16_t> Socket::PeerEndpointV4(TINY_TOOLKIT_SOCKET_TYPE socket)
		{
			struct sockaddr address{ };

			if (!GetPeerAddress(socket, address))
			{
				return std::make_pair(std::string{ }, 0);
			}

			return std::make_pair
			(
				Address::AsString(reinterpret_cast<struct sockaddr_in *>(&address)->sin_addr),
				ntohs(reinterpret_cast<struct sockaddr_in *>(&address)->sin_port)
			);
		}

		/**
		 *
		 * 对端端点
		 *
		 * @param socket 套接字
		 *
		 * @return 本地端点
		 *
		 */
		std::pair<std::string, uint16_t> Socket::PeerEndpointV6(TINY_TOOLKIT_SOCKET_TYPE socket)
		{
			struct sockaddr address{ };

			if (!GetPeerAddress(socket, address))
			{
				return std::make_pair(std::string{ }, 0);
			}

			return std::make_pair
			(
				Address::AsString(reinterpret_cast<struct sockaddr_in6 *>(&address)->sin6_addr),
				ntohs(reinterpret_cast<struct sockaddr_in6 *>(&address)->sin6_port)
			);
		}

		/**
		 *
		 * 本地端点
		 *
		 * @param socket 套接字
		 *
		 * @return 本地端点
		 *
		 */
		std::pair<std::string, uint16_t> Socket::LocalEndpointV4(TINY_TOOLKIT_SOCKET_TYPE socket)
		{
			struct sockaddr address{ };

			if (!GetLocalAddress(socket, address))
			{
				return std::make_pair(std::string{ }, 0);
			}

			return std::make_pair
			(
				Address::AsString(reinterpret_cast<struct sockaddr_in *>(&address)->sin_addr),
				ntohs(reinterpret_cast<struct sockaddr_in *>(&address)->sin_port)
			);
		}

		/**
		 *
		 * 本地端点
		 *
		 * @param socket 套接字
		 *
		 * @return 本地端点
		 *
		 */
		std::pair<std::string, uint16_t> Socket::LocalEndpointV6(TINY_TOOLKIT_SOCKET_TYPE socket)
		{
			struct sockaddr address{ };

			if (!GetLocalAddress(socket, address))
			{
				return std::make_pair(std::string{ }, 0);
			}

			return std::make_pair
			(
				Address::AsString(reinterpret_cast<struct sockaddr_in6 *>(&address)->sin6_addr),
				ntohs(reinterpret_cast<struct sockaddr_in6 *>(&address)->sin6_port)
			);
		}

		/**
		 *
		 * tcp套接字
		 *
		 * @param family 协议族
		 *
		 * @return tcp套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE Socket::TCPSocket(int32_t family)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			WSADATA wsaData{ };

			if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
			{
				return TINY_TOOLKIT_SOCKET_INVALID;
		}

			if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
			{
				WSACleanup();

				return TINY_TOOLKIT_SOCKET_INVALID;
			}

			return ::WSASocketW(family, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

		#else

			return ::socket(family, SOCK_STREAM, IPPROTO_TCP);

		#endif
		}

		/**
		 *
		 * tcp套接字
		 *
		 * @return tcp套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE Socket::TCPSocketV4()
		{
			return TCPSocket(AF_INET);
		}

		/**
		 *
		 * tcp套接字
		 *
		 * @return tcp套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE Socket::TCPSocketV6()
		{
			return TCPSocket(AF_INET6);
		}

		/**
		 *
		 * udp套接字
		 *
		 * @param family 协议族
		 *
		 * @return udp套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE Socket::UDPSocket(int32_t family)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			WSADATA wsaData{ };

			if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
			{
				return TINY_TOOLKIT_SOCKET_INVALID;
			}

			if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
			{
				WSACleanup();

				return TINY_TOOLKIT_SOCKET_INVALID;
			}

			return ::WSASocketW(family, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);

		#else

			return ::socket(family, SOCK_DGRAM, IPPROTO_UDP);

		#endif
		}

		/**
		 *
		 * udp套接字
		 *
		 * @return udp套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE Socket::UDPSocketV4()
		{
			return UDPSocket(AF_INET);
		}

		/**
		 *
		 * udp套接字
		 *
		 * @return udp套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE Socket::UDPSocketV6()
		{
			return UDPSocket(AF_INET6);
		}

		/**
		 *
		 * 轮询套接字
		 *
		 * @return 轮询套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_POLL_TYPE Socket::PollSocket()
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			WSADATA wsaData{ };

			if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
			{
				return TINY_TOOLKIT_SOCKET_POLL_INVALID;
			}

			if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
			{
				WSACleanup();

				return TINY_TOOLKIT_SOCKET_POLL_INVALID;
			}

			return CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			return kqueue();

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			return epoll_create1(EPOLL_CLOEXEC);

		#else

			return TINY_TOOLKIT_SOCKET_POLL_INVALID;

		#endif
		}
	}
}
