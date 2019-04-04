/**
 *
 *  作者: hm
 *
 *  说明: udp服务
 *
 */


#include "udp.h"

#include "../debug/trace.h"
#include "../utilities/net.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param session 会话
	 * @param socket 会话套接字
	 * @param handle 管理句柄
	 *
	 */
	UDPSessionPipe::UDPSessionPipe(IUDPSession * session, TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_HANDLE handle) : _session(session),
																																_socket(socket),
																																_handle(handle)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		_sendEvent._type = NET_EVENT_TYPE::SEND;
		_sendEvent._socket = socket;
		_sendEvent._completer = this;

		_receiveEvent._type = NET_EVENT_TYPE::RECEIVE;
		_receiveEvent._socket = socket;
		_receiveEvent._completer = this;

#else

		_netEvent._type = NET_EVENT_TYPE::IO;
		_netEvent._socket = socket;
		_netEvent._completer = this;

#endif
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void UDPSessionPipe::Close()
	{
		if (_socket != TINY_TOOLKIT_SOCKET_INVALID)
		{
			_isConnect = false;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], _socket, EVFILT_READ,  EV_DELETE, 0, 0, nullptr);
			EV_SET(&event[1], _socket, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

			kevent(_handle, event, 2, nullptr, 0, nullptr);

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			epoll_ctl(_handle, EPOLL_CTL_DEL, _socket, nullptr);

#endif

			Net::CloseSocket(_socket);

			_socket = TINY_TOOLKIT_SOCKET_INVALID;

			if (_session)
			{
				_session->OnDisconnect();
			}
		}
	}

	/**
	 *
	 * 发送数据
	 *
	 * @param ip 远端地址
	 * @param port 远端端口
	 * @param data 待发送数据
	 * @param size 待发送数据长度
	 *
	 */
	void UDPSessionPipe::Send(const char * ip, uint16_t port, const void * data, std::size_t size)
	{
		if (size == 0)
		{
			return;
		}

		if (!_isConnect)
		{
			return;
		}

		_sendQueue.push(new NetMessage(ip, port, data, size));

		if (_isSend)
		{
			return;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (!AsyncSend())
		{
			Close();
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		struct kevent event[2]{ };

		EV_SET(&event[0], _socket, EVFILT_READ,  EV_ENABLE, 0, 0, &_netEvent);
		EV_SET(&event[1], _socket, EVFILT_WRITE, EV_ENABLE, 0, 0, &_netEvent);

		if (kevent(_handle, event, 2, nullptr, 0, nullptr) == -1)
		{
			Close();
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		struct epoll_event event{ };

		event.events = EPOLLIN | EPOLLOUT;
		event.data.ptr = &_netEvent;

		if (epoll_ctl(_handle, EPOLL_CTL_MOD, _socket, &event) == -1)
		{
			Close();
		}

#endif
	}

	/**
	 *
	 * 异步发送
	 *
	 * @return 是否处理成功
	 *
	 */
	bool UDPSessionPipe::AsyncSend()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		DWORD flag = 0;
		DWORD bytes = 0;

		auto value = _sendQueue.front();

		memset(&_sendEvent._overlap, 0, sizeof(OVERLAPPED));

		_sendEvent._buffer.len = value->_size;
		_sendEvent._buffer.buf = value->_data;

		_sendEvent._address.sin_port = htons(value->_port);
		_sendEvent._address.sin_family = AF_INET;
		_sendEvent._address.sin_addr.s_addr = htonl(value->_ip);

		if (WSASendTo(_socket, &_sendEvent._buffer, 1, &bytes, flag, (struct sockaddr *)&_sendEvent._address, sizeof(struct sockaddr_in), (LPWSAOVERLAPPED)&_sendEvent, NULL) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				return false;
			}
		}

#endif

		_isSend = true;

		return true;
	}

	/**
	 *
	 * 异步接收
	 *
	 * @return 是否处理成功
	 *
	 */
	bool UDPSessionPipe::AsyncReceive()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		DWORD flag = 0;
		DWORD bytes = 0;

		auto addressLength = sizeof(struct sockaddr_in);

		memset(&_receiveEvent._overlap, 0, sizeof(OVERLAPPED));

		_receiveEvent._buffer.len = sizeof(_receiveEvent._temp);
		_receiveEvent._buffer.buf = _receiveEvent._temp;

		if (WSARecvFrom(_socket, &_receiveEvent._buffer, 1, &bytes, &flag, (struct sockaddr*)&_receiveEvent._address, &addressLength, (LPWSAOVERLAPPED)&_receiveEvent, NULL) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				return false;
			}
		}

