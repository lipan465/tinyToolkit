/**
 *
 *  作者: hm
 *
 *  说明: tinyToolkit
 *
 */


#include "tinyToolkit.h"


namespace tinyToolkit
{
	/**
	 *
	 * 启动
	 *
	 * @param host 主机地址
	 * @param port 主机端口
	 *
	 * @return 是否启动成功
	 *
	 */
	bool ITCPServer::Launch(const std::string & host, uint16_t port)
	{
		return NetWorkManager::Instance().LaunchTCPServer(this, host, port);
	}

	/**
	 *
	 * 启动
	 *
	 * @param host 主机地址
	 * @param port 主机端口
	 *
	 * @return 是否启动成功
	 *
	 */
	bool ITCPSession::Launch(const std::string & host, uint16_t port)
	{
		return NetWorkManager::Instance().LaunchTCPClient(this, host, port);
	}
}