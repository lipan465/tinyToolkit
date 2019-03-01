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
		 * @param managerSocket 管理句柄
		 * @param sessionSocket 会话句柄
		 * @param session 会话
		 * @param netEventType 事件类型
		 *
		 */
		TCPSessionPipe(int32_t managerSocket, int32_t sessionSocket, ITCPSession * session, NET_EVENT_TYPE netEventType);

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
		 * @param sysEvent 系统事件
		 *
		 */
		void DoConnect(const void * sysEvent);

		/**
		 *
		 * 交互处理
		 *
		 * @param sysEvent 系统事件
		 *
		 */
		void DoTransmit(const void * sysEvent);

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
		int32_t _managerSocket{ -1 };
		int32_t _sessionSocket{ -1 };

		ITCPSession * _session{ nullptr };

		std::queue<NetEventPackage> _sendQueue{ };
	};

	class TINY_TOOLKIT_API TCPServerPipe : public ITCPPipe
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param managerSocket 管理句柄
		 * @param sessionSocket 会话句柄
		 * @param server 服务器
		 * @param netEventType 事件类型
		 *
		 */
		TCPServerPipe(int32_t managerSocket, int32_t sessionSocket, ITCPServer * server, NET_EVENT_TYPE netEventType);

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
		 * @param sysEvent 系统事件
		 *
		 */
		void DoAccept(const void * sysEvent);

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
		int32_t _managerSocket{ -1 };
		int32_t _sessionSocket{ -1 };

		ITCPServer * _server{ nullptr };
	};
}


#endif // __TINY_TOOLKIT__NET__TCP__H__
