#ifndef __TINY_TOOLKIT__NET__UDP__H__
#define __TINY_TOOLKIT__NET__UDP__H__


/**
 *
 *  作者: hm
 *
 *  说明: udp服务
 *
 */


#include "event.h"
#include "server.h"
#include "buffer.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API UDPSessionPipe : public IUDPPipe, public INetCompleter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param session 会话
		 * @param socket 会话套接字
		 * @param handle 管理句柄
		 *
		 */
		UDPSessionPipe(IUDPSession * session, TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_HANDLE handle);

		/**
		 *
		 * 析构函数
		 *
		 */
		~UDPSessionPipe() override = default;

		/**
		 *
		 * 关闭会话
		 *
		 */
		void Close() override;

		/**
		 *
		 * 发送数据
		 *
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 * @param cache 缓冲发送
		 *
		 */
		void Send(const void * value, std::size_t size, bool delay) override;

		/**
		 *
		 * 异步发送
		 *
		 * @return 是否处理成功
		 *
		 */
		bool AsyncSend() override;

		/**
		 *
		 * 异步连接
		 *
		 * @return 是否处理成功
		 *
		 */
		bool AsyncAccept() override;

		/**
		 *
		 * 异步接收
		 *
		 * @return 是否处理成功
		 *
		 */
		bool AsyncReceive() override;

		/**
		 *
		 * 回调函数
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void OnCallback(NetEvent * netEvent, void * sysEvent) override;

	protected:
		/**
		 *
		 * 交互处理
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void DoIO(NetEvent * netEvent, void * sysEvent);

		/**
		 *
		 * 交互处理
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void DoSend(NetEvent * netEvent, void * sysEvent);

		/**
		 *
		 * 交互处理
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void DoReceive(NetEvent * netEvent, void * sysEvent);

		/**
		 *
		 * 连接处理
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void DoConnect(NetEvent * netEvent, void * sysEvent);

	public:
		bool _isSend{ false };
		bool _isReceive{ false };
		bool _isConnect{ false };

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		NetEvent _sendEvent{ };
		NetEvent _receiveEvent{ };

#else

		NetEvent _netEvent{ };

#endif

	private:
		NetBuffer _sendBuffer;
		NetBuffer _receiveBuffer;

		IUDPSession * _session{ nullptr };

		TINY_TOOLKIT_SOCKET_TYPE _socket{ TINY_TOOLKIT_SOCKET_INVALID };

		TINY_TOOLKIT_SOCKET_HANDLE _handle{ TINY_TOOLKIT_SOCKET_HANDLE_INVALID };
	};

	class TINY_TOOLKIT_API UDPServerPipe : public IUDPPipe, public INetCompleter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param server 服务器
		 * @param socket 会话套接字
		 * @param handle 管理句柄
		 *
		 */
		UDPServerPipe(IUDPServer * server, TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_HANDLE handle);

		/**
		 *
		 * 析构函数
		 *
		 */
		~UDPServerPipe() override = default;

		/**
		 *
		 * 关闭会话
		 *
		 */
		void Close() override;

		/**
		 *
		 * 发送数据
		 *
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 * @param cache 缓冲发送
		 *
		 */
		void Send(const void * value, std::size_t size, bool cache) override;

		/**
		 *
		 * 异步发送
		 *
		 * @return 是否处理成功
		 *
		 */
		bool AsyncSend() override;

		/**
		 *
		 * 异步连接
		 *
		 * @return 是否处理成功
		 *
		 */
		bool AsyncAccept() override;

		/**
		 *
		 * 异步接收
		 *
		 * @return 是否处理成功
		 *
		 */
		bool AsyncReceive() override;

		/**
		 *
		 * 回调函数
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void OnCallback(NetEvent * netEvent, void * sysEvent) override;

	protected:
		/**
		 *
		 * 连接处理
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void DoAccept(NetEvent * netEvent, void * sysEvent);

	public:
		NetEvent _netEvent{ };

	private:
		IUDPServer * _server{ nullptr };

		TINY_TOOLKIT_SOCKET_TYPE _socket{ TINY_TOOLKIT_SOCKET_INVALID };

		TINY_TOOLKIT_SOCKET_HANDLE _handle{ TINY_TOOLKIT_SOCKET_HANDLE_INVALID };
	};
}


#endif // __TINY_TOOLKIT__NET__UDP__H__
