#ifndef __TINY_TOOLKIT__UTILITIES__NET__H__
#define __TINY_TOOLKIT__UTILITIES__NET__H__


/**
 *
 *  作者: hm
 *
 *  说明: 网络处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Net
	{
	public:
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
		 * 遍历域名对应的地址列表
		 *
		 * @param host 待解析域名
		 * @param list 地址列表
		 *
		 * @return 是否解析成功
		 *
		 */
		static bool TraverseAddressFromHost(const char * host, std::vector<std::string> & list);

		/**
		 *
		 * 启用Nagle算法
		 *
		 * @param socket 句柄
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool EnableNoDelay(int32_t socket);

		/**
		 *
		 * 启用非堵塞
		 *
		 * @param socket 句柄
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool EnableNonBlock(int32_t socket);

		/**
		 *
		 * 启用地址复用
		 *
		 * @param socket 句柄
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool EnableReuseAddress(int32_t socket);

		/**
		 *
		 * 设置发送缓冲区大小
		 *
		 * @param sock 句柄
		 * @param size 缓冲区大小
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool SetSendBufferSize(int32_t sock, int32_t size);

		/**
		 *
		 * 设置接收缓冲区大小
		 *
		 * @param sock 句柄
		 * @param size 缓冲区大小
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool SetReceiveBufferSize(int32_t sock, int32_t size);

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


#endif // __TINY_TOOLKIT__UTILITIES__NET__H__
