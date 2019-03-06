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
#include "session.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TCPSessionPipe : public ITCPPipe
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param managerEvent 管理事件
		 * @param socket 会话句柄
		 * @param session 会话
		 * @param type 事件类型
		 *
		 */
		TCPSessionPipe(NetManagerEvent & managerEvent, int32_t socket, ITCPSession * session, NET_EVENT_TYPE type);

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
		 * 发送数据
		 *
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 *
		 */
		void Send(const void * value, std::size_t size) override;

	protected:
		/**
		 *
		 * 连接处理
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void DoConnect(const NetEvent * netEvent, const void * sysEvent);

		/**
		 *
		 * 交互处理
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void DoTransmit(const NetEvent * netEvent, const void * sysEvent);

		/**
		 *
		 * 回调函数
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void OnCallBack(const NetEvent * netEvent, const void * sysEvent);

	public:
		bool _isConnect{ false };

		NetEvent _netEvent{ };

	private:
		int32_t _socket{ -1 };

		ITCPSession * _session{ nullptr };

		NetManagerEvent & _managerEvent;

		std::queue<NetEventPackage> _sendQueue{ };
	};

	class TINY_TOOLKIT_API TCPServerPipe : public ITCPPipe
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param managerEvent 管理事件
		 * @param socket 会话句柄
		 * @param server 服务器
		 * @param type 事件类型
		 *
		 */
		TCPServerPipe(NetManagerEvent & managerEvent, int32_t socket, ITCPServer * server, NET_EVENT_TYPE type);

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
		 * 发送数据
		 *
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 *
		 */
		void Send(const void * value, std::size_t size) override;

	protected:
		/**
		 *
		 * 连接处理
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void DoAccept(const NetEvent * netEvent, const void * sysEvent);

		/**
		 *
		 * 回调函数
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		void OnCallBack(const NetEvent * netEvent, const void * sysEvent);

	public:
		NetEvent _netEvent{ };

	private:
		int32_t _socket{ -1 };

		ITCPServer * _server{ nullptr };

		NetManagerEvent & _managerEvent;
	};
}


#endif // __TINY_TOOLKIT__NET__TCP__H__
