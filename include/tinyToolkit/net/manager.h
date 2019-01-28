#ifndef __TINY_TOOLKIT__NET__MANAGER__H__
#define __TINY_TOOLKIT__NET__MANAGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 管理器
 *
 */


#include "tcp.h"
#include "udp.h"

#include "../utilities/singleton.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetWorkManager
	{
		friend Singleton<NetWorkManager>;

	public:
		/**
		 *
		 * 获取单例
		 *
		 * @return 单例对象引用
		 *
		 */
		static NetWorkManager & Instance();

		/**
		 *
		 * 启动udp客户端
		 *
		 * @param client 客户端
		 * @param host 主机地址
		 * @param port 主机端口
		 *
		 * @return 是否启动成功
		 *
		 */
		bool LaunchUDPClient(IUDPSession * client, const std::string & host, uint16_t port);

		/**
		 *
		 * 启动udp服务器
		 *
		 * @param server 服务器
		 * @param host 主机地址
		 * @param port 主机端口
		 *
		 * @return 是否启动成功
		 *
		 */
		bool LaunchUDPServer(IUDPServer * server, const std::string & host, uint16_t port);

		/**
		 *
		 * 启动tcp客户端
		 *
		 * @param client 客户端
		 * @param host 主机地址
		 * @param port 主机端口
		 *
		 * @return 是否启动成功
		 *
		 */
		bool LaunchTCPClient(ITCPSession * client, const std::string & host, uint16_t port);

		/**
		 *
		 * 启动tcp服务器
		 *
		 * @param server 服务器
		 * @param host 主机地址
		 * @param port 主机端口
		 *
		 * @return 是否启动成功
		 *
		 */
		bool LaunchTCPServer(ITCPServer * server, const std::string & host, uint16_t port);

	protected:
		/**
		 *
		 * 构造函数
		 *
		 */
		NetWorkManager() = default;

		/**
		 *
		 * 析构函数
		 *
		 */
		~NetWorkManager();

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
		 * app线程逻辑函数
		 *
		 */
		void ThreadProcess();

	private:
		bool _status{ false };

		int32_t _socket{ -1 };

		std::thread _thread{ };
	};
}


#endif // __TINY_TOOLKIT__NET__MANAGER__H__
