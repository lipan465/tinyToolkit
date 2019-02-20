/**
 *
 *  作者: hm
 *
 *  说明: tcp服务
 *
 */


#include "tcp.h"

#include "../debug/trace.h"
#include "../utilities/net.h"
#include "../utilities/time.h"
#include "../utilities/operator.h"


namespace tinyToolkit
{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	/// todo

#else

	/**
	*
	* 启用Nagle算法
	*
	* @param socket 句柄
	*
	* @return 是否设置成功
	*
	*/
	static bool EnableNoDelay(int32_t socket)
	{
		int32_t val = 1l;

		return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char *>(&val), sizeof(val)) == 0;
	}

	/**
	*
	* 启用非堵塞
	*
	* @param socket 句柄
	*
	* @return 是否设置成功
	*
	*/
	static bool EnableNonBlock(int32_t socket)
	{
		return fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK) == 0;
	}

	/**
	*
	* 启用端口复用
	*
	* @param socket 句柄
	*
	* @return 是否设置成功
	*
	*/
	static bool EnableReusePort(int32_t socket)
	{
		int32_t val = 1l;

		return setsockopt(socket, SOL_SOCKET, SO_REUSEPORT, reinterpret_cast<const char *>(&val), sizeof(val)) == 0;
	}

	/**
	*
	* 启用地址复用
	*
	* @param socket 句柄
	*
	* @return 是否设置成功
	*
	*/
	static bool EnableReuseAddress(int32_t socket)
	{
		int32_t val = 1l;

		return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&val), sizeof(val)) == 0;
	}

#endif

	/**
	 *
	 * 构造函数
	 *
	 * @param managerSocket 管理句柄
	 * @param sessionSocket 会话句柄
	 * @param session 会话
	 * @param netEventType 事件类型
	 *
	 */
	TCPSessionPipe::TCPSessionPipe(int32_t managerSocket, int32_t sessionSocket, ITCPSession * session, NET_EVENT_TYPE netEventType) : _managerSocket(managerSocket),
																																	   _sessionSocket(sessionSocket),
																																	   _session(session)
	{
		_netEvent._type = netEventType;
		_netEvent._socket = sessionSocket;
		_netEvent._callback = std::bind(&TCPSessionPipe::OnCallBack, this, std::placeholders::_1, std::placeholders::_2);
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void TCPSessionPipe::Close()
	{
		if (_sessionSocket != -1)
		{
			_isConnect = false;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], _sessionSocket, EVFILT_READ,  EV_DELETE, 0, 0, nullptr);
			EV_SET(&event[1], _sessionSocket, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

			kevent(_managerSocket, event, 2, nullptr, 0, nullptr);

#else

			epoll_ctl(_managerSocket, EPOLL_CTL_DEL, _sessionSocket, nullptr);

#endif

			::close(_sessionSocket);

			_sessionSocket = -1;

			if (_session)
			{
				_session->OnDisconnect();
			}

			Operator::Clear(_sendQueue);
		}
	}

	/**
	 *
	 * 发送数据
	 *
	 * @param value 待发送数据
	 * @param size 待发送数据长度
	 *
	 */
	void TCPSessionPipe::Send(const void * value, std::size_t size)
	{
		if (_isConnect)
		{
			_sendQueue.emplace(_session->_localHost.c_str(), _session->_localPort, value, size);

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], _sessionSocket, EVFILT_READ,  EV_ENABLE, 0, 0, (void *)&_netEvent);
			EV_SET(&event[1], _sessionSocket, EVFILT_WRITE, EV_ENABLE, 0, 0, (void *)&_netEvent);

			if (kevent(_managerSocket, event, 2, nullptr, 0, nullptr) == -1)
			{
				Close();
			}

#else

			struct epoll_event event{ };

			event.events = EPOLLIN | EPOLLOUT;
			event.data.ptr = &_netEvent;

			if (epoll_ctl(_managerSocket, EPOLL_CTL_MOD, _sessionSocket, &event) == -1)
			{
				Close();
			}

