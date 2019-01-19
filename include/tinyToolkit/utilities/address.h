#ifndef __TINY_TOOLKIT__UTILITIES__ADDRESS__H__
#define __TINY_TOOLKIT__UTILITIES__ADDRESS__H__


/**
 *
 *  作者: hm
 *
 *  说明: 地址处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Address
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
		 * @param value 待转换字符串
		 *
		 * @return 网络字节序
		 *
		 */
		static uint32_t AsNetByte(const char * value);


		/**
		 *
		 * 转换主机字节序
		 *
		 * @param value 待转换字符串
		 *
		 * @return 主机字节序
		 *
		 */
		static uint32_t AsHostByte(const char * value);

		/**
		 *
		 * 转换网络字节序
		 *
		 * @param value 待转换字符串
		 *
		 * @return 网络字节序
		 *
		 */
		static uint32_t AsNetByte(const std::string & value);

		/**
		 *
		 * 转换主机字节序
		 *
		 * @param value 待转换字符串
		 *
		 * @return 主机字节序
		 *
		 */
		static uint32_t AsHostByte(const std::string & value);

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
		static bool AsNetByte(const std::string & value, uint32_t & head, uint32_t & tail);

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
		static bool AsHostByte(const std::string & value, uint32_t & head, uint32_t & tail);

		/**
		 *
		 * 解析域名
		 *
		 * @param host 待解析域名
		 *
		 * @return 解析后域名
		 *
		 */
		static std::string ParseHost(const char * host);

		/**
		 *
		 * 解析域名
		 *
		 * @param host 待解析域名
		 *
		 * @return 解析后域名
		 *
		 */
		static std::string ParseHost(const std::string & host);

		/**
		 *
		 * 转换为字符串
		 *
		 * @param value 待转换主机字节序
		 *
		 * @return 字符串
		 *
		 */
		static std::string AsString(uint32_t value);
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__ADDRESS__H__
