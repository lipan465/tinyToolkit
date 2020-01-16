#ifndef __TINY_TOOLKIT__NET__TCP__H__
#define __TINY_TOOLKIT__NET__TCP__H__


/**
 *
 *  作者: hm
 *
 *  说明: tcp
 *
 */


#include "cache.h"
#include "event.h"
#include "server.h"
#include "message.h"
#include "session.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TCPSessionPipe : public INetPipe, public INetCompleter
	{
		friend class TCPServerPipe;
		friend class NetEventMonitor;

	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param socket 句柄
		 * @param session 会话
		 * @param monitor 监控
		 *
		 */
		TCPSessionPipe(std::shared_ptr<NetSocket> socket, ITCPSession * session, NetEventMonitor * monitor);

		/**
		 *
		 * 析构函数
		 *
		 */
		~TCPSessionPipe() override = default;

		/**
		 *
		 * 关闭会话
		 *
		 */
		void Close() override;

		/**
		 *
		 * 接受连接
		 *
		 * @return 是否连接成功
		 *
		 */
		bool Accept() override;

		/**
		 *
		 * 接收数据
		 *
		 * @return 是否接收成功
		 *
		 */
		bool Receive() override;

		/**
		 *
		 * 发送数据
		 *
		 * @param buffer 待发送数据缓冲区指针
		 * @param length 待发送数据缓冲区长度
		 *
		 * @return 是否发送成功
		 *
		 */
		bool Send(const char * buffer, std::size_t length) override;

		/**
		 *
		 * 追加数据
		 *
		 * @param buffer 待追加数据缓冲区指针
		 * @param length 待追加数据缓冲区长度
		 *
		 * @return 是否追加成功
		 *
		 */
		bool Append(const void * buffer, std::size_t length) override;

		/**
		 *
		 * 逻辑处理
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void Logic(NetContext * netContext, void * sysContext) override;

		/**
		 *
		 * 剩余消息个数
		 *
		 * @return 剩余消息个数
		 *
		 */
		std::size_t RemainMessageCount() override;

		/**
		 *
		 * 句柄
		 *
		 * @return 句柄
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE SocketHandle() override;

	private:
		/**
		 *
		 * 交互处理
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void DoIO(NetContext * netContext, void * sysContext);

		/**
		 *
		 * 发送处理
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void DoSend(NetContext * netContext, void * sysContext);

		/**
		 *
		 * 接收处理
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void DoReceive(NetContext * netContext, void * sysContext);

		/**
		 *
		 * 连接处理
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void DoConnect(NetContext * netContext, void * sysContext);

	private:
		bool _isSend{ false };
		bool _isConnect{ false };

		std::mutex _mutex{ };

		NetCache _cache;

		std::shared_ptr<NetSocket> _socket{ };

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		NetContext _sendContext{ };
		NetContext _receiveContext{ };

#else

		NetContext _ioContext{ };

#endif

		ITCPSession * _session{ nullptr };

		NetEventMonitor * _monitor{ nullptr };

		std::queue<std::shared_ptr<NetMessage>> _messageQueue{ };
	};

	class TINY_TOOLKIT_API TCPServerPipe : public INetPipe, public INetCompleter
	{
		friend class NetEventMonitor;

	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param socket 句柄
		 * @param server 服务
		 * @param monitor 监控
		 *
		 */
		TCPServerPipe(std::shared_ptr<NetSocket> socket, ITCPServer * server, NetEventMonitor * monitor);

		/**
		 *
		 * 析构函数
		 *
		 */
		~TCPServerPipe() override = default;

		/**
		 *
		 * 关闭会话
		 *
		 */
		void Close() override;

		/**
		 *
		 * 接受连接
		 *
		 * @return 是否连接成功
		 *
		 */
		bool Accept() override;

		/**
		 *
		 * 接收数据
		 *
		 * @return 是否接收成功
		 *
		 */
		bool Receive() override;

		/**
		 *
		 * 发送数据
		 *
		 * @param buffer 待发送数据缓冲区指针
		 * @param length 待发送数据缓冲区长度
		 *
		 * @return 是否发送成功
		 *
		 */
		bool Send(const char * buffer, std::size_t length) override;

		/**
		 *
		 * 追加数据
		 *
		 * @param buffer 待追加数据缓冲区指针
		 * @param length 待追加数据缓冲区长度
		 *
		 * @return 是否追加成功
		 *
		 */
		bool Append(const void * buffer, std::size_t length) override;

		/**
		 *
		 * 逻辑处理
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void Logic(NetContext * netContext, void * sysContext) override;

		/**
		 *
		 * 剩余消息个数
		 *
		 * @return 剩余消息个数
		 *
		 */
		std::size_t RemainMessageCount() override;

		/**
		 *
		 * 句柄
		 *
		 * @return 句柄
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE SocketHandle() override;

	private:
		/**
		 *
		 * 连接处理
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void DoAccept(NetContext * netContext, void * sysContext);

	private:
		bool _isListen{ true };

		NetContext _acceptContext{ };

		ITCPServer * _server{ nullptr };

		NetEventMonitor * _monitor{ nullptr };

		std::shared_ptr<NetSocket> _socket{ };
	};
}




#endif // __TINY_TOOLKIT__NET__TCP__H__
