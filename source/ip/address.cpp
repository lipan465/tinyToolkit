/**
 *
 *  作者: hm
 *
 *  说明: 地址处理
 *
 */


#include "address.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  pragma comment(lib,"ws2_32.lib")
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <netdb.h>
#
#  include <arpa/inet.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <netdb.h>
#
#  include <arpa/inet.h>
#
#endif


namespace tinyToolkit
{
	namespace ip
	{
		/**
		 *
		 * 解析域名
		 *
		 * @param domain 域名
		 * @param list 列表
		 *
		 * @return 是否解析成功
		 *
		 */
		bool Address::ParseDomain(const char * domain, std::vector<std::string> & list)
		{
			bool status = true;

			struct addrinfo   hints{ };
			struct addrinfo * result{ nullptr };

			hints.ai_flags = AI_CANONNAME;
			hints.ai_family = AF_UNSPEC;  /// 意味着函数返回的是适用于指定主机名和服务名且适合任何协议族的地址
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = 0;

			auto res = getaddrinfo(domain, nullptr, &hints, &result);

			if (res != 0)
			{
				return false;
			}

			for (auto iter = result; iter != nullptr; iter = iter->ai_next)
			{
				char address[1024]{ 0 };

				res = getnameinfo
				(
					iter->ai_addr,
					static_cast<socklen_t>(iter->ai_addrlen),
					address,
					sizeof(address),
					nullptr,
					0,
					NI_NUMERICHOST | NI_NUMERICSERV
				);

				if (res != 0)
				{
					status = false;

					break;
				}

				list.emplace_back(address);
			}

			freeaddrinfo(result);

			return status;
		}

		/**
		 *
		 * 转换主机字节序
		 *
		 * @param host 主机
		 * @param address 地址结构
		 *
		 * @return 是否转换成功
		 *
		 */
		bool Address::AsHostByteOrder(const char * host, struct in_addr & address)
		{
			return ntohl(AsNetworkByteOrder(host, address));
		}

		/**
		 *
		 * 转换主机字节序
		 *
		 * @param host 主机
		 * @param address 地址结构
		 *
		 * @return 是否转换成功
		 *
		 */
		bool Address::AsHostByteOrder(const char * host, struct in6_addr & address)
		{
			return ntohl(AsNetworkByteOrder(host, address));
		}

		/**
		 *
		 * 转换网络字节序
		 *
		 * @param host 主机
		 * @param address 地址结构
		 *
		 * @return 是否转换成功
		 *
		 */
		bool Address::AsNetworkByteOrder(const char * host, struct in_addr & address)
		{
			return inet_pton(AF_INET, host, &address) > 0;
		}

		/**
		 *
		 * 转换网络字节序
		 *
		 * @param host 主机
		 * @param address 地址结构
		 *
		 * @return 是否转换成功
		 *
		 */
		bool Address::AsNetworkByteOrder(const char * host, struct in6_addr & address)
		{
			return inet_pton(AF_INET6, host, &address) > 0;
		}

		/**
		 *
		 * 转换字符串
		 *
		 * @param address 地址结构
		 *
		 * @return 字符串
		 *
		 */
		std::string Address::AsString(const struct in_addr & address)
		{
			char str[INET_ADDRSTRLEN]{ 0 };

			if (inet_ntop(AF_INET, &address, str, INET_ADDRSTRLEN))
			{
				return std::string(str);
			}
			else
			{
				return { };
			}
		}

		/**
		 *
		 * 转换字符串
		 *
		 * @param address 地址结构
		 *
		 * @return 字符串
		 *
		 */
		std::string Address::AsString(const struct in6_addr & address)
		{
			char str[INET6_ADDRSTRLEN]{ 0 };

			if (inet_ntop(AF_INET6, &address, str, INET6_ADDRSTRLEN))
			{
				return std::string(str);
			}
			else
			{
				return { };
			}
		}

		/**
		 *
		 * 地址结构
		 *
		 * @param host 主机
		 * @param port 端口
		 *
		 * @return 地址结构
		 *
		 */
		struct sockaddr_in Address::AsAddressV4(const char * host, uint16_t port)
		{
			struct sockaddr_in address{ };

			address.sin_port = htons(port);
			address.sin_family = AF_INET;

			AsNetworkByteOrder(host, address.sin_addr);

			return address;
		}

		/**
		 *
		 * 地址结构
		 *
		 * @param host 主机
		 * @param port 端口
		 *
		 * @return 地址结构
		 *
		 */
		struct sockaddr_in6 Address::AsAddressV6(const char * host, uint16_t port)
		{
			struct sockaddr_in6 address{ };

			address.sin6_port = htons(port);
			address.sin6_family = AF_INET6;

			AsNetworkByteOrder(host, address.sin6_addr);

			return address;
		}
	}
}
