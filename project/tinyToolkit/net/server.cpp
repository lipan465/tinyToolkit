/**
 *
 *  作者: hm
 *
 *  说明: 服务
 *
 */


#include "event.h"
#include "server.h"

#include "../utilities/net.h"
#include "../utilities/singleton.h"


namespace tinyToolkit
{
	/**
	 *
	 * 事件错误触发回调函数
	 *
	 */
	void INetServer::OnEventError()
	{

	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void INetServer::OnDisconnect()
	{

	}

	/**
	 *
	 * 监听连接触发回调函数
	 *
	 */
	void INetServer::OnBind()
	{

	}

	/**
	 *
	 * 监听失败触发回调函数
	 *
	 */
	void INetServer::OnBindFailed()
	{

	}

	/**
	 *
	 * 套接字生成触发回调函数
	 *
	 */
	void INetServer::OnSocket()
	{

	}

	/**
	 *
	 * 套接字错误触发回调函数
	 *
	 */
	void INetServer::OnSocketFailed()
	{

	}

	/**
	 *
	 * 监听连接触发回调函数
	 *
	 */
	void INetServer::OnListen()
	{

	}

	/**
	 *
	 * 监听失败触发回调函数
	 *
	 */
	void INetServer::OnListenFailed()
	{

	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void INetServer::OnAcceptFailed()
	{

	}

	/**
	 *
	 * 会话连接触发回调函数
	 *
	 * @return 会话
	 *
	 */
	tinyToolkit::ITCPSession * INetServer::OnAccept()
	{
		return nullptr;
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void INetServer::Close()
	{
		if (_pipe)
		{
			_pipe->Close();
		}
	}

	/**
	 *
	 * 目标地址
	 *
	 * @return 目标地址
	 *
	 */
	const NetAddress & INetServer::PeerAddress()
	{
		if (_pipe && !_peerAddress.IsValid())
		{
			struct sockaddr_in address{ };

			if (Net::GetPeerAddress(_pipe->SocketHandle(), address))
			{
				char addr[INET_ADDRSTRLEN]{ 0 };

				_peerAddress.host = inet_ntop(AF_INET, reinterpret_cast<const char *>(&address.sin_addr), addr, static_cast<socklen_t>(sizeof(addr)));
				_peerAddress.port = ntohs(address.sin_port);
			}
		}

		return _peerAddress;
	}

	/**
	 *
	 * 本地地址
	 *
	 * @return 本地地址
	 *
	 */
	const NetAddress & INetServer::LocalAddress()
	{
		if (_pipe && !_localAddress.IsValid())
		{
			struct sockaddr_in address{ };

			if (Net::GetLocalAddress(_pipe->SocketHandle(), address))
			{
				char addr[INET_ADDRSTRLEN]{ 0 };

				_localAddress.host = inet_ntop(AF_INET, reinterpret_cast<const char *>(&address.sin_addr), addr, static_cast<socklen_t>(sizeof(addr)));
				_localAddress.port = ntohs(address.sin_port);
			}
		}

		return _localAddress;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 启动
	 *
	 * @param localHost 本地地址
	 * @param localPort 本地端口
	 * @param cacheSize 缓存大小
	 * @param eventMonitor 事件监控
	 *
	 * @return 是否启动成功
	 *
	 */
	bool ITCPServer::Launch(std::string localHost, uint16_t localPort, std::size_t cacheSize, NetEventMonitor * eventMonitor)
	{
		_cacheSize = cacheSize;

		_localAddress.port = localPort;
		_localAddress.host = std::move(localHost);

		if (eventMonitor)
		{
			return eventMonitor->LaunchTCPServer(this);
		}
		else
		{
			return Singleton<NetEventMonitor>::Instance().LaunchTCPServer(this);
		}
	}
}
