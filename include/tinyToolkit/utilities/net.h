#ifndef __TINY_TOOLKIT__UTILITIES__NET__H__
#define __TINY_TOOLKIT__UTILITIES__NET__H__


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
	class TINY_TOOLKIT_API Net
	{
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
		 * 设置堆栈大小
		 *
		 * @param size 大小
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool SetStackSize(uint32_t size);

		/**
		 *
		 * 设置最大打开描诉符限制
		 *
		 * @param size 个数
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool SetMaxOpenFile(uint32_t size);

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
		static bool GetLocalName(int32_t socket, std::string & host, uint16_t & port);

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
		static bool GetRemoteName(int32_t socket, std::string & host, uint16_t & port);

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
		 * 启用端口复用
		 *
		 * @param socket 句柄
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool EnableReusePort(int32_t socket);

		/**
		 *
		 * 启用非堵塞
		 *
		 * @param socket 句柄
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool EnableNonBlocking(int32_t socket);

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
		 * 转换为字符串
		 *
		 * @param value 待转换主机字节序
		 *
		 * @return 字符串
		 *
		 */
		static std::string AsString(uint32_t value);

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
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__NET__H__