#endif

		_isReceive = true;

		return true;
	}

	/**
	 *
	 * 回调函数
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void UDPSessionPipe::OnCallback(NetEvent * netEvent, void * sysEvent)
	{
		switch (netEvent->_type)
		{
			case NET_EVENT_TYPE::IO:
			{
				DoIO(netEvent, sysEvent);

				break;
			}

			case NET_EVENT_TYPE::SEND:
			{
				DoSend(netEvent, sysEvent);

				break;
			}

			case NET_EVENT_TYPE::CONNECT:
			{
				DoConnect(netEvent, sysEvent);

				break;
			}

			case NET_EVENT_TYPE::RECEIVE:
			{
				DoReceive(netEvent, sysEvent);

				break;
			}

			default:
			{
				TINY_TOOLKIT_ASSERT(false, "UDPSessionPipe type error : {}", netEvent->_socket);

				break;
			}
		}
	}

	/**
	 *
	 * 交互处理
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void UDPSessionPipe::DoIO(NetEvent * netEvent, void * sysEvent)
	{
		(void)netEvent;
		(void)sysEvent;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto currentEvent = reinterpret_cast<const struct kevent *>(sysEvent);

		if (currentEvent->flags & EV_ERROR)
		{
			Close();

			return;
		}

		if (currentEvent->filter == EVFILT_READ)
		{
			static char temp[TINY_TOOLKIT_SOCKET_TEMP_SIZE]{ 0 };

			if (_isConnect)
			{
				_isReceive = true;

				auto addressLength = sizeof(struct sockaddr_in);

				auto len = ::recvfrom(_socket, temp, sizeof(temp), MSG_DONTWAIT, (struct sockaddr *)&netEvent->_address, (socklen_t *)&addressLength);

				_isReceive = false;

				if (len <= 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					if (_session)
					{
						_session->OnReceive(inet_ntoa(netEvent->_address.sin_addr), ntohs(netEvent->_address.sin_port), temp, static_cast<size_t>(len));
					}
				}
				else
				{
					Close();

					return;
				}
			}
		}

		if (currentEvent->filter == EVFILT_WRITE)
		{
			if (_isConnect)
			{
				_isSend = true;

				auto value = _sendQueue.front();

				netEvent->_address.sin_port = htons(value->_port);
				netEvent->_address.sin_family = AF_INET;
				netEvent->_address.sin_addr.s_addr = htonl(value->_ip);

				auto len = ::sendto(_socket, value->_data, value->_size, 0, (struct sockaddr *)&netEvent->_address, sizeof(struct sockaddr_in));

				_isSend = false;

				delete value;

				_sendQueue.pop();

				if (len > 0)
				{
					if (_sendQueue.empty())
					{
						struct kevent event[2]{ };

						EV_SET(&event[0], _socket, EVFILT_READ, EV_ENABLE, 0, 0, &_netEvent);
						EV_SET(&event[1], _socket, EVFILT_WRITE, EV_DISABLE, 0, 0, &_netEvent);

						if (kevent(_handle, event, 2, nullptr, 0, nullptr) == -1)
						{
							Close();

							return;
						}
					}
				}
				else if (len <= 0 && errno != EAGAIN)
				{
					Close();

					return;
				}
			}
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		auto currentEvent = reinterpret_cast<const struct epoll_event *>(sysEvent);

		if (currentEvent->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
		{
			Close();

			return;
		}

		if (currentEvent->events & EPOLLIN)
		{
			static char temp[TINY_TOOLKIT_SOCKET_TEMP_SIZE]{ 0 };

			if (_isConnect)
			{
				_isReceive = true;

				auto addressLength = sizeof(struct sockaddr_in);

				auto len = ::recvfrom(_socket, temp, sizeof(temp), MSG_DONTWAIT, (struct sockaddr *)&netEvent->_address, (socklen_t *)&addressLength);

				_isReceive = false;

				if (len <= 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					if (_session)
					{
						_session->OnReceive(inet_ntoa(netEvent->_address.sin_addr), ntohs(netEvent->_address.sin_port), temp, static_cast<size_t>(len));
					}
				}
				else
				{
					Close();

					return;
				}
			}
		}

		if (currentEvent->events & EPOLLOUT)
		{
			if (_isConnect)
			{
				_isSend = true;

				auto value = _sendQueue.front();

				netEvent->_address.sin_port = htons(value->_port);
				netEvent->_address.sin_family = AF_INET;
				netEvent->_address.sin_addr.s_addr = htonl(value->_ip);

				auto len = ::sendto(_socket, value->_data, value->_size, 0, (struct sockaddr *)&netEvent->_address, sizeof(struct sockaddr_in));

				_isSend = false;

				delete value;

				_sendQueue.pop();

				if (len > 0)
				{
					if (_sendQueue.empty())
					{
						struct epoll_event event{ };

						event.events = EPOLLIN;
						event.data.ptr = &_netEvent;

						if (epoll_ctl(_handle, EPOLL_CTL_MOD, _socket, &event) == -1)
						{
							Close();

							return;
						}
					}
				}
				else if (len <= 0 && errno != EAGAIN)
				{
					Close();

					return;
				}
			}
		}

#endif
	}

	/**
	 *
	 * 发送处理
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void UDPSessionPipe::DoSend(NetEvent * netEvent, void * sysEvent)
	{
		(void)netEvent;
		(void)sysEvent;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		_isSend = false;

		delete _sendQueue.front();

		_sendQueue.pop();

		if (_sendQueue.size() > 0)
		{
			if (!AsyncSend())
			{
				Close();
			}
		}

#endif
	}

	/**
	 *
	 * 交互处理
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void UDPSessionPipe::DoReceive(NetEvent * netEvent, void * sysEvent)
	{
		(void)netEvent;
		(void)sysEvent;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		_isReceive = false;

		if (netEvent->_bytes > 0)
		{
			if (_session)
			{
				_session->OnReceive(inet_ntoa(netEvent->_address.sin_addr), ntohs(netEvent->_address.sin_port), netEvent->_temp, netEvent->_bytes);
			}

			if (!AsyncReceive())
			{
				Close();
			}
		}
		else
		{
			Close();
		}

#endif
	}

	/**
	 *
	 * 连接处理
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void UDPSessionPipe::DoConnect(NetEvent * netEvent, void * sysEvent)
	{
		(void)netEvent;
		(void)sysEvent;

		delete netEvent;
	}
}
