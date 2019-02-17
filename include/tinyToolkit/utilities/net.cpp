/**
 *
 *  作者: hm
 *
 *  说明: 地址处理
 *
 */


#include "net.h"


namespace tinyToolkit
{
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
		return inet_addr(value);
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
	 * 转换网络字节序
	 *
	 * @param value 待转换字符串
	 *
	 * @return 网络字节序
	 *
	 */
	uint32_t Net::AsNetByte(const std::string & value)
	{
		return AsNetByte(value.c_str());
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
	uint32_t Net::AsHostByte(const std::string & value)
	{
		return AsHostByte(value.c_str());
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
				head = AsNetByte(value);
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

					head = AsNetByte(value.substr(0, pos));
					tail = head | ~(mask == 0 ? 0 : htonl(0xFFFFFFFF << (32 - mask)));
				}
				else  /// a.b.c.d/a.b.c.d
				{
					head = AsNetByte(value.substr(0, pos));
					tail = AsNetByte(value.substr(pos + 1));
				}
			}
		}
		else  /// a.b.c.d-a.b.c.d
		{
			head = AsNetByte(value.substr(0, pos));
			tail = AsNetByte(value.substr(pos + 1));
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
				head = AsHostByte(value);
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

					head = AsHostByte(value.substr(0, pos));
					tail = head | ~(mask == 0 ? 0 : 0xFFFFFFFF << (32 - mask));
				}
				else  /// a.b.c.d/a.b.c.d
				{
					head = AsHostByte(value.substr(0, pos));
					tail = AsHostByte(value.substr(pos + 1));
				}
			}
		}
		else  /// a.b.c.d-a.b.c.d
		{
			head = AsHostByte(value.substr(0, pos));
			tail = AsHostByte(value.substr(pos + 1));
		}

		return head <= tail;
	}

	/**
	 *
	 * 设置堆栈大小
	 *
	 * @param size 大小
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::SetStackSize(uint32_t size)
	{
		struct rlimit limit{ };

		limit.rlim_max = size * 1024;
		limit.rlim_cur = size * 1024;

		return setrlimit(RLIMIT_STACK, &limit) == 0;
	}

	/**
	 *
	 * 设置最大打开描诉符限制
	 *
	 * @param size 个数
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::SetMaxOpenFile(uint32_t size)
	{
		struct rlimit limit{ };

		limit.rlim_max = size;
		limit.rlim_cur = size;

		return setrlimit(RLIMIT_NOFILE, &limit) == 0;
	}

	/**
	 *
	 * 获取本地名称
	 *
	 * @param socket 句柄
	 * @param host 地址
	 * @param port 端口
	 *
	 * @return 是否获取成功
	 *
	 */
	bool Net::GetLocalName(int32_t socket, std::string & host, uint16_t & port)
	{
		struct sockaddr_in address{ };

		std::size_t addressLen = sizeof(address);

		if (::getsockname(socket, (struct sockaddr *)&address, (socklen_t *)&addressLen) == 0)
		{
			host = inet_ntoa(address.sin_addr);

			port = ntohs(address.sin_port);

			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 *
	 * 获取远程名称
	 *
	 * @param socket 句柄
	 * @param host 地址
	 * @param port 端口
	 *
	 * @return 是否获取成功
	 *
	 */
	bool Net::GetRemoteName(int32_t socket, std::string & host, uint16_t & port)
	{
		struct sockaddr_in address{ };

		std::size_t addressLen = sizeof(address);

		if (::getpeername(socket, (struct sockaddr *)&address, (socklen_t *)&addressLen) == 0)
		{
			host = inet_ntoa(address.sin_addr);

			port = ntohs(address.sin_port);

			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 *
	 * 启用Nagle算法
	 *
	 * @param socket 句柄
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::EnableNoDelay(int32_t socket)
	{
		int32_t val = 1l;

		return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) == 0;
	}

	/**
	 *
	 * 启用端口复用
	 *
	 * @param socket 句柄
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::EnableReusePort(int32_t socket)
	{
		int32_t val = 1l;

		return setsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val)) == 0;
	}

	/**
	 *
	 * 启用非堵塞
	 *
	 * @param socket 句柄
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::EnableNonBlocking(int32_t socket)
	{
		return fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK) == 0;
	}

	/**
	 *
	 * 启用地址复用
	 *
	 * @param socket 句柄
	 *
	 * @return 是否设置成功
	 *
	 */
	bool Net::EnableReuseAddress(int32_t socket)
	{
		int32_t val = 1l;

		return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == 0;
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

	/**
	 *
	 * 解析域名
	 *
	 * @param host 待解析域名
	 *
	 * @return 解析后域名
	 *
	 */
	std::string Net::ParseHost(const char * host)
	{
		struct hostent * list = gethostbyname(host);

		if (list)
		{
			return inet_ntoa(*(struct in_addr *)list->h_addr);
		}
		else
		{
			return { };
		}
	}

	/**
	 *
	 * 解析域名
	 *
	 * @param host 待解析域名
	 *
	 * @return 解析后域名
	 *
	 */
	std::string Net::ParseHost(const std::string & host)
	{
		return ParseHost(host.c_str());
	}
}
