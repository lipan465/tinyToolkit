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
	 * @param host 主机地址
	 * @param port 主机端口
	 * @param sSize 发送缓冲区大小
	 * @param rSize 接受缓冲区大小
	 *
	 * @return 是否启动成功
	 *
	 */
	bool ITCPSession::Launch(const std::string & host, uint16_t port, std::size_t sSize, std::size_t rSize)
	{
		return NetWorkManager::Instance().LaunchTCPClient(this, host, port, sSize, rSize);
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
	 * 获取地址
	 *
	 */
	void ITCPSession::GetAddress()
	{
		if (_pipe)
		{
			if (_localPort == 0)
			{
				struct sockaddr_in address{ };

				if (Net::GetLocalAddress(_pipe->Socket(), address))
				{
					_localPort = ntohs(address.sin_port);
					_localHost = inet_ntoa(address.sin_addr);
				}
			}

			if (_remotePort == 0)
			{
				struct sockaddr_in address{ };

				if (Net::GetRemoteAddress(_pipe->Socket(), address))
				{
					_remotePort = ntohs(address.sin_port);
					_remoteHost = inet_ntoa(address.sin_addr);
				}
			}
		}
	}

	/**
	 *
	 * 发送数据
	 *
	 * @param value 待发送数据
	 * @param size 待发送数据长度
	 * @param delay 延迟发送
	 *
	 */
	void ITCPSession::Send(const void * value, std::size_t size, bool delay)
	{
		if (_pipe)
		{
			_pipe->Send(value, size, delay);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 启动
	 *
	 * @param host 主机地址
	 * @param port 主机端口
	 * @param sSize 发送缓冲区大小
	 * @param rSize 接受缓冲区大小
	 *
	 * @return 是否启动成功
	 *
	 */
	bool IUDPSession::Launch(const std::string & host, uint16_t port, std::size_t sSize, std::size_t rSize)
	{
		return NetWorkManager::Instance().LaunchUDPClient(this, host, port, sSize, rSize);
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
	 * 获取地址
	 *
	 */
	void IUDPSession::GetAddress()
	{
		if (_pipe)
		{
			if (_localPort == 0)
			{
				struct sockaddr_in address{ };

				if (Net::GetLocalAddress(_pipe->Socket(), address))
				{
					_localPort = ntohs(address.sin_port);
					_localHost = inet_ntoa(address.sin_addr);
				}
			}

			if (_remotePort == 0)
			{
				struct sockaddr_in address{ };

				if (Net::GetRemoteAddress(_pipe->Socket(), address))
				{
					_remotePort = ntohs(address.sin_port);
					_remoteHost = inet_ntoa(address.sin_addr);
				}
			}
		}
	}

	/**
	 *
	 * 发送数据
	 *
	 * @param value 待发送数据
	 * @param size 待发送数据长度
	 * @param delay 延迟发送
	 *
	 */
	void IUDPSession::Send(const void * value, std::size_t size, bool delay)
	{
		if (_pipe)
		{
			_pipe->Send(value, size, delay);
		}
	}
}
