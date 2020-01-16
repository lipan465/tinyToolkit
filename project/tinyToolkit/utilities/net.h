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
		 * 转换范围
		 *
		 * @param value 待转换字符串
		 * @param head 转换后范围首部
		 * @param tail 转换后范围尾部
		 *
		 * @return 是否转换成功
		 *
		 */
		static bool AsRange(const std::string & value, uint16_t & head, uint16_t & tail);

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
		 * 遍历域名对应的地址列表
		 *
		 * @param host 待解析域名
		 * @param list 地址列表
		 *
		 * @return 是否解析成功
		 *
		 */
		static bool TraverseAddressFromHost(const std::string & host, std::vector<std::string> & list);

		/**
		 *
		 * 关闭套接字
		 *
		 * @param socket 套接字
		 *
		 * @return 是否关闭成功
		 *
		 */
		static bool CloseSocket(TINY_TOOLKIT_SOCKET_TYPE socket);

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
		static bool GetPeerAddress(TINY_TOOLKIT_SOCKET_TYPE socket, struct sockaddr_in & address);

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
		static bool GetLocalAddress(TINY_TOOLKIT_SOCKET_TYPE socket, struct sockaddr_in & address);

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
		static bool SetLinger(TINY_TOOLKIT_SOCKET_TYPE socket, bool on, int32_t timeout);

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
		static bool SetNoBlock(TINY_TOOLKIT_SOCKET_TYPE socket, bool on);

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
		static bool SetNoDelay(TINY_TOOLKIT_SOCKET_TYPE socket, bool on);

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
		static bool SetReusePort(TINY_TOOLKIT_SOCKET_TYPE socket, bool on);

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
		static bool SetReuseAddress(TINY_TOOLKIT_SOCKET_TYPE socket, bool on);

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
		static bool SetSendTimeout(TINY_TOOLKIT_SOCKET_TYPE socket, std::time_t second);

		/**
		 *
		 * 设置接收超时时间
		 *
		 * @param socket 套接字
		 * @param second 超时秒数
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool SetReceiveTimeout(TINY_TOOLKIT_SOCKET_TYPE socket, std::time_t second);

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
		static bool SetSendBufferSize(TINY_TOOLKIT_SOCKET_TYPE socket, int32_t size);

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
		static bool SetReceiveBufferSize(TINY_TOOLKIT_SOCKET_TYPE socket, int32_t size);

		/**
		 *
		 * 获取协议族
		 *
		 * @param socket 套接字
		 *
		 * @return 协议族
		 *
		 */
		static int32_t GetFamily(TINY_TOOLKIT_SOCKET_TYPE socket);

		/**
		 *
		 * 获取发送缓冲区大小
		 *
		 * @param socket 套接字
		 *
		 * @return 发送缓冲区大小
		 *
		 */
		static int32_t GetSendBufferSize(TINY_TOOLKIT_SOCKET_TYPE socket);

		/**
		 *
		 * 获取接收缓冲区大小
		 *
		 * @param socket 套接字
		 *
		 * @return 接收缓冲区大小
		 *
		 */
		static int32_t GetReceiveBufferSize(TINY_TOOLKIT_SOCKET_TYPE socket);

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
		static uint32_t AsHostByte(const char * value);

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
