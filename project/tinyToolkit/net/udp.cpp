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
	static int32_t Accept(TINY_TOOLKIT_SOCKET_TYPE socket, struct sockaddr * address, socklen_t * addressLen)
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

	/**
	 *
	 * 构造函数
	 *
	 * @param handle 管理句柄
	 * @param session 会话
	 * @param socket 会话句柄
	 * @param sSize 发送缓冲区大小
	 * @param rSize 接收缓冲区大小
	 *
	 */
	UDPSessionPipe::UDPSessionPipe(NetHandle & handle, IUDPSession * session, TINY_TOOLKIT_SOCKET_TYPE socket, std::size_t sSize, std::size_t rSize) : _sendBuffer(sSize),
																																					   _receiveBuffer(rSize),
																																					   _managerHandle(handle),
																																					   _session(session),
																																					   _socket(socket)
	{
		_netEvent._type = NET_EVENT_TYPE::TRANSMIT;
		_netEvent._socket = socket;
		_netEvent._completer = this;
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

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			::closesocket(_socket);

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], _socket, EVFILT_READ,  EV_DELETE, 0, 0, nullptr);
			EV_SET(&event[1], _socket, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

			kevent(_managerHandle.socket, event, 2, nullptr, 0, nullptr);

			::close(_socket);

#else

			epoll_ctl(_managerHandle.socket, EPOLL_CTL_DEL, _socket, nullptr);

			::close(_socket);

#endif

			_socket = TINY_TOOLKIT_SOCKET_INVALID;

			if (_session)
			{
				auto find = sContainer.find(String::Format("{}_{}", _session->_remoteHost, _session->_remotePort));

				if (find != sContainer.end())
				{
					sContainer.erase(find);
				}

				_session->OnDisconnect();
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
	void UDPSessionPipe::Send(const void * value, std::size_t size, bool delay)
	{
		if (size == 0)
		{
			return;
		}

		if (!_isConnect)
		{
			return;
		}

		if (!_sendBuffer.Push(value, size))
		{
			Close();

			return;
		}

		if (!_isSend && !delay)
		{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], _socket, EVFILT_READ,  EV_ENABLE, 0, 0, (void *)&_netEvent);
			EV_SET(&event[1], _socket, EVFILT_WRITE, EV_ENABLE, 0, 0, (void *)&_netEvent);

			if (kevent(_managerHandle.socket, event, 2, nullptr, 0, nullptr) == -1)
			{
				Close();
			}

#else

			struct epoll_event event{ };

			event.events = EPOLLIN | EPOLLOUT;
			event.data.ptr = &_netEvent;

			if (epoll_ctl(_managerHandle.socket, EPOLL_CTL_MOD, _socket, &event) == -1)
			{
				Close();
			}

#endif
		}
	}

	/**
	 *
	 * 回调函数
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void UDPSessionPipe::OnCallback(const NetEvent * netEvent, const void * sysEvent)
	{
		switch (netEvent->_type)
		{
			case NET_EVENT_TYPE::CONNECT:
			{
				DoConnect(netEvent, sysEvent);

				break;
			}

			case NET_EVENT_TYPE::TRANSMIT:
			{
				DoTransmit(netEvent, sysEvent);

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
	 * 连接处理
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void UDPSessionPipe::DoConnect(const NetEvent * netEvent, const void * sysEvent)
	{
		(void)netEvent;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto * currentEventPtr = reinterpret_cast<const struct kevent *>(sysEvent);

		{
			struct kevent event[2]{ };

			EV_SET(&event[0], _socket, EVFILT_READ,  EV_DELETE, 0, 0, nullptr);
			EV_SET(&event[1], _socket, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

			kevent(_managerHandle.socket, event, 2, nullptr, 0, nullptr);
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

			EV_SET(&event[0], _socket, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&_netEvent);
			EV_SET(&event[1], _socket, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&_netEvent);

			if (kevent(_managerHandle.socket, event, 2, nullptr, 0, nullptr) == -1)
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

		epoll_ctl(_managerHandle.socket, EPOLL_CTL_DEL, _socket, nullptr);

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

			if (epoll_ctl(_managerHandle.socket, EPOLL_CTL_ADD, _socket, &event) == -1)
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
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void UDPSessionPipe::DoTransmit(const NetEvent * netEvent, const void * sysEvent)
	{
		(void)netEvent;

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
			static char buffer[TINY_TOOLKIT_MB]{ 0 };

			if (_isConnect)
			{
				_isSend = true;

				auto len = ::read(_socket, buffer, sizeof(buffer));

				_isSend = false;

				if (len < 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					if (_receiveBuffer.Push(buffer, static_cast<std::size_t>(len)))
					{
						if (_session)
						{
							_receiveBuffer.Reduced(_session->OnReceive(_receiveBuffer.Value(), _receiveBuffer.Length()));
						}
					}
					else
					{
						Close();

						return;
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
				_isReceive = true;

				auto len = ::write(_socket, _sendBuffer.Value(), _sendBuffer.Length());

				_isReceive = false;

				if (len > 0)
				{
					if (!_sendBuffer.Reduced(static_cast<std::size_t>(len)))
					{
						Close();

						return;
					}

					if (_sendBuffer.Length() == 0)
					{
						struct kevent event[2]{ };

						EV_SET(&event[0], _socket, EVFILT_READ,  EV_ENABLE,  0, 0, (void *)&_netEvent);
						EV_SET(&event[1], _socket, EVFILT_WRITE, EV_DISABLE, 0, 0, (void *)&_netEvent);

						if (kevent(_managerHandle.socket, event, 2, nullptr, 0, nullptr) == -1)
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

#else

		auto currentEvent = reinterpret_cast<const struct epoll_event *>(sysEvent);

		if (currentEvent->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
		{
			Close();

			return;
		}

		if (currentEvent->events & EPOLLIN)
		{
			static char buffer[TINY_TOOLKIT_MB]{ 0 };

			if (_isConnect)
			{
				_isSend = true;

				auto len = ::read(_socket, buffer, sizeof(buffer));

				_isSend = false;

				if (len < 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					if (_receiveBuffer.Push(buffer, static_cast<std::size_t>(len)))
					{
						if (_session)
						{
							_receiveBuffer.Reduced(_session->OnReceive(_receiveBuffer.Value(), _receiveBuffer.Length()));
						}
					}
					else
					{
						Close();

						return;
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
				_isReceive = true;

				auto len = ::write(_socket, _sendBuffer.Value(), _sendBuffer.Length());

				_isReceive = false;

				if (len > 0)
				{
					if (!_sendBuffer.Reduced(static_cast<std::size_t>(len)))
					{
						Close();

						return;
					}

					if (_sendBuffer.Length() == 0)
					{
						struct epoll_event event{ };

						event.events = EPOLLIN;
						event.data.ptr = &_netEvent;

						if (epoll_ctl(_managerHandle.socket, EPOLL_CTL_MOD, _socket, &event) == -1)
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


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 构造函数
	 *
	 * @param handle 管理句柄
	 * @param server 服务器
	 * @param socket 会话句柄
	 * @param sSize 发送缓冲区大小
	 * @param rSize 接收缓冲区大小
	 *
	 */
	UDPServerPipe::UDPServerPipe(NetHandle & handle, IUDPServer * server, TINY_TOOLKIT_SOCKET_TYPE socket, std::size_t sSize, std::size_t rSize) : _sSize(sSize),
																																				   _rSize(rSize),
																																				   _managerHandle(handle),
																																				   _server(server),
																																				   _socket(socket)
	{
		_netEvent._type = NET_EVENT_TYPE::ACCEPT;
		_netEvent._socket = _socket;
		_netEvent._completer = this;
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void UDPServerPipe::Close()
	{
		if (_socket != TINY_TOOLKIT_SOCKET_INVALID)
		{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			::closesocket(_socket);

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], _socket, EVFILT_READ,  EV_DELETE, 0, 0, nullptr);
			EV_SET(&event[1], _socket, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

			kevent(_managerHandle.socket, event, 2, nullptr, 0, nullptr);

			::close(_socket);

#else

			epoll_ctl(_managerHandle.socket, EPOLL_CTL_DEL, _socket, nullptr);

			::close(_socket);

#endif

			_socket = TINY_TOOLKIT_SOCKET_INVALID;

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
	 * @param delay 延迟发送
	 *
	 */
	void UDPServerPipe::Send(const void * value, std::size_t size, bool delay)
	{
		(void)size;
		(void)delay;
		(void)value;
	}

	/**
	 *
	 * 回调函数
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void UDPServerPipe::OnCallback(const NetEvent * netEvent, const void * sysEvent)
	{
		switch (netEvent->_type)
		{
			case NET_EVENT_TYPE::ACCEPT:
			{
				DoAccept(netEvent, sysEvent);

				break;
			}

			default:
			{
				TINY_TOOLKIT_ASSERT(false, "UDPServerPipe type error : {}", netEvent->_socket);

				break;
			}
		}
	}

	/**
	 *
	 * 连接处理
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void UDPServerPipe::DoAccept(const NetEvent * netEvent, const void * sysEvent)
	{
		(void)netEvent;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto currentEvent = reinterpret_cast<const struct kevent *>(sysEvent);

		if (currentEvent->filter == EVFILT_READ)
		{
			struct sockaddr_in clientAddress{ };

			std::size_t addressLen = sizeof(struct sockaddr);

			TINY_TOOLKIT_SOCKET_TYPE sock = Accept(_socket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLen);

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

				if (::bind(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
				{
					::close(sock);

					_server->OnError();

					return;
				}

				if (::connect(sock, (struct sockaddr *)&clientAddress, sizeof(struct sockaddr)) == TINY_TOOLKIT_SOCKET_ERROR)
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

					auto pipe = std::make_shared<UDPSessionPipe>(_managerHandle, session, sock, _sSize, _rSize);

					struct kevent event[2]{ };

					EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&pipe->_netEvent);
					EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&pipe->_netEvent);

					if (kevent(_managerHandle.socket, event, 2, nullptr, 0, nullptr) == -1)
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

			int32_t sock = Accept(_socket, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLen);

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

				if (::bind(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
				{
					::close(sock);

					_server->OnError();

					return;
				}

				if (::connect(sock, (struct sockaddr *)&clientAddress, sizeof(struct sockaddr)) == TINY_TOOLKIT_SOCKET_ERROR)
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

					auto pipe = std::make_shared<UDPSessionPipe>(_managerHandle, session, sock, _sSize, _rSize);

					struct epoll_event event{ };

					event.events = EPOLLIN;
					event.data.ptr = &pipe->_netEvent;

					if (epoll_ctl(_managerHandle.socket, EPOLL_CTL_ADD, sock, &event) == -1)
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
}
