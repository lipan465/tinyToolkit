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
	class TINY_TOOLKIT_API UDPClientPipe : public IUDPPipe
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param managerSocket 管理句柄
		 * @param sessionSocket 会话句柄
		 * @param session 会话
		 * @param type 事件类型
		 *
		 */
		UDPClientPipe(int32_t managerSocket, int32_t sessionSocket, IUDPSession * session, EVENT_TYPE type);

		/**
		 *
		 * 析构函数
		 *
		 */
		~UDPClientPipe() override = default;

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
		 * @param host 待发送主机地址
		 * @param port 待发送主机端口
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 *
		 */
		void Send(const char * host, uint16_t port, const void * value, std::size_t size) override;

	protected:
		/**
		 *
		 * 回调函数
		 *
		 * @param currentEventValue 当前事件数据
		 * @param currentEvent 当前事件
		 *
		 */
		void OnCallBack(const EventValue * currentEventValue, const struct epoll_event & currentEvent);

	public:
		bool isConnect{ false };

		EventValue eventValue{ };

	protected:
		int32_t _managerSocket{ -1 };
		int32_t _sessionSocket{ -1 };

		IUDPSession * _session{ nullptr };

		std::queue<EventPackage> _sendQueue{ };
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
		 * @param session 会话
		 * @param type 事件类型
		 *
		 */
		UDPServerPipe(int32_t managerSocket, int32_t sessionSocket, IUDPServer * server, EVENT_TYPE type);

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
		 * @param host 待发送主机地址
		 * @param port 待发送主机端口
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 *
		 */
		void Send(const char * host, uint16_t port, const void * value, std::size_t size) override;

	protected:
		/**
		 *
		 * 回调函数
		 *
		 * @param currentEventValue 当前事件数据
		 * @param currentEvent 当前事件
		 *
		 */
		void OnCallBack(const EventValue * currentEventValue, const struct epoll_event & currentEvent);

	public:
		bool isConnect{ false };

		EventValue eventValue{ };

	protected:
		int32_t _managerSocket{ -1 };
		int32_t _sessionSocket{ -1 };

		IUDPServer * _server{ nullptr };

		std::queue<EventPackage> _sendQueue{ };
	};
}


#endif // __TINY_TOOLKIT__NET__UDP__H__
