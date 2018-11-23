#ifndef __TINY_TOOLKIT__UTILITIES__IP__H__
#define __TINY_TOOLKIT__UTILITIES__IP__H__


/**
 *
 *  作者: hm
 *
 *  说明: ip处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API IP
	{
		/**
		 *
		 * ntohl(uint32_t __netlong)	网络字节序 --> 主机字节序
		 * htonl(uint32_t __hostlong)	主机字节序 --> 网络字节序
		 *
		 * ntohs(uint16_t __netshort)	网络字节序 --> 主机字节序
		 * htons(uint16_t __hostshort)	主机字节序 --> 网络字节序
		 *
		 * in_addr_t inet_addr(const char * __cp)		字符串 --> 网络字节序
		 * in_addr_t inet_network(const char * __cp)	字符串 --> 主机字节序
		 *
		 * char * inet_ntoa(struct in_addr __in)														网络字节序 --> 字符串
		 * char * inet_ntop(int af, const void * __restrict cp, char * __restrict buf, socklen_t len)	网络字节序 --> 字符串
		 *
		 * int inet_aton(const char * __cp, struct in_addr * __inp)						字符串 --> 网络字节序
		 * int inet_pton(int af, const char * __restrict cp, void * __restrict buf)		字符串 --> 网络字节序
		 *
		 */

	public:
		/**
		 *
		 * 转换网络字节序
		 *
		 * @param value 待转换主机字节序
		 *
		 * @return 网络字节序
		 *
		 */
		static uint32_t AsNetByte(uint32_t value)
		{
			return htonl(value);
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
		static uint32_t AsNetByte(const char * value)
		{
			return inet_addr(value);
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
		static uint32_t AsNetByte(const std::string & value)
		{
			return AsNetByte(value.c_str());
		}

		/**
		 *
		 * 转换主机字节序
		 *
		 * @param value 待转换网络字节序
		 *
		 * @return 主机字节序
		 *
		 */
		static uint32_t AsHostByte(uint32_t value)
		{
			return ntohl(value);
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
		static uint32_t AsHostByte(const char * value)
		{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			return ntohl(AsNetByte(value));

#else

			return inet_network(value);

#endif
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
		static uint32_t AsHostByte(const std::string & value)
		{
			return AsHostByte(value.c_str());
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
		static std::string AsString(uint32_t value)
		{
			return  std::to_string((value >> 24) & 0xFF) + "." +
					std::to_string((value >> 16) & 0xFF) + "." +
					std::to_string((value >>  8) & 0xFF) + "." +
					std::to_string((value >>  0) & 0xFF);
		}
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__IP__H__
