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
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	/**
	 *
	 * 异步接收处理
	 *
	 * @param socket 会话套接字
	 * @param acceptSocket 监听套接字
	 * @param buffer 缓冲区
	 * @param overlapped 结构指针
	 *
	 * @return 是否处理成功
	 *
	 */
	static bool AcceptEx(TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_TYPE acceptSocket, PVOID buffer, LPOVERLAPPED overlapped)
	{
		static LPFN_ACCEPTEX function = nullptr;

		if (function == nullptr)
		{
			GUID guid = WSAID_ACCEPTEX;

			DWORD byte = 0;

			WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &function, sizeof(function), &byte, nullptr, nullptr);
		}

		if (function)
		{
			DWORD byte = 0;

			if (!function(socket, acceptSocket, buffer, 0, sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16, &byte, overlapped))
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					return false;
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	}

#else

	/**
	 *
	 * 会话处理
	 *
	 * @param socket 会话套接字
	 * @param event 事件
	 *
	 * @return 监听套接字
	 *
	 */
	static TINY_TOOLKIT_SOCKET_TYPE Accept(TINY_TOOLKIT_SOCKET_TYPE socket, NetEvent & event)
	{
		while (true)
		{
			auto addressLen = sizeof(struct sockaddr_in);

			auto len = ::recvfrom(socket, event._temp, sizeof(event._temp), 0, (struct sockaddr *)&event._address, (socklen_t *)&addressLen);

			if (len < 0 && errno == EAGAIN)
			{
				TINY_TOOLKIT_SLEEP_MS(10);

				continue;
			}
			else if (len > 0)
			{
				event._temp[len] = '\0';

				event._bytes = static_cast<std::size_t>(len);

				return ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			}
			else
			{
				return TINY_TOOLKIT_SOCKET_INVALID;
			}
		}
	}

