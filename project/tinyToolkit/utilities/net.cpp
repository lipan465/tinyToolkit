/**
 *
 *  作者: hm
 *
 *  说明: 网络处理
 *
 */


#include "net.h"

#include "../debug/trace.h"


namespace tinyToolkit
{
	/**
	 *
	 * 转换范围
	 *
	 * @param value 待转换字符串
	 * @param head 转换后范围首部
	 * @param tail 转换后范围尾部
	 *
	 * @return 是否转换成功
	 *
	 */
	bool Net::AsRange(const std::string & value, uint16_t & head, uint16_t & tail)
	{
		std::size_t pos = value.find('-');

		if (pos == std::string::npos)
		{
			head = static_cast<uint16_t>(strtoul(value.c_str(), nullptr, 10));
			tail = head;
		}
		else  /// a-b
		{
			head = static_cast<uint16_t>(strtoul(value.substr(0, pos).c_str(), nullptr, 10));
			tail = static_cast<uint16_t>(strtoul(value.substr(pos + 1).c_str(), nullptr, 10));
		}

		return head <= tail;
	}

	/**
	 *
	 * 转换网络字节序范围
	 *
	 * @param value 待转换字符串
	 * @param head 转换后字节序范围首部
	 * @param tail 转换后字节序范围尾部
	 *
	 * @return 是否转换成功
	 *
	 */
	bool Net::AsNetByte(const std::string & value, uint32_t & head, uint32_t & tail)
	{
		std::size_t pos = value.find('-');

		if (pos == std::string::npos)
		{
			pos = value.find('/');

			if (pos == std::string::npos)  /// a.b.c.d
			{
				head = AsNetByte(value.c_str());
				tail = head;
			}
			else
			{
				if (value.find('.', pos + 1) == std::string::npos)  /// a.b.c.d/e
				{
					int64_t mask = strtol(value.substr(pos + 1).data(), nullptr, 10);

					if (32 < mask || mask < 0)
					{
						return false;
					}

					head = AsNetByte(value.substr(0, pos).c_str());
					tail = head | ~(mask == 0 ? 0 : htonl(0xFFFFFFFF << (32 - mask)));
				}
				else  /// a.b.c.d/a.b.c.d
				{
					head = AsNetByte(value.substr(0, pos).c_str());
					tail = AsNetByte(value.substr(pos + 1).c_str());
				}
			}
		}
		else  /// a.b.c.d-a.b.c.d
		{
			head = AsNetByte(value.substr(0, pos).c_str());
			tail = AsNetByte(value.substr(pos + 1).c_str());
		}

		return head <= tail;
	}

	/**
	 *
	 * 转换主机字节序范围
	 *
	 * @param value 待转换字符串
	 * @param head 转换后字节序范围首部
	 * @param tail 转换后字节序范围尾部
	 *
	 * @return 是否转换成功
	 *
	 */
	bool Net::AsHostByte(const std::string & value, uint32_t & head, uint32_t & tail)
	{
		std::size_t pos = value.find('-');

		if (pos == std::string::npos)
		{
			pos = value.find('/');

			if (pos == std::string::npos)  /// a.b.c.d
			{
				head = AsHostByte(value.c_str());
				tail = head;
			}
			else
			{
				if (value.find('.', pos + 1) == std::string::npos)  /// a.b.c.d/e
				{
					int64_t mask = strtol(value.substr(pos + 1).data(), nullptr, 10);

					if (32 < mask || mask < 0)
					{
						return false;
					}

					head = AsHostByte(value.substr(0, pos).c_str());
					tail = head | ~(mask == 0 ? 0 : 0xFFFFFFFF << (32 - mask));
				}
				else  /// a.b.c.d/a.b.c.d
				{
					head = AsHostByte(value.substr(0, pos).c_str());
					tail = AsHostByte(value.substr(pos + 1).c_str());
				}
			}
		}
		else  /// a.b.c.d-a.b.c.d
		{
			head = AsHostByte(value.substr(0, pos).c_str());
			tail = AsHostByte(value.substr(pos + 1).c_str());
		}

		return head <= tail;
	}

