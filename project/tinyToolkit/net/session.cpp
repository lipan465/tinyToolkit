/**
 *
 *  作者: hm
 *
 *  说明: 会话
 *
 */


#include "event.h"
#include "session.h"

#include "../utilities/net.h"
#include "../utilities/singleton.h"


namespace tinyToolkit
{
	/**
	 *
	 * 缓存堵塞触发回调函数
	 *
	 * @return 是否关闭连接
	 *
	 */
	bool INetSession::OnCacheFull()
	{
		return true;
	}

	/**
	 *
	 * 事件错误触发回调函数
	 *
	 */
	void INetSession::OnEventError()
	{

	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void INetSession::OnDisconnect()
	{

	}

	/**
	 *
	 * 发送成功触发回调函数
	 *
	 */
	void INetSession::OnSend()
	{

	}

	/**
	 *
	 * 发送失败触发回调函数
	 *
	 */
	void INetSession::OnSendFailed()
	{

	}

	/**
	 *
	 * 监听连接触发回调函数
	 *
	 */
	void INetSession::OnBind()
	{

	}

	/**
	 *
	 * 监听失败触发回调函数
	 *
	 */
	void INetSession::OnBindFailed()
	{

	}

	/**
	 *
	 * 套接字生成触发回调函数
	 *
	 */
	void INetSession::OnSocket()
	{

	}

	/**
	 *
	 * 套接字错误触发回调函数
	 *
	 */
	void INetSession::OnSocketFailed()
	{

	}

	/**
	 *
	 * 连接成功触发回调函数
	 *
	 */
	void INetSession::OnConnect()
	{

	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void INetSession::OnConnectFailed()
	{

	}

	/**
	 *
	 * 接收失败触发回调函数
	 *
	 */
	void INetSession::OnReceiveFailed()
	{

	}

	/**
	 *
	 * 接收数据触发回调函数
	 *
	 * @param buffer 数据缓冲区指针
	 * @param length 数据缓冲区长度
	 *
	 * @return 使用的字节数
	 *
	 */
	std::size_t INetSession::OnReceive(const char * buffer, std::size_t length)
	{
		(void)buffer;

		return length;
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void INetSession::Close()
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
	 * @param buffer 待发送数据缓冲区指针
	 * @param length 待发送数据缓冲区长度
	 *
	 * @return 是否发送成功
	 *
	 */
	bool INetSession::Send(const void * buffer, std::size_t length)
	{
		if (_pipe)
		{
			return _pipe->Append(buffer, length);
		}

		return false;
	}

	/**
	 *
	 * 剩余消息个数
	 *
	 * @return 剩余消息个数
	 *
	 */
	std::size_t INetSession::RemainMessageCount()
	{
		if (_pipe)
		{
			return _pipe->RemainMessageCount();
		}

		return 0;
	}

	/**
	 *
	 * 目标地址
	 *
	 * @return 目标地址
	 *
	 */
	const NetAddress & INetSession::PeerAddress()
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
	const NetAddress & INetSession::LocalAddress()
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
	 * @param peerHost 目标地址
	 * @param peerPort 目标端口
	 * @param cacheSize 缓存大小
	 * @param eventMonitor 事件监控
	 *
	 * @return 是否启动成功
	 *
	 */
	bool ITCPSession::Launch(std::string peerHost, uint16_t peerPort, std::size_t cacheSize, NetEventMonitor * eventMonitor)
	{
		_cacheSize = cacheSize;

		_peerAddress.port = peerPort;
		_peerAddress.host = std::move(peerHost);

		if (eventMonitor)
		{
			return eventMonitor->LaunchTCPSession(this);
		}
		else
		{
			return Singleton<NetEventMonitor>::Instance().LaunchTCPSession(this);
		}
	}
}
