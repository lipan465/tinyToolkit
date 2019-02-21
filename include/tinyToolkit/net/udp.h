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
#include "session.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API UDPSessionPipe : public IUDPPipe
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
		UDPSessionPipe(int32_t managerSocket, int32_t sessionSocket, IUDPSession * session, NET_EVENT_TYPE netEventType);

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

		IUDPSession * _session{ nullptr };

		std::queue<NetEventPackage> _sendQueue{ };
	};

	class TINY_TOOLKIT_API UDPServerPipe : public IUDPPipe
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
		UDPServerPipe(int32_t managerSocket, int32_t sessionSocket, IUDPServer * server, NET_EVENT_TYPE netEventType);

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

		IUDPServer * _server{ nullptr };
	};
}


#endif // __TINY_TOOLKIT__NET__UDP__H__