#endif
		}
	}

	/**
	 *
	 * 连接处理
	 *
	 * @param sysEvent 系统事件
	 *
	 */
	void TCPSessionPipe::DoConnect(const void * sysEvent)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto * currentEventPtr = reinterpret_cast<const struct kevent *>(sysEvent);

		{
			struct kevent event[2]{ };

			EV_SET(&event[0], _sessionSocket, EVFILT_READ,  EV_DELETE, 0, 0, nullptr);
			EV_SET(&event[1], _sessionSocket, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

			kevent(_managerSocket, event, 2, nullptr, 0, nullptr);
		}

		if (currentEventPtr->flags & EV_ERROR)
		{
			Close();

			if (_session)
			{
				_session->OnConnectFailed();
			}

			return;
		}

		if (currentEventPtr->filter == EVFILT_WRITE)
		{
			struct kevent event[2]{ };

			EV_SET(&event[0], _sessionSocket, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&_netEvent);
			EV_SET(&event[1], _sessionSocket, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&_netEvent);

			if (kevent(_managerSocket, event, 2, nullptr, 0, nullptr) == -1)
			{
				Close();

				if (_session)
				{
					_session->OnConnectFailed();
				}
			}
			else
			{
				_isConnect = true;

				_netEvent._type = NET_EVENT_TYPE::TRANSMIT;

				if (_session)
				{
					_session->OnConnect();
				}
			}
		}
		else
		{
			Close();

			if (_session)
			{
				_session->OnConnectFailed();
			}
		}

#else

		auto currentEvent = reinterpret_cast<const struct epoll_event *>(sysEvent);

		epoll_ctl(_managerSocket, EPOLL_CTL_DEL, _sessionSocket, nullptr);

		if (currentEvent->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
		{
			Close();

			if (_session)
			{
				_session->OnConnectFailed();
			}

			return;
		}

		if (currentEvent->events & EPOLLOUT)
		{
			struct epoll_event event{ };

			event.events = EPOLLIN;
			event.data.ptr = &_netEvent;

			if (epoll_ctl(_managerSocket, EPOLL_CTL_ADD, _sessionSocket, &event) == -1)
			{
				Close();

				if (_session)
				{
					_session->OnConnectFailed();
				}
			}
			else
			{
				_isConnect = true;

				_netEvent._type = NET_EVENT_TYPE::TRANSMIT;

				if (_session)
				{
					_session->OnConnect();
				}
			}
		}
		else
		{
			Close();

			if (_session)
			{
				_session->OnConnectFailed();
			}
		}

#endif
	}

	/**
	 *
	 * 交互处理
	 *
	 * @param sysEvent 系统事件
	 *
	 */
	void TCPSessionPipe::DoTransmit(const void * sysEvent)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto * currentEventPtr = reinterpret_cast<const struct kevent *>(sysEvent);

		if (currentEventPtr->flags & EV_ERROR)
		{
			Close();

			return;
		}

		if (currentEventPtr->filter == EVFILT_READ)
		{
			auto tick = Time::Microseconds();

			static char buffer[TINY_TOOLKIT_MB]{ 0 };

			while (_isConnect && Time::Microseconds() - tick <= 1000)
			{
				auto len = ::recv(_sessionSocket, buffer, sizeof(buffer), 0);

				if (len < 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					buffer[len] = '\0';

					if (_session)
					{
						_session->OnReceive(buffer, static_cast<std::size_t>(len));
					}
				}
				else
				{
					Close();

					return;
				}
			}
		}

		if (currentEventPtr->filter == EVFILT_WRITE)
		{
			if (!_sendQueue.empty())
			{
				std::size_t count = 0;

				std::size_t length = _sendQueue.front()._value.size();

				const char * value = _sendQueue.front()._value.c_str();

				while (_isConnect && count < length)
				{
					auto len = ::send(_sessionSocket, value + count, length - count, 0);

					if (len > 0)
					{
						count += len;

						if (count == length)
						{
							_sendQueue.pop();

							if (_sendQueue.empty())
							{
								struct kevent event[2]{ };

								EV_SET(&event[0], _sessionSocket, EVFILT_READ,  EV_ENABLE,  0, 0, (void *)&_netEvent);
								EV_SET(&event[1], _sessionSocket, EVFILT_WRITE, EV_DISABLE, 0, 0, (void *)&_netEvent);

								if (kevent(_managerSocket, event, 2, nullptr, 0, nullptr) == -1)
								{
									Close();

									return;
								}
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
		}

#else

		auto currentEvent = reinterpret_cast<const struct epoll_event *>(sysEvent);

		if (currentEvent->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
		{
			Close();

			return;
		}

		if (currentEvent->events & EPOLLIN)
		{
			auto tick = Time::Microseconds();

			static char buffer[TINY_TOOLKIT_MB]{ 0 };

			while (_isConnect && Time::Microseconds() - tick <= 1000)
			{
				auto len = ::recv(_sessionSocket, buffer, sizeof(buffer), 0);

				if (len < 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					buffer[len] = '\0';

					if (_session)
					{
						_session->OnReceive(buffer, static_cast<std::size_t>(len));
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
			if (!_sendQueue.empty())
			{
				std::size_t count = 0;

				std::size_t length = _sendQueue.front()._value.size();

				const char * value = _sendQueue.front()._value.c_str();

				while (_isConnect && count < length)
				{
					auto len = ::send(_sessionSocket, value + count, length - count, 0);

					if (len > 0)
					{
						count += len;

						if (count == length)
						{
							_sendQueue.pop();

							if (_sendQueue.empty())
							{
								struct epoll_event event{ };

								event.events = EPOLLIN;
								event.data.ptr = &_netEvent;

								if (epoll_ctl(_managerSocket, EPOLL_CTL_MOD, _sessionSocket, &event) == -1)
								{
									Close();

									return;
								}
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
		}

#endif
	}

	/**
	 *
	 * 回调函数
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void TCPSessionPipe::OnCallBack(const NetEvent * netEvent, const void * sysEvent)
	{
		switch(netEvent->_type)
		{
			case NET_EVENT_TYPE::CONNECT:
			{
				DoConnect(sysEvent);

				break;
			}

			case NET_EVENT_TYPE::TRANSMIT:
			{
				DoTransmit(sysEvent);

				break;
			}

			default:
			{
				TINY_TOOLKIT_ASSERT(false, "TCPSessionPipe type error : {}", netEvent->_socket)

				break;
			}
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 构造函数
	 *
	 * @param managerSocket 管理句柄
	 * @param sessionSocket 会话句柄
	 * @param server 服务器
	 * @param netEventType 事件类型
	 *
	 */
	TCPServerPipe::TCPServerPipe(int32_t managerSocket, int32_t sessionSocket, ITCPServer * server, NET_EVENT_TYPE netEventType) : _managerSocket(managerSocket),
																																   _sessionSocket(sessionSocket),
																																   _server(server)
	{
		_netEvent._type = netEventType;
		_netEvent._socket = sessionSocket;
		_netEvent._callback = std::bind(&TCPServerPipe::OnCallBack, this, std::placeholders::_1, std::placeholders::_2);
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void TCPServerPipe::Close()
	{
		if (_sessionSocket != -1)
		{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], _sessionSocket, EVFILT_READ,  EV_DELETE, 0, 0, nullptr);
			EV_SET(&event[1], _sessionSocket, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

			kevent(_managerSocket, event, 2, nullptr, 0, nullptr);

#else

			epoll_ctl(_managerSocket, EPOLL_CTL_DEL, _sessionSocket, nullptr);

#endif

			::close(_sessionSocket);

			_sessionSocket = -1;

			if (_server)
			{
				_server->OnRelease();
			}
		}
	}

	/**
	 *
	 * 发送数据
	 *
	 * @param value 待发送数据
	 * @param size 待发送数据长度
	 *
	 */
	void TCPServerPipe::Send(const void * value, std::size_t size)
	{
		(void)size;
		(void)value;
	}

	/**
	 *
	 * 连接处理
	 *
	 * @param sysEvent 系统事件
	 *
	 */
	void TCPServerPipe::DoAccept(const void * sysEvent)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto currentEvent = reinterpret_cast<const struct kevent *>(sysEvent);

		if (currentEvent->filter == EVFILT_READ)
		{
			struct sockaddr_in clientAddress{ };

			std::size_t addressLen = sizeof(clientAddress);

			int32_t sock = ::accept(_sessionSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLen);

			if (sock >= 0)
			{
				if (!EnableNoDelay(sock) ||
					!EnableNonBlock(sock) ||
					!EnableReusePort(sock) ||
					!EnableReuseAddress(sock))
				{
					::close(sock);

					_server->OnError();

					return;
				}

				uint16_t localPort = _server->_port;
				uint16_t remotePort = ntohs(clientAddress.sin_port);

				std::string localHost = _server->_host;
				std::string remoteHost = inet_ntoa(clientAddress.sin_addr);

				auto session = _server->OnNewConnect(remoteHost, remotePort);

				if (session)
				{
					session->_localPort = localPort;
					session->_localHost = localHost;

					session->_remotePort = remotePort;
					session->_remoteHost = remoteHost;

					auto pipe = std::make_shared<TCPSessionPipe>(_managerSocket, sock, session, NET_EVENT_TYPE::TRANSMIT);

					struct kevent event[2]{ };

					EV_SET(&event[0], _sessionSocket, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&_netEvent);
					EV_SET(&event[1], _sessionSocket, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&_netEvent);

					if (kevent(_managerSocket, event, 2, nullptr, 0, nullptr) == -1)
					{
						::close(sock);

						_server->OnSessionError(session);
					}
					else
					{
						pipe->_isConnect = true;

						session->_pipe = pipe;

						session->OnConnect();
					}
				}
				else
				{
					_server->OnSessionError(session);
				}
			}
			else
			{
				_server->OnError();
			}
		}

#else

		auto currentEvent = reinterpret_cast<const struct epoll_event *>(sysEvent);

		if (currentEvent->events & EPOLLIN)
		{
			struct sockaddr_in clientAddress{ };

			std::size_t addressLen = sizeof(struct sockaddr_in);

			int32_t sock = ::accept(_sessionSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLen);

			if (sock >= 0)
			{
				if (!EnableNoDelay(sock) ||
					!EnableNonBlock(sock) ||
					!EnableReusePort(sock) ||
					!EnableReuseAddress(sock))
				{
					::close(sock);

					_server->OnError();

					return;
				}

				uint16_t localPort = _server->_port;
				uint16_t remotePort = ntohs(clientAddress.sin_port);

				std::string localHost = _server->_host;
				std::string remoteHost = inet_ntoa(clientAddress.sin_addr);

				auto session = _server->OnNewConnect(remoteHost, remotePort);

				if (session)
				{
					session->_localPort = localPort;
					session->_localHost = localHost;

					session->_remotePort = remotePort;
					session->_remoteHost = remoteHost;

					auto pipe = std::make_shared<TCPSessionPipe>(_managerSocket, sock, session, NET_EVENT_TYPE::TRANSMIT);

					struct epoll_event event{ };

					event.events = EPOLLIN;
					event.data.ptr = &pipe->_netEvent;

					if (epoll_ctl(_managerSocket, EPOLL_CTL_ADD, sock, &event) == -1)
					{
						::close(sock);

						_server->OnSessionError(session);
					}
					else
					{
						pipe->_isConnect = true;

						session->_pipe = pipe;

						session->OnConnect();
					}
				}
				else
				{
					_server->OnSessionError(session);
				}
			}
			else
			{
				_server->OnError();
			}
		}

#endif
	}

	/**
	 *
	 * 回调函数
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void TCPServerPipe::OnCallBack(const NetEvent * netEvent, const void * sysEvent)
	{
		switch (netEvent->_type)
		{
			case NET_EVENT_TYPE::ACCEPT:
			{
				DoAccept(sysEvent);

				break;
			}

			default:
			{
				TINY_TOOLKIT_ASSERT(false, "TCPServerPipe type error : {}", netEvent->_socket)

				break;
			}
		}
	}
}