	/**
	 *
	 * 遍历域名对应的地址列表
	 *
	 * @param host 待解析域名
	 * @param list 域名列表
	 *
	 * @return 是否解析成功
	 *
	 */
	bool Net::TraverseAddressFromHost(const char * host, std::vector<std::string> & list)
	{
		bool status = false;

		struct addrinfo   hints{ };
		struct addrinfo * result{ nullptr };

		hints.ai_flags = AI_CANONNAME;
		hints.ai_family = AF_UNSPEC;  /// 意味着函数返回的是适用于指定主机名和服务名且适合任何协议族的地址
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = 0;

		auto res = getaddrinfo(host, nullptr, &hints, &result);

		if (res == 0)
		{
			for (auto iter = result; iter != nullptr; iter = iter->ai_next)
			{
				char address[1024]{ 0 };

				res = getnameinfo(iter->ai_addr, iter->ai_addrlen, address, sizeof(address), nullptr, 0, NI_NUMERICHOST | NI_NUMERICSERV);

				if (res == 0)
				{
					status = true;

					list.emplace_back(address);
				}
				else
				{
					status = false;

					TINY_TOOLKIT_ASSERT(false, gai_strerror(res));

					break;
				}
			}

			freeaddrinfo(result);
		}
		else
		{
			status = false;

			TINY_TOOLKIT_ASSERT(false, gai_strerror(res));
		}

		return status;
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
	bool Net::CloseSocket(TINY_TOOLKIT_SOCKET_TYPE socket)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		return ::closesocket(socket) == 0;

#else

		return ::close(socket) == 0;

#endif
	}

	/**
	 *
	 * 关闭Nagle算法
	 *
	 * @param socket 套接字
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::DisableNagle(TINY_TOOLKIT_SOCKET_TYPE socket)
	{
		int32_t val = 1l;

		return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char *>(&val), sizeof(val)) == 0;
	}

	/**
	 *
	 * 设置延时关闭
	 *
	 * @param socket 套接字
	 * @param timeout 超时时长
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::EnableLinger(TINY_TOOLKIT_SOCKET_TYPE socket, int32_t timeout)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		LINGER val{ };

#else

		struct linger val{ };

#endif

		val.l_onoff = 1l;
		val.l_linger = timeout;

		return setsockopt(socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char *>(&val), sizeof(val)) == 0;
	}

	/**
	 *
	 * 启用非堵塞
	 *
	 * @param socket 套接字
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::EnableNonBlock(TINY_TOOLKIT_SOCKET_TYPE socket)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		u_long opt = 1l;

		return ioctlsocket(socket, FIONBIO, &opt) == 0;

#else

		return fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK) == 0;

#endif
	}

	/**
	 *
	 * 启用地址复用
	 *
	 * @param socket 套接字
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::EnableReuseAddress(TINY_TOOLKIT_SOCKET_TYPE socket)
	{
		int32_t val = 1l;

		return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&val), sizeof(val)) == 0;
	}

	/**
	 *
	 * 获取本地地址
	 *
	 * @param socket 套接字
	 * @param address 地址
	 *
	 * @return 是否获取成功
	 *
	 */
	bool Net::GetLocalAddress(TINY_TOOLKIT_SOCKET_TYPE socket, struct sockaddr_in & address)
	{
		std::size_t len = sizeof(struct sockaddr_in);

		return ::getsockname(socket, reinterpret_cast<struct sockaddr *>(&address), reinterpret_cast<socklen_t *>(&len)) == 0;
	}

	/**
	 *
	 * 获取远程地址
	 *
	 * @param socket 套接字
	 * @param address 地址
	 *
	 * @return 是否获取成功
	 *
	 */
	bool Net::GetRemoteAddress(TINY_TOOLKIT_SOCKET_TYPE socket, struct sockaddr_in & address)
	{
		std::size_t len = sizeof(struct sockaddr_in);

		return ::getpeername(socket, reinterpret_cast<struct sockaddr *>(&address), reinterpret_cast<socklen_t *>(&len)) == 0;
	}

