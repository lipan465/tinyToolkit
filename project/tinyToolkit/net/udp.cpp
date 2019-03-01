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

	static std::unordered_map<std::string, tinyToolkit::IUDPSession *> sContainer{};

#else

	static std::unordered_map<std::string, tinyToolkit::IUDPSession *> sContainer{ };

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
		static char buffer[TINY_TOOLKIT_MB]{ 0 };

		while (true)
		{
			struct sockaddr_in clientAddress{ };

			auto len = ::recvfrom(socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddress, addressLen);

			if (len < 0 && errno == EAGAIN)
			{
				continue;
			}
			else if (len > 0)
			{
				buffer[len] = '\0';

				memcpy(address, (struct sockaddr *)&clientAddress, sizeof(struct sockaddr));

				auto find = sContainer.find(String::Format("{}_{}", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port)));

				if (find == sContainer.end())
				{
					return ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				}
				else
				{
					if (find->second)
					{
						find->second->OnReceive(buffer, static_cast<std::size_t>(len));
					}

					return -2;
				}
			}
			else
			{
				return -1;
			}
		}
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
				auto find = sContainer.find(String::Format("{}_{}", _session->_remoteHost, _session->_remotePort));

				if (find != sContainer.end())
				{
					sContainer.erase(find);
				}

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
	void UDPSessionPipe::Send(const void * value, std::size_t size)
	{
		if (_isConnect)
		{
			_sendQueue.emplace(_session->_remoteHost.c_str(), _session->_remotePort, value, size);

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
				auto len = ::read(_sessionSocket, buffer, sizeof(buffer));

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

		if (currentEvent->filter == EVFILT_WRITE)
		{
			if (!_sendQueue.empty())
			{
				std::size_t count = 0;

				std::size_t length = _sendQueue.front()._value.size();

				const char * value = _sendQueue.front()._value.c_str();

				while (_isConnect && count < length)
				{
					auto len = ::write(_sessionSocket, value + count, length - count);

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
				auto len = ::read(_sessionSocket, buffer, sizeof(buffer));

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
					auto len = ::write(_sessionSocket, value + count, length - count);

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
	 * @param value 待发送数据
	 * @param size 待发送数据长度
	 *
	 */
	void UDPServerPipe::Send(const void * value, std::size_t size)
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
	void UDPServerPipe::DoAccept(const void * sysEvent)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto currentEvent = reinterpret_cast<const struct kevent *>(sysEvent);

		if (currentEvent->filter == EVFILT_READ)
		{
			struct sockaddr_in clientAddress{ };

			std::size_t addressLen = sizeof(struct sockaddr);

			int32_t sock = Accept(_sessionSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLen);

			if (sock >= 0)
			{
				if (!Net::EnableNonBlock(sock) ||
					!Net::EnableReuseAddress(sock))
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

					auto pipe = std::make_shared<UDPSessionPipe>(_managerSocket, sock, session, NET_EVENT_TYPE::TRANSMIT);

					struct kevent event[2]{ };

					EV_SET(&event[0], _sessionSocket, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&pipe->_netEvent);
					EV_SET(&event[1], _sessionSocket, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&pipe->_netEvent);

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

			std::size_t addressLen = sizeof(struct sockaddr);

			int32_t sock = Accept(_sessionSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLen);

			if (sock >= 0)
			{
				if (!Net::EnableNonBlock(sock) ||
					!Net::EnableReuseAddress(sock))
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

					auto pipe = std::make_shared<UDPSessionPipe>(_managerSocket, sock, session, NET_EVENT_TYPE::TRANSMIT);

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
						sContainer.insert(std::make_pair(String::Format("{}_{}", remoteHost, remotePort), session));

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
			else if (sock == -2)
			{
				/// do nothing
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
