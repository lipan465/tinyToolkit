/**
 *
 *  作者: hm
 *
 *  说明: 通讯服务器
 *
 */


#include "server.h"
#include "manager.h"

#include "../utilities/singleton.h"


namespace tinyToolkit
{
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

	/**
	 *
	 * 发送数据
	 *
	 * @param data 待发送数据指针
	 * @param size 待发送数据长度
	 *
	 */
	void ITCPServer::Send(const void * data, std::size_t size)
	{
		if (_pipe)
		{
			_pipe->Send(data, size);
		}
	}

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
	 * 主机端口
	 *
	 * @return 主机端口
	 *
	 */
	uint16_t ITCPServer::LocalPort() const
	{
		return _localPort;
	}

	/**
	 *
	 * 远端端口
	 *
	 * @return 远端端口
	 *
	 */
	uint16_t ITCPServer::RemotePort() const
	{
		return _remotePort;
	}

	/**
	 *
	 * 缓存大小
	 *
	 * @return 缓存大小
	 *
	 */
	std::size_t ITCPServer::CacheSize() const
	{
		return _cacheSize;
	}

	/**
	 *
	 * 主机地址
	 *
	 * @return 主机地址
	 *
	 */
	const std::string & ITCPServer::LocalHost() const
	{
		return _localHost;
	}

	/**
	 *
	 * 远端地址
	 *
	 * @return 远端地址
	 *
	 */
	const std::string & ITCPServer::RemoteHost() const
	{
		return _remoteHost;
	}
}