	/**
	 *
	 * 设置发送超时时间
	 *
	 * @param sock 套接字
	 * @param second 超时秒数
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::SetSendTimeout(TINY_TOOLKIT_SOCKET_TYPE sock, std::time_t second)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		std::time_t timeout = second * 1000;

		return setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&timeout), sizeof(timeout)) == 0;

#else

		struct timeval timeout
		{
			.tv_sec = second,
			.tv_usec = 0
		};

		return setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&timeout), sizeof(timeout)) == 0;

#endif
	}

	/**
	 *
	 * 设置接收超时时间
	 *
	 * @param sock 套接字
	 * @param second 超时秒数
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::SetReceiveTimeout(TINY_TOOLKIT_SOCKET_TYPE sock, std::time_t second)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		std::time_t timeout = second * 1000;

		return setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&timeout), sizeof(timeout)) == 0;

#else

		struct timeval timeout
		{
			.tv_sec = second,
			.tv_usec = 0
		};

		return setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&timeout), sizeof(timeout)) == 0;

#endif
	}

	/**
	 *
	 * 设置发送缓冲区大小
	 *
	 * @param sock 套接字
	 * @param size 缓冲区大小
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::SetSendBufferSize(TINY_TOOLKIT_SOCKET_TYPE sock, int32_t size)
	{
		return setsockopt(sock, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char *>(&size), sizeof(size)) == 0;
	}

	/**
	 *
	 * 设置接收缓冲区大小
	 *
	 * @param sock 套接字
	 * @param size 缓冲区大小
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::SetReceiveBufferSize(TINY_TOOLKIT_SOCKET_TYPE sock, int32_t size)
	{
		return setsockopt(sock, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char *>(&size), sizeof(size)) == 0;
	}

	/**
	 *
	 * 获取协议族
	 *
	 * @param sock 套接字
	 *
	 * @return 协议族
	 *
	 */
	int32_t Net::GetFamily(TINY_TOOLKIT_SOCKET_TYPE sock)
	{
		socklen_t len = sizeof(struct sockaddr_storage);

		struct sockaddr_storage storage{ };

		return getsockname(sock, reinterpret_cast<struct sockaddr *>(&storage), &len) == 0 ? storage.ss_family : -1;
	}

	/**
	 *
	 * 获取发送缓冲区大小
	 *
	 * @param sock 套接字
	 *
	 * @return 发送缓冲区大小
	 *
	 */
	int32_t Net::GetSendBufferSize(TINY_TOOLKIT_SOCKET_TYPE sock)
	{
		int32_t optionValue = -1;

		socklen_t optionLength = sizeof(optionValue);

		return getsockopt(sock, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char *>(&optionValue), &optionLength) == 0 ? optionValue : -1;
	}

	/**
	 *
	 * 获取接收缓冲区大小
	 *
	 * @param sock 套接字
	 *
	 * @return 接收缓冲区大小
	 *
	 */
	int32_t Net::GetReceiveBufferSize(TINY_TOOLKIT_SOCKET_TYPE sock)
	{
		int32_t optionValue = -1;

		socklen_t optionLength = sizeof(optionValue);

		return getsockopt(sock, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&optionValue), &optionLength) == 0 ? optionValue : -1;
	}

	/**
	 *
	 * 转换网络字节序
	 *
	 * @param value 待转换字符串
	 *
	 * @return 网络字节序
	 *
	 */
	uint32_t Net::AsNetByte(const char * value)
	{
		struct in_addr addr{ 0 };

		inet_pton(AF_INET, value, &addr);

		return addr.s_addr;
	}

	/**
	 *
	 * 转换主机字节序
	 *
	 * @param value 待转换字符串
	 *
	 * @return 主机字节序
	 *
	 */
	uint32_t Net::AsHostByte(const char * value)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		return ntohl(AsNetByte(value));

#else

		return inet_network(value);

#endif
	}

	/**
	 *
	 * 转换为字符串
	 *
	 * @param value 待转换主机字节序
	 *
	 * @return 字符串
	 *
	 */
	std::string Net::AsString(uint32_t value)
	{
		return  std::to_string((value >> 24) & 0xFF) + "." +
				std::to_string((value >> 16) & 0xFF) + "." +
				std::to_string((value >>  8) & 0xFF) + "." +
				std::to_string((value >>  0) & 0xFF);
	}
}
