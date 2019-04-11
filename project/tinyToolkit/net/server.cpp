/**
 *
 *  作者: hm
 *
 *  说明: 通讯服务器
 *
 */


#include "server.h"
#include "manager.h"

#include "../utilities/net.h"


namespace tinyToolkit
{
	/**
	 *
	 * 启动
	 *
	 * @param localHost 主机地址
	 * @param localPort 主机端口
	 * @param cacheSize 缓存大小
	 *
	 * @return 是否启动成功
	 *
	 */
	bool ITCPServer::Launch(std::string localHost, uint16_t localPort, std::size_t cacheSize)
	{
		_cacheSize = cacheSize;

		_localPort = localPort;
		_localHost = std::move(localHost);

		return Singleton<NetManager>::Instance().LaunchTCPServer(this);
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void ITCPServer::Close()
	{
		if (_pipe)
		{
			_pipe->Close();
		}
	}
}
