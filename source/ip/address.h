#ifndef __TINY_TOOLKIT__IP__ADDRESS__H__
#define __TINY_TOOLKIT__IP__ADDRESS__H__


/**
 *
 *  作者: hm
 *
 *  说明: 地址处理
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <vector>
#  include <string>
#
#  include <WS2tcpip.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <vector>
#  include <string>
#
#  include <netinet/in.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <vector>
#  include <string>
#
#  include <netinet/in.h>
#
#endif


namespace tinyToolkit
{
	namespace ip
	{
		class TINY_TOOLKIT_API Address
		{
		public:
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
			static bool ParseDomain(const char * domain, std::vector<std::string> & list);

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
			static bool AsHostByteOrder(const char * host, struct in_addr & address);

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
			static bool AsHostByteOrder(const char * host, struct in6_addr & address);

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
			static bool AsNetworkByteOrder(const char * host, struct in_addr & address);

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
			static bool AsNetworkByteOrder(const char * host, struct in6_addr & address);

			/**
			 *
			 * 转换字符串
			 *
			 * @param address 地址结构
			 *
			 * @return 字符串
			 *
			 */
			static std::string AsString(const struct in_addr & address);

			/**
			 *
			 * 转换字符串
			 *
			 * @param address 地址结构
			 *
			 * @return 字符串
			 *
			 */
			static std::string AsString(const struct in6_addr & address);

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
			static struct sockaddr_in AsAddressV4(const char * host, uint16_t port);

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
			static struct sockaddr_in6 AsAddressV6(const char * host, uint16_t port);
		};
	}
}


#endif // __TINY_TOOLKIT__IP__ADDRESS__H__
