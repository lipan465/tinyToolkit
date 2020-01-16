#ifndef __TINY_TOOLKIT__NET__EVENT__H__
#define __TINY_TOOLKIT__NET__EVENT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 事件
 *
 */


#include "context.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ITCPServer;
	class TINY_TOOLKIT_API ITCPSession;

	class TINY_TOOLKIT_API NetEvent
	{
		friend class NetEventMonitor;

	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		NetEvent();

		/**
		 *
		 * 析构函数
		 *
		 */
		~NetEvent();

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
		 * 移除监听事件
		 *
		 * @param socket 句柄
		 * @param context 上下文
		 *
		 * @return 是否移除成功
		 *
		 */
		bool Remove(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context);

		/**
		 *
		 * 修改监听事件
		 *
		 * @param socket 句柄
		 * @param context 上下文
		 * @param readFlag 读监听
		 * @param writeFlag 写监听
		 *
		 * @return 是否修改成功
		 *
		 */
		bool Modify(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context, bool readFlag, bool writeFlag);

		/**
		 *
		 * 添加监听事件
		 *
		 * @param socket 句柄
		 * @param context 上下文
		 * @param readFlag 读监听
		 * @param writeFlag 写监听
		 *
		 * @return 是否添加成功
		 *
		 */
		bool Append(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context, bool readFlag, bool writeFlag);

	private:
		TINY_TOOLKIT_SOCKET_EVENT_TYPE _socket{ TINY_TOOLKIT_SOCKET_EVENT_INVALID };
	};

	class TINY_TOOLKIT_API NetEventMonitor
	{
		friend class TINY_TOOLKIT_API ITCPServer;
		friend class TINY_TOOLKIT_API ITCPSession;

	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		NetEventMonitor();

		/**
		 *
		 * 析构函数
		 *
		 */
		~NetEventMonitor();

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
		 * 移除事件监控
		 *
		 * @param socket 句柄
		 * @param context 上下文
		 *
		 * @return 是否移除成功
		 *
		 */
		bool RemoveEvent(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context);

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
		bool ModifyEvent(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context, bool readFlag, bool writeFlag);

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
		bool AppendEvent(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context, bool readFlag, bool writeFlag);

	private:
		/**
		 *
		 * 启动tcp服务
		 *
		 * @param server 服务
		 *
		 * @return 是否启动成功
		 *
		 */
		bool LaunchTCPServer(ITCPServer * server);

		/**
		 *
		 * 启动tcp会话
		 *
		 * @param session 会话
		 *
		 * @return 是否启动成功
		 *
		 */
		bool LaunchTCPSession(ITCPSession * session);

	private:
		bool _status{ true };

		NetEvent _event{ };

		std::mutex _mutex{ };

		std::thread _thread{ };
	};
}


#endif // __TINY_TOOLKIT__NET__EVENT__H__
