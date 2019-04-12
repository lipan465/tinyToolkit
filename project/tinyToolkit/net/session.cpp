/**
 *
 *  作者: hm
 *
 *  说明: 通讯会话
 *
 */


#include "session.h"
#include "manager.h"

#include "../utilities/singleton.h"


namespace tinyToolkit
{
	/**
	 *
	 * 关闭会话
	 *
	 */
	void ITCPSession::Close()
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
	void ITCPSession::Send(const void * data, std::size_t size)
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
	 * @param remoteHost 远端地址
	 * @param remotePort 远端端口
	 * @param cacheSize 缓存大小
	 *
	 * @return 是否启动成功
	 *
	 */
	bool ITCPSession::Launch(std::string remoteHost, uint16_t remotePort, std::size_t cacheSize)
	{
		_cacheSize = cacheSize;

		_remotePort = remotePort;
		_remoteHost = std::move(remoteHost);

		return Singleton<NetManager>::Instance().LaunchTCPClient(this);
	}

	/**
	 *
	 * 主机端口
	 *
	 * @return 主机端口
	 *
	 */
	uint16_t ITCPSession::LocalPort() const
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
	uint16_t ITCPSession::RemotePort() const
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
	std::size_t ITCPSession::CacheSize() const
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
	const std::string & ITCPSession::LocalHost() const
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
	const std::string & ITCPSession::RemoteHost() const
	{
		return _remoteHost;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 关闭会话
	 *
	 */
	void IUDPSession::Close()
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
	void IUDPSession::Send(const void * data, std::size_t size)
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
	 * @param remoteHost 远端地址
	 * @param remotePort 远端端口
	 * @param cacheSize 缓存大小
	 *
	 * @return 是否启动成功
	 *
	 */
	bool IUDPSession::Launch(std::string localHost, uint16_t localPort, std::string remoteHost, uint16_t remotePort, std::size_t cacheSize)
	{
		_cacheSize = cacheSize;

		_localPort = localPort;
		_localHost = std::move(localHost);

		_remotePort = remotePort;
		_remoteHost = std::move(remoteHost);

		return Singleton<NetManager>::Instance().LaunchUDPClient(this);
	}

	/**
	 *
	 * 主机端口
	 *
	 * @return 主机端口
	 *
	 */
	uint16_t IUDPSession::LocalPort() const
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
	uint16_t IUDPSession::RemotePort() const
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
	std::size_t IUDPSession::CacheSize() const
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
	const std::string & IUDPSession::LocalHost() const
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
	const std::string & IUDPSession::RemoteHost() const
	{
		return _remoteHost;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 关闭会话
	 *
	 */
	void IRUDPSession::Close()
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
	void IRUDPSession::Send(const void * data, std::size_t size)
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
	 * @param remoteHost 远端地址
	 * @param remotePort 远端端口
	 * @param cacheSize 缓存大小
	 *
	 * @return 是否启动成功
	 *
	 */
	bool IRUDPSession::Launch(std::string localHost, uint16_t localPort, std::string remoteHost, uint16_t remotePort, std::size_t cacheSize)
	{
		_cacheSize = cacheSize;

		_localPort = localPort;
		_localHost = std::move(localHost);

		_remotePort = remotePort;
		_remoteHost = std::move(remoteHost);

		return Singleton<NetManager>::Instance().LaunchRUDPClient(this);
	}

	/**
	 *
	 * 主机端口
	 *
	 * @return 主机端口
	 *
	 */
	uint16_t IRUDPSession::LocalPort() const
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
	uint16_t IRUDPSession::RemotePort() const
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
	std::size_t IRUDPSession::CacheSize() const
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
	const std::string & IRUDPSession::LocalHost() const
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
	const std::string & IRUDPSession::RemoteHost() const
	{
		return _remoteHost;
	}
}
