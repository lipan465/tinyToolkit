#ifndef __TINY_TOOLKIT__NET__MANAGER__H__
#define __TINY_TOOLKIT__NET__MANAGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯管理器
 *
 */


#include "tcp.h"
#include "udp.h"

#include "../utilities/copyable.h"
#include "../utilities/singleton.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetManager : public NonCopyable
	{
	public:

		/**
		 *
		 * 构造函数
		 *
		 */
		NetManager() = default;

		/**
		 *
		 * 析构函数
		 *
		 */
		~NetManager() override;

		/**
		 *
		 * 启动
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch();

		/**
		 *
		 * 启动udp客户端
		 *
		 * @param client 客户端
		 *
		 * @return 是否启动成功
		 *
		 */
		bool LaunchUDPClient(IUDPSession * client);

		/**
		 *
		 * 启动tcp客户端
		 *
		 * @param client 客户端
		 *
		 * @return 是否启动成功
		 *
		 */
		bool LaunchTCPClient(ITCPSession * client);

		/**
		 *
		 * 启动tcp服务器
		 *
		 * @param server 服务器
		 *
		 * @return 是否启动成功
		 *
		 */
		bool LaunchTCPServer(ITCPServer * server);

	private:
		/**
		 *
		 * app线程逻辑函数
		 *
		 */
		void ThreadProcess();

	private:
		bool _status{ false };

		std::thread _thread{ };

		TINY_TOOLKIT_SOCKET_HANDLE _handle{ TINY_TOOLKIT_SOCKET_HANDLE_INVALID };
	};
}


#endif // __TINY_TOOLKIT__NET__MANAGER__H__
