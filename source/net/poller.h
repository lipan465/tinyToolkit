#ifndef __TINY_TOOLKIT__NET__POLLER__H__
#define __TINY_TOOLKIT__NET__POLLER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 轮询器
 *
 */


#include "context.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <mutex>
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <thread>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <mutex>
#  include <thread>
#  include <string>
#  include <cstdint>
#
#endif


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API ITCPServer;
		class TINY_TOOLKIT_API IUDPServer;
		class TINY_TOOLKIT_API ITCPSession;
		class TINY_TOOLKIT_API IUDPSession;

		class TINY_TOOLKIT_API Poller
		{
			friend class TCPServerChannel;
			friend class UDPServerChannel;
			friend class TCPSessionChannel;
			friend class UDPSessionChannel;

		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			Poller();

			/**
			 *
			 * 析构函数
			 *
			 */
			~Poller();

			/**
			 *
			 * 是否有效
			 *
			 * @return 是否有效
			 *
			 */
			bool IsValid();

			/**
			 *
			 * 启动tcp服务
			 *
			 * @param server 服务
			 * @param host 本地地址
			 * @param port 本地端口
			 *
			 * @return 是否启动成功
			 *
			 */
			bool LaunchTCPServer(ITCPServer * server, std::string host, uint16_t port);

			/**
			 *
			 * 启动udp服务
			 *
			 * @param server 服务
			 * @param host 本地地址
			 * @param port 本地端口
			 *
			 * @return 是否启动成功
			 *
			 */
			bool LaunchUDPServer(IUDPServer * server, std::string host, uint16_t port);

			/**
			 *
			 * 启动tcp会话
			 *
			 * @param session 会话
			 * @param host 目标地址
			 * @param port 目标端口
			 *
			 * @return 是否启动成功
			 *
			 */
			bool LaunchTCPSession(ITCPSession * session, std::string host, uint16_t port);

			/**
			 *
			 * 启动udp会话
			 *
			 * @param session 会话
			 * @param host 目标地址
			 * @param port 目标端口
			 *
			 * @return 是否启动成功
			 *
			 */
			bool LaunchUDPSession(IUDPSession * session, std::string host, uint16_t port);

		private:
			/**
			 *
			 * 移除事件监控
			 *
			 * @param socket 句柄
			 * @param context 上下文
			 *
			 * @return 是否移除成功
			 *
			 */
			bool RemoveEvent(TINY_TOOLKIT_SOCKET_TYPE socket, Context * context);

			/**
			 *
			 * 修改事件监控
			 *
			 * @param socket 句柄
			 * @param context 上下文
			 * @param readFlag 读监听
			 * @param writeFlag 写监听
			 *
			 * @return 是否修改成功
			 *
			 */
			bool ModifyEvent(TINY_TOOLKIT_SOCKET_TYPE socket, Context * context, bool readFlag, bool writeFlag);

			/**
			 *
			 * 添加事件监控
			 *
			 * @param socket 句柄
			 * @param context 上下文
			 * @param readFlag 读监听
			 * @param writeFlag 写监听
			 *
			 * @return 是否添加成功
			 *
			 */
			bool AppendEvent(TINY_TOOLKIT_SOCKET_TYPE socket, Context * context, bool readFlag, bool writeFlag);

		private:
			bool _status{ true };

			std::mutex _mutex{ };

			std::thread _thread{ };

			TINY_TOOLKIT_SOCKET_POLL_TYPE _socket{ TINY_TOOLKIT_SOCKET_POLL_INVALID };
		};
	}
}


#endif // __TINY_TOOLKIT__NET__POLLER__H__
