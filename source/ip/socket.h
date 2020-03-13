#ifndef __TINY_TOOLKIT__IP__SOCKET__H__
#define __TINY_TOOLKIT__IP__SOCKET__H__


/**
 *
 *  作者: hm
 *
 *  说明: 套接字处理
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <ctime>
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <ctime>
#  include <string>
#
#  include <netinet/in.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <ctime>
#  include <string>
#
#  include <netinet/in.h>
#
#endif


namespace tinyToolkit
{
	namespace ip
	{
		class TINY_TOOLKIT_API Socket
		{
		public:
			/**
			 *
			 * 是否有效
			 *
			 * @param socket 套接字
			 *
			 * @return 是否有效
			 *
			 */
			static bool IsValid(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * 是否有效
			 *
			 * @param socket 套接字
			 *
			 * @return 是否有效
			 *
			 */
			static bool IsValidPoll(TINY_TOOLKIT_SOCKET_POLL_TYPE socket);

			/**
			 *
			 * 关闭套接字
			 *
			 * @param socket 套接字
			 *
			 * @return 是否关闭成功
			 *
			 */
			static bool Close(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * 关闭轮询套接字
			 *
			 * @param socket 套接字
			 *
			 * @return 是否关闭成功
			 *
			 */
			static bool ClosePoll(TINY_TOOLKIT_SOCKET_POLL_TYPE socket);

			/**
			 *
			 * 获取目标地址
			 *
			 * @param socket 套接字
			 * @param address 地址信息
			 *
			 * @return 是否获取成功
			 *
			 */
			static bool GetPeerAddress(TINY_TOOLKIT_SOCKET_TYPE socket, struct sockaddr & address);

			/**
			 *
			 * 获取本地地址
			 *
			 * @param socket 套接字
			 * @param address 地址信息
			 *
			 * @return 是否获取成功
			 *
			 */
			static bool GetLocalAddress(TINY_TOOLKIT_SOCKET_TYPE socket, struct sockaddr & address);

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
			 * 设置是否堵塞
			 *
			 * @param socket 套接字
			 * @param on 状态
			 *
			 * @return 是否设置成功
			 *
			 */
			static bool SetBlock(TINY_TOOLKIT_SOCKET_TYPE socket, bool on);

			/**
			 *
			 * 设置是否开启Nagle算法
			 *
			 * @param socket 套接字
			 * @param on 状态
			 *
			 * @return 是否设置成功
			 *
			 */
			static bool SetDelay(TINY_TOOLKIT_SOCKET_TYPE socket, bool on);

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
			 * @param timeout 超时时间
			 *
			 * @return 是否设置成功
			 *
			 */
			static bool SetReceiveTimeout(TINY_TOOLKIT_SOCKET_TYPE socket, std::time_t timeout);

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
			 * 协议族
			 *
			 * @param socket 套接字
			 *
			 * @return 协议族
			 *
			 */
			static int32_t Family(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * 可读数据大小
			 *
			 * @param socket 套接字
			 *
			 * @return 可读数据大小
			 *
			 */
			static int32_t ReadableSize(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * 发送缓冲区大小
			 *
			 * @param socket 套接字
			 *
			 * @return 发送缓冲区大小
			 *
			 */
			static int32_t SendBufferSize(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * 接收缓冲区大小
			 *
			 * @param socket 套接字
			 *
			 * @return 接收缓冲区大小
			 *
			 */
			static int32_t ReceiveBufferSize(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * 对端端点
			 *
			 * @param socket 套接字
			 *
			 * @return 本地端点
			 *
			 */
			static std::pair<std::string, uint16_t> PeerEndpointV4(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * 对端端点
			 *
			 * @param socket 套接字
			 *
			 * @return 本地端点
			 *
			 */
			static std::pair<std::string, uint16_t> PeerEndpointV6(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * 本地端点
			 *
			 * @param socket 套接字
			 *
			 * @return 本地端点
			 *
			 */
			static std::pair<std::string, uint16_t> LocalEndpointV4(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * 本地端点
			 *
			 * @param socket 套接字
			 *
			 * @return 本地端点
			 *
			 */
			static std::pair<std::string, uint16_t> LocalEndpointV6(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * tcp套接字
			 *
			 * @param family 协议族
			 *
			 * @return tcp套接字
			 *
			 */
			static TINY_TOOLKIT_SOCKET_TYPE TCPSocket(int32_t family);

			/**
			 *
			 * tcp套接字
			 *
			 * @return tcp套接字
			 *
			 */
			static TINY_TOOLKIT_SOCKET_TYPE TCPSocketV4();

			/**
			 *
			 * tcp套接字
			 *
			 * @return tcp套接字
			 *
			 */
			static TINY_TOOLKIT_SOCKET_TYPE TCPSocketV6();

			/**
			 *
			 * udp套接字
			 *
			 * @param family 协议族
			 *
			 * @return udp套接字
			 *
			 */
			static TINY_TOOLKIT_SOCKET_TYPE UDPSocket(int32_t family);

			/**
			 *
			 * udp套接字
			 *
			 * @return udp套接字
			 *
			 */
			static TINY_TOOLKIT_SOCKET_TYPE UDPSocketV4();

			/**
			 *
			 * udp套接字
			 *
			 * @return udp套接字
			 *
			 */
			static TINY_TOOLKIT_SOCKET_TYPE UDPSocketV6();

			/**
			 *
			 * 轮询套接字
			 *
			 * @return 轮询套接字
			 *
			 */
			static TINY_TOOLKIT_SOCKET_POLL_TYPE PollSocket();
		};
	}
}


#endif // __TINY_TOOLKIT__IP__SOCKET__H__