#endif


	/**
	 *
	 * 构造函数
	 *
	 * @param session 会话
	 * @param socket 会话套接字
	 * @param handle 管理句柄
	 *
	 */
	UDPSessionPipe::UDPSessionPipe(IUDPSession * session, TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_HANDLE handle) : _sendBuffer(session->_sSize),
																																_receiveBuffer(session->_rSize),
																																_session(session),
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
	 * @param value 待发送数据
	 * @param size 待发送数据长度
	 * @param cache 缓冲发送
	 *
	 */
	void UDPSessionPipe::Send(const void * value, std::size_t size, bool cache)
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

		if (!_isSend && !cache)
		{
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

		memset(&_sendEvent._overlap, 0, sizeof(OVERLAPPED));

		_sendEvent._buffer.len = _sendBuffer.Length();
		_sendEvent._buffer.buf = const_cast<char *>(_sendBuffer.Value());

		if (WSASend(_socket, &_sendEvent._buffer, 1, &bytes, flag, (LPWSAOVERLAPPED)&_sendEvent, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
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
	 * 异步连接
	 *
	 * @return 是否处理成功
	 *
	 */
	bool UDPSessionPipe::AsyncAccept()
	{
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

		memset(&_receiveEvent._overlap, 0, sizeof(OVERLAPPED));

		_receiveEvent._buffer.len = sizeof(_receiveEvent._temp);
		_receiveEvent._buffer.buf = _receiveEvent._temp;

		if (WSARecv(_socket, &_receiveEvent._buffer, 1, &bytes, &flag, (LPWSAOVERLAPPED)&_receiveEvent, NULL) == TINY_TOOLKIT_SOCKET_ERROR)
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

				auto len = ::read(_socket, temp, sizeof(temp));

				_isReceive = false;

				if (len < 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					if (_receiveBuffer.Push(temp, static_cast<std::size_t>(len)))
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
				_isSend = true;

				auto len = ::write(_socket, _sendBuffer.Value(), _sendBuffer.Length());

				_isSend = false;

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

				auto len = ::read(_socket, temp, sizeof(temp));

				_isReceive = false;

				if (len < 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					if (_receiveBuffer.Push(temp, static_cast<std::size_t>(len)))
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
				_isSend = true;

				auto len = ::write(_socket, _sendBuffer.Value(), _sendBuffer.Length());

				_isSend = false;

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

		if (!_sendBuffer.Reduced(netEvent->_bytes))
		{
			Close();

			return;
		}

		if (_sendBuffer.Length() > 0)
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
			if (_receiveBuffer.Push(netEvent->_temp, netEvent->_bytes))
			{
				if (!AsyncReceive())
				{
					Close();

					return;
				}

				if (_session)
				{
					_receiveBuffer.Reduced(_session->OnReceive(_receiveBuffer.Value(), _receiveBuffer.Length()));
				}
			}
			else
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

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (AsyncReceive())
		{
			_isConnect = true;

			if (_session)
			{
				_session->OnConnect();
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

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto * currentEventPtr = reinterpret_cast<const struct kevent *>(sysEvent);

		{
			struct kevent event[2]{ };

			EV_SET(&event[0], _socket, EVFILT_READ,  EV_DELETE, 0, 0, nullptr);
			EV_SET(&event[1], _socket, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

			kevent(_handle, event, 2, nullptr, 0, nullptr);
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

			EV_SET(&event[0], _socket, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, &_netEvent);
			EV_SET(&event[1], _socket, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, &_netEvent);

			if (kevent(_handle, event, 2, nullptr, 0, nullptr) == -1)
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

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		auto currentEvent = reinterpret_cast<const struct epoll_event *>(sysEvent);

		epoll_ctl(_handle, EPOLL_CTL_DEL, _socket, nullptr);

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

			if (epoll_ctl(_handle, EPOLL_CTL_ADD, _socket, &event) == -1)
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


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 构造函数
	 *
	 * @param server 服务器
	 * @param socket 会话套接字
	 * @param handle 管理句柄
	 *
	 */
	UDPServerPipe::UDPServerPipe(IUDPServer * server, TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_HANDLE handle) : _server(server),
																															_socket(socket),
																															_handle(handle)

	{
		_netEvent._type = NET_EVENT_TYPE::ACCEPT;
		_netEvent._socket = socket;
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
	 * @param cache 缓冲发送
	 *
	 */
	void UDPServerPipe::Send(const void * value, std::size_t size, bool cache)
	{
		(void)size;
		(void)cache;
		(void)value;
	}

	/**
	 *
	 * 异步发送
	 *
	 * @return 是否处理成功
	 *
	 */
	bool UDPServerPipe::AsyncSend()
	{
		return true;
	}

	/**
	 *
	 * 异步连接
	 *
	 * @return 是否处理成功
	 *
	 */
	bool UDPServerPipe::AsyncAccept()
	{
		return true;
	}

	/**
	 *
	 * 异步接收
	 *
	 * @return 是否处理成功
	 *
	 */
	bool UDPServerPipe::AsyncReceive()
	{
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
	void UDPServerPipe::OnCallback(NetEvent * netEvent, void * sysEvent)
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
	void UDPServerPipe::DoAccept(NetEvent * netEvent, void * sysEvent)
	{
		(void)netEvent;
		(void)sysEvent;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto currentEvent = reinterpret_cast<const struct kevent *>(sysEvent);

		if (currentEvent->filter == EVFILT_READ)
		{
			TINY_TOOLKIT_SOCKET_TYPE sock = Accept(_socket, _netEvent);

			if (sock == TINY_TOOLKIT_SOCKET_INVALID)
			{
				_server->OnError();
			}
			else
			{
				if (!Net::EnableNonBlock(sock) ||
					!Net::EnableReuseAddress(sock))
				{
					Net::CloseSocket(sock);

					_server->OnError();

					return;
				}

				struct sockaddr_in serverAddress{ };

				serverAddress.sin_port = htons(_server->_port);
				serverAddress.sin_family = AF_INET;
				serverAddress.sin_addr.s_addr = Net::AsNetByte(_server->_host.c_str());

				if (::bind(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
				{
					Net::CloseSocket(sock);

					_server->OnError();

					return;
				}

				if (::connect(sock, (struct sockaddr *)&_netEvent._address, sizeof(struct sockaddr)) == TINY_TOOLKIT_SOCKET_ERROR)
				{
					Net::CloseSocket(sock);

					_server->OnError();

					return;
				}

				uint16_t localPort = _server->_port;
				uint16_t remotePort = ntohs(_netEvent._address.sin_port);

				std::string localHost = _server->_host;
				std::string remoteHost = inet_ntoa(_netEvent._address.sin_addr);

				auto session = _server->OnNewConnect(remoteHost, remotePort);

				if (session)
				{
					session->_sSize = _server->_sSize;
					session->_rSize = _server->_rSize;

					session->_localPort = localPort;
					session->_localHost = localHost;

					session->_remotePort = remotePort;
					session->_remoteHost = remoteHost;

					auto pipe = std::make_shared<UDPSessionPipe>(session, sock, _handle);

					struct kevent event[2]{ };

					EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, &pipe->_netEvent);
					EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, &pipe->_netEvent);

					if (kevent(_handle, event, 2, nullptr, 0, nullptr) == -1)
					{
						Net::CloseSocket(sock);

						_server->OnSessionError(session);
					}
					else
					{
						pipe->_isConnect = true;

						session->_pipe = pipe;

						session->OnConnect();

						session->OnReceive(_netEvent._temp, _netEvent._bytes);
					}
				}
				else
				{
					Net::CloseSocket(sock);

					_server->OnSessionError(session);
				}
			}
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		auto currentEvent = reinterpret_cast<const struct epoll_event *>(sysEvent);

		if (currentEvent->events & EPOLLIN)
		{
			TINY_TOOLKIT_SOCKET_TYPE sock = Accept(_socket, _netEvent);

			if (sock == TINY_TOOLKIT_SOCKET_INVALID)
			{
				_server->OnError();
			}
			else
			{
				if (!Net::EnableNonBlock(sock) ||
					!Net::EnableReuseAddress(sock))
				{
					Net::CloseSocket(sock);

					_server->OnError();

					return;
				}

				struct sockaddr_in serverAddress{ };

				serverAddress.sin_port = htons(_server->_port);
				serverAddress.sin_family = AF_INET;
				serverAddress.sin_addr.s_addr = Net::AsNetByte(_server->_host.c_str());

				if (::bind(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
				{
					Net::CloseSocket(sock);

					_server->OnError();

					return;
				}

				if (::connect(sock, (struct sockaddr *)&_netEvent._address, sizeof(struct sockaddr)) == TINY_TOOLKIT_SOCKET_ERROR)
				{
					Net::CloseSocket(sock);

					_server->OnError();

					return;
				}

				uint16_t localPort = _server->_port;
				uint16_t remotePort = ntohs(_netEvent._address.sin_port);

				std::string localHost = _server->_host;
				std::string remoteHost = inet_ntoa(_netEvent._address.sin_addr);

				auto session = _server->OnNewConnect(remoteHost, remotePort);

				if (session)
				{
					session->_sSize = _server->_sSize;
					session->_rSize = _server->_rSize;

					session->_localPort = localPort;
					session->_localHost = localHost;

					session->_remotePort = remotePort;
					session->_remoteHost = remoteHost;

					auto pipe = std::make_shared<UDPSessionPipe>(session, sock, _handle);

					struct epoll_event event{ };

					event.events = EPOLLIN;
					event.data.ptr = &pipe->_netEvent;

					if (epoll_ctl(_handle, EPOLL_CTL_ADD, sock, &event) == -1)
					{
						Net::CloseSocket(sock);

						_server->OnSessionError(session);
					}
					else
					{
						pipe->_isConnect = true;

						session->_pipe = pipe;

						session->OnConnect();

						session->OnReceive(_netEvent._temp, _netEvent._bytes);
					}
				}
				else
				{
					Net::CloseSocket(sock);

					_server->OnSessionError(session);
				}
			}
		}

#endif
	}
}
