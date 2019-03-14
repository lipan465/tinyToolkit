#ifndef __TINY_TOOLKIT__NET__TCP__H__
#define __TINY_TOOLKIT__NET__TCP__H__


/**
 *
 *  作者: hm
 *
 *  说明: tcp服务
 *
 */


#include "event.h"
#include "server.h"
#include "buffer.h"
#include "session.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TCPSessionPipe : public ITCPPipe, public INetCompleter
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
		TCPSessionPipe(ITCPSession * session, TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_HANDLE handle);

		/**
		 *
		 * 析构函数
		 *
		 */
		~TCPSessionPipe() override = default;

		/**
		 *
		 * 套接字
		 *
		 * @return 套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE Socket() override;

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
		 * @param delay 延迟发送
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

		ITCPSession * _session{ nullptr };

		TINY_TOOLKIT_SOCKET_TYPE _socket{ TINY_TOOLKIT_SOCKET_INVALID };

		TINY_TOOLKIT_SOCKET_HANDLE _handle{ TINY_TOOLKIT_SOCKET_HANDLE_INVALID };
	};

	class TINY_TOOLKIT_API TCPServerPipe : public ITCPPipe, public INetCompleter
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
		TCPServerPipe(ITCPServer * server, TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_HANDLE handle);

		/**
		 *
		 * 析构函数
		 *
		 */
		~TCPServerPipe() override = default;

		/**
		 *
		 * 套接字
		 *
		 * @return 套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE Socket() override;

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
		 * @param delay 延迟发送
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
		ITCPServer * _server{ nullptr };

		TINY_TOOLKIT_SOCKET_TYPE _socket{ TINY_TOOLKIT_SOCKET_INVALID };

		TINY_TOOLKIT_SOCKET_HANDLE _handle{ TINY_TOOLKIT_SOCKET_HANDLE_INVALID };
	};
}


#endif // __TINY_TOOLKIT__NET__TCP__H__
