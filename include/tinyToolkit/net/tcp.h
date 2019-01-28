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
	class TINY_TOOLKIT_API TCPClientPipe : public ITCPPipe
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
		TCPClientPipe(int32_t managerSocket, int32_t sessionSocket, ITCPSession * session, EVENT_TYPE type);

		/**
		 *
		 * 析构函数
		 *
		 */
		~TCPClientPipe() override = default;

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

		ITCPSession * _session{ nullptr };

		std::queue<EventPackage> _sendQueue{ };
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
		 * @param session 会话
		 * @param type 事件类型
		 *
		 */
		TCPServerPipe(int32_t managerSocket, int32_t sessionSocket, ITCPServer * server, EVENT_TYPE type);

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
		 * 回调函数
		 *
		 * @param currentEventValue 当前事件数据
		 * @param currentEvent 当前事件
		 *
		 */
		void OnCallBack(const EventValue * currentEventValue, const struct epoll_event & currentEvent);

	public:
		EventValue eventValue{ };

	protected:
		int32_t _managerSocket{ -1 };
		int32_t _sessionSocket{ -1 };

		ITCPServer * _server{ nullptr };
	};
}


#endif // __TINY_TOOLKIT__NET__TCP__H__
