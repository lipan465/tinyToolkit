/**
 *
 *  作者: hm
 *
 *  说明: 通讯会话
 *
 */


#include "session.h"
#include "manager.h"

#include "../utilities/net.h"


namespace tinyToolkit
{
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

	////////////////////////////////////////////////////////////////////////////////////////////////////


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
}
