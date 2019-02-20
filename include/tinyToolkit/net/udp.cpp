/**
 *
 *  作者: hm
 *
 *  说明: udp服务
 *
 */


#include "udp.h"

#include "../system/os.h"
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
	 * 会话处理
	 *
	 * @param socket 句柄
	 * @param address 地址
	 * @param addressLen 地址长度
	 *
	 * @return 新生成的句柄
	 *
	 */
	static int32_t Accept(int32_t socket, struct sockaddr * address, socklen_t * addressLen)
	{
		static char buffer[6]{ 0 };

		while (true)
		{
			auto len = ::recvfrom(socket, buffer, sizeof(buffer), 0, address, addressLen);

			if (len < 0 && errno == EAGAIN)
			{
				continue;
			}
			else if (len > 0)
			{
				return ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			}
			else
			{
				return -1;
			}
		}
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
	UDPSessionPipe::UDPSessionPipe(int32_t managerSocket, int32_t sessionSocket, IUDPSession * session, NET_EVENT_TYPE netEventType) : _managerSocket(managerSocket),
																																	   _sessionSocket(sessionSocket),
																																	   _session(session)
	{
		_netEvent._type = netEventType;
		_netEvent._socket = sessionSocket;
		_netEvent._callback = std::bind(&UDPSessionPipe::OnCallBack, this, std::placeholders::_1, std::placeholders::_2);
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void UDPSessionPipe::Close()
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
	 * @param host 待发送主机地址
	 * @param port 待发送主机端口
	 * @param value 待发送数据
	 * @param size 待发送数据长度
	 *
	 */
	void UDPSessionPipe::Send(const char * host, uint16_t port, const void * value, std::size_t size)
	{
		if (_isConnect)
		{
			_sendQueue.emplace(host, port, value, size);

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
	void UDPSessionPipe::DoConnect(const void * sysEvent)
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
	void UDPSessionPipe::DoTransmit(const void * sysEvent)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto currentEvent = reinterpret_cast<const struct kevent *>(sysEvent);

		if (currentEvent->flags & EV_ERROR)
		{
			Close();

			return;
		}

		if (currentEvent->filter == EVFILT_READ)
		{
			auto tick = Time::Microseconds();

			static char buffer[TINY_TOOLKIT_MB]{ 0 };

			while (_isConnect && Time::Microseconds() - tick <= 1000)
			{
				struct sockaddr_in address{ };

				std::size_t addressLen = sizeof(struct sockaddr);

				address.sin_port = htons(_session->_localPort);
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = Net::AsNetByte(_session->_localHost.c_str());

				auto len = ::recvfrom(_sessionSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, (socklen_t *)&addressLen);

				if (len < 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					buffer[len] = '\0';

					if (_session)
					{
						_session->OnReceive(inet_ntoa(address.sin_addr), ntohs(address.sin_port), buffer, static_cast<std::size_t>(len));
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
			if (!_sendQueue.empty())
			{
				std::size_t count = 0;

				std::size_t length = _sendQueue.front()._value.size();

				const char * value = _sendQueue.front()._value.c_str();

				struct sockaddr_in address{ };

				address.sin_port = htons(_sendQueue.front()._port);
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = Net::AsNetByte(_sendQueue.front()._host.c_str());

				while (_isConnect && count < length)
				{
					auto len = ::sendto(_sessionSocket, value + count, length - count, 0, (struct sockaddr *)&address, sizeof(struct sockaddr_in));

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
				struct sockaddr_in address{ };

				std::size_t addressLen = sizeof(struct sockaddr);

				address.sin_port = htons(_session->_localPort);
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = Net::AsNetByte(_session->_localHost.c_str());

				auto len = ::recvfrom(_sessionSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, (socklen_t *)&addressLen);

				if (len < 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					buffer[len] = '\0';

					if (_session)
					{
						_session->OnReceive(inet_ntoa(address.sin_addr), ntohs(address.sin_port), buffer, static_cast<std::size_t>(len));
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

				struct sockaddr_in address{ };

				address.sin_port = htons(_sendQueue.front()._port);
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = Net::AsNetByte(_sendQueue.front()._host.c_str());

				while (_isConnect && count < length)
				{
					auto len = ::sendto(_sessionSocket, value + count, length - count, 0, (struct sockaddr *)&address, sizeof(struct sockaddr_in));

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
	void UDPSessionPipe::OnCallBack(const NetEvent * netEvent, const void * sysEvent)
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
				TINY_TOOLKIT_ASSERT(false, "UDPSessionPipe type error : {}", netEvent->_socket)

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
	UDPServerPipe::UDPServerPipe(int32_t managerSocket, int32_t sessionSocket, IUDPServer * server, NET_EVENT_TYPE netEventType) : _managerSocket(managerSocket),
																																   _sessionSocket(sessionSocket),
																																   _server(server)
	{
		_netEvent._type = netEventType;
		_netEvent._socket = _sessionSocket;
		_netEvent._callback = std::bind(&UDPServerPipe::OnCallBack, this, std::placeholders::_1, std::placeholders::_2);
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void UDPServerPipe::Close()
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
	 * @param host 待发送主机地址
	 * @param port 待发送主机端口
	 * @param value 待发送数据
	 * @param size 待发送数据长度
	 *
	 */
	void UDPServerPipe::Send(const char * host, uint16_t port, const void * value, std::size_t size)
	{
		(void)host;
		(void)port;
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
	void UDPServerPipe::DoAccept(const void * sysEvent)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto currentEvent = reinterpret_cast<const struct kevent *>(sysEvent);

		if (currentEvent->filter == EVFILT_READ)
		{

		}

#else

		auto currentEvent = reinterpret_cast<const struct epoll_event *>(sysEvent);

		if (currentEvent->events & EPOLLIN)
		{
			struct sockaddr_in clientAddress{ };

			std::size_t addressLen = sizeof(struct sockaddr);

			int32_t sock = Accept(_sessionSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLen);

			if (sock >= 0)
			{
				if (!EnableNonBlock(sock) ||
					!EnableReusePort(sock) ||
					!EnableReuseAddress(sock))
				{
					::close(sock);

					_server->OnError();

					return;
				}

				struct sockaddr_in serverAddress{ };

				serverAddress.sin_port = htons(_server->_port);
				serverAddress.sin_family = AF_INET;
				serverAddress.sin_addr.s_addr = Net::AsNetByte(_server->_host.c_str());

				if (::bind(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) == -1)
				{
					::close(sock);

					_server->OnError();

					return;
				}

				if (::connect(sock, (struct sockaddr *)&clientAddress, sizeof(struct sockaddr)) == -1)
				{
					::close(sock);

					_server->OnError();

					return;
				}

				uint16_t port = ntohs(clientAddress.sin_port);

				std::string host = inet_ntoa(clientAddress.sin_addr);

				auto session = _server->OnNewConnect(host, port);

				if (session)
				{
					GetLocalName(sock, session->_localHost, session->_localPort);
					session->_localPort = _server->_port;
					session->_localHost = _server->_host;

					session->_remotePort = port;
					session->_remoteHost = host;

					auto pipe = std::make_shared<UDPSessionPipe>(_managerSocket, sock, session, NET_EVENT_TYPE::TRANSMIT);

					struct epoll_event event{ };

					event.events = EPOLLIN;
					event.data.ptr = &_netEvent;

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
	void UDPServerPipe::OnCallBack(const NetEvent * netEvent, const void * sysEvent)
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
				TINY_TOOLKIT_ASSERT(false, "UDPServerPipe type error : {}", netEvent->_socket)

				break;
			}
		}
	}
}
