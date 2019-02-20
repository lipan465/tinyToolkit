/**
 *
 *  作者: hm
 *
 *  说明: 网络处理
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
}
