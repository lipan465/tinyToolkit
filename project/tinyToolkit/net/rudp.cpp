/**
 *
 *  作者: hm
 *
 *  说明: 可靠udp
 *
 */


#include "rudp.h"

#include "../utilities/time.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param session 会话
	 *
	 */
	RUDPSessionPipe::RUDPSessionPipe(IRUDPSession * session) : _cache(session->_cacheSize), _session(session)
	{

	}

	/**
	 *
	 * 启动
	 *
	 * @return 是否启动成功
	 *
	 */
	bool RUDPSessionPipe::Launch()
	{
		if (!_status)
		{
			_udpSession = std::make_shared<UDPClientSession>(this);

			if (_udpSession->Launch(_session->_localHost, _session->_localPort, _session->_remoteHost, _session->_remotePort, _session->_cacheSize))
			{
				_session->_localHost = _udpSession->LocalHost();
				_session->_localPort = _udpSession->LocalPort();

				_session->_remoteHost = _udpSession->RemoteHost();
				_session->_remotePort = _udpSession->RemotePort();
			}
			else
			{
				return false;
			}

			_status = true;
			_isConnect = false;
			_isConnectStage = true;

			_thread = std::thread(&RUDPSessionPipe::ThreadProcess, this);
		}

		return _status;
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void RUDPSessionPipe::Close()
	{
		if (_status)
		{
			_status = false;

			if (_thread.joinable())
			{
				_thread.join();
			}
		}

		if (_isConnect)
		{
			_isConnect = false;

			if (_kcp)
			{
				ikcp_release(_kcp);

				_kcp = nullptr;
			}
		}

		_udpSession->Close();
	}

	/**
	 *
	 * 发送数据
	 *
	 * @param data 待发送数据指针
	 * @param size 待发送数据长度
	 *
	 */
	void RUDPSessionPipe::Send(const void * data, std::size_t size)
	{
		if (!_isConnect)
		{
			return;
		}

		if (data == nullptr || size == 0)
		{
			return;
		}

		_sendQueue.Push(std::make_shared<NetMessage>(data, size));
	}

	/**
	 *
	 * app线程逻辑函数
	 *
	 */
	void RUDPSessionPipe::ThreadProcess()
	{
		auto lastConnectTime = Time::Milliseconds();
		auto firstConnectTime = Time::Milliseconds();

		while (_status)
		{
			TINY_TOOLKIT_SLEEP_MS(5);

			auto currentTime = Time::Milliseconds();

			if (_isConnectStage)
			{
				if (currentTime - firstConnectTime >= TINY_TOOLKIT_SOCKET_CONNECT_TIMEOUT)
				{
					_isConnectStage = false;

					_udpSession->OnConnectFailed();

					return;
				}

				if (currentTime - lastConnectTime >= TINY_TOOLKIT_SOCKET_CONNECT_INTERVAL)
				{
					_udpSession->Send(TINY_TOOLKIT_REQUEST_CONNECT_UUID_MESSAGE, sizeof(TINY_TOOLKIT_REQUEST_CONNECT_UUID_MESSAGE));

					lastConnectTime = currentTime;
				}
			}

			if (_isConnect)
			{
				ikcp_update(_kcp, currentTime);

				if (!_sendQueue.Empty())
				{
					auto value = _sendQueue.Front();

					if (ikcp_send(_kcp, value->_data, value->_size) < 0)
					{
						_isConnect = false;

						_udpSession->OnDisconnect();

						return;
					}

					_sendQueue.Pop();
				}
			}
		}
	}

	/**
	 *
	 * kcp输出函数
	 *
	 * @param data 待发送的数据指针
	 * @param size 待发送的数据长度
	 * @param kcp 对象
	 * @param user 用户数据
	 *
	 * @return 调用状态
	 *
	 */
	int32_t RUDPSessionPipe::Output(const char * data, int32_t size, ikcpcb * kcp, void * user)
	{
		(void)kcp;

		reinterpret_cast<RUDPSessionPipe *>(user)->_udpSession->Send(data, size);

		return size;
	}
}