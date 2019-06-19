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
		auto addressLen = sizeof(struct sockaddr_in);

		memset(&event._address, 0, sizeof(struct sockaddr_in));

		return ::accept(socket, (struct sockaddr *)&event._address, (socklen_t *)&addressLen);
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
	TCPSessionPipe::TCPSessionPipe(ITCPSession * session, TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_HANDLE handle) : _cache(session->_cacheSize),
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
	 * 析构函数
	 *
	 */
	TCPSessionPipe::~TCPSessionPipe()
	{
		Close();
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void TCPSessionPipe::Close()
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
	 * @param data 待发送数据指针
	 * @param size 待发送数据长度
	 *
	 */
	void TCPSessionPipe::Send(const void * data, std::size_t size)
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

		if (!_isSend)
		{
			_isSend = true;

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
	 * 回调函数
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void TCPSessionPipe::OnCallback(NetEvent * netEvent, void * sysEvent)
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
				TINY_TOOLKIT_ASSERT(false, "TCPSessionPipe type error : {}", netEvent->_socket);

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
	void TCPSessionPipe::DoIO(NetEvent * netEvent, void * sysEvent)
	{
		(void)netEvent;
		(void)sysEvent;

		if (!_isConnect)
		{
			return;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto * currentEventPtr = reinterpret_cast<const struct kevent *>(sysEvent);

		if (currentEventPtr->flags & EV_ERROR)
		{
			Close();

			return;
		}

		if (currentEventPtr->filter == EVFILT_READ)
		{
			if (_isConnect)
			{
				auto len = ::recv(_socket, netEvent->_temp, sizeof(netEvent->_temp), 0);

				if (len < 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					netEvent->_bytes = static_cast<std::size_t>(len);

					if (_cache.Push(netEvent->_temp, netEvent->_bytes))
					{
						if (_session)
						{
							_cache.Reduced(_session->OnReceive(_cache.Value(), _cache.Length()));
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

		if (currentEventPtr->filter == EVFILT_WRITE)
		{
			if (_isConnect)
			{
				auto & value = _sendQueue.Front();

				auto len = ::send(_socket, value->_data, value->_size, 0);

				if (len > 0)
				{
					_sendQueue.Pop();

					if (_sendQueue.Empty())
					{
						_isSend = false;

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
			if (_isConnect)
			{
				auto len = ::recv(_socket, netEvent->_temp, sizeof(netEvent->_temp), 0);

				if (len < 0 && errno == EAGAIN)
				{
					return;
				}
				else if (len > 0)
				{
					netEvent->_bytes = static_cast<std::size_t>(len);

					if (_cache.Push(netEvent->_temp, netEvent->_bytes))
					{
						if (_session)
						{
							_cache.Reduced(_session->OnReceive(_cache.Value(), _cache.Length()));
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
				auto & value = _sendQueue.Front();

				std::size_t total = 0;

				while (total < value->_size)
				{
					auto len = ::send(_socket, value->_data, value->_size, 0);

					if (len > 0)
					{
						total += len;

						if (total >= value->_size)
						{
							_sendQueue.Pop();

							if (_sendQueue.Empty())
							{
								_isSend = false;

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
	 * 发送处理
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void TCPSessionPipe::DoSend(NetEvent * netEvent, void * sysEvent)
	{
		(void)netEvent;
		(void)sysEvent;

		if (!_isConnect)
		{
			return;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		_sendQueue.Pop();

		if (_sendQueue.Empty())
		{
			_isSend = false;

			return;
		}

		if (!AsyncSend())
		{
			Close();
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
	void TCPSessionPipe::DoReceive(NetEvent * netEvent, void * sysEvent)
	{
		(void)netEvent;
		(void)sysEvent;

		if (!_isConnect)
		{
			return;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (netEvent->_bytes > 0)
		{
			if (_cache.Push(netEvent->_temp, netEvent->_bytes))
			{
				if (_session)
				{
					_cache.Reduced(_session->OnReceive(_cache.Value(), _cache.Length()));
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
	void TCPSessionPipe::DoConnect(NetEvent * netEvent, void * sysEvent)
	{
		(void)netEvent;
		(void)sysEvent;

		delete netEvent;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (Net::GetLocalAddress(_socket, _receiveEvent._address))
		{
			if (_session)
			{
				_session->_localPort = ntohs(_receiveEvent._address.sin_port);
				_session->_localHost = inet_ntoa(_receiveEvent._address.sin_addr);
			}
		}

		if (setsockopt(_socket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, nullptr, 0) == -1)
		{
			Close();

			if (_session)
			{
				_session->OnConnectFailed();
			}
		}

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

		if (Net::GetLocalAddress(_socket, _netEvent._address))
		{
			if (_session)
			{
				_session->_localPort = ntohs(_netEvent._address.sin_port);
				_session->_localHost = inet_ntoa(_netEvent._address.sin_addr);
			}
		}

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

		if (Net::GetLocalAddress(_socket, _netEvent._address))
		{
			if (_session)
			{
				_session->_localPort = ntohs(_netEvent._address.sin_port);
				_session->_localHost = inet_ntoa(_netEvent._address.sin_addr);
			}
		}

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

	/**
	 *
	 * 异步发送
	 *
	 * @return 是否处理成功
	 *
	 */
	bool TCPSessionPipe::AsyncSend()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		DWORD flag = 0;
		DWORD bytes = 0;

		auto & value = _sendQueue.Front();

		memset(&_sendEvent._overlap, 0, sizeof(OVERLAPPED));

		_sendEvent._buffer.buf = value->_data;
		_sendEvent._buffer.len = value->_size;

		if (WSASend(_socket, &_sendEvent._buffer, 1, &bytes, flag, (LPWSAOVERLAPPED)&_sendEvent, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				return false;
			}
		}

#endif

		return true;
	}

	/**
	 *
	 * 异步连接
	 *
	 * @return 是否处理成功
	 *
	 */
	bool TCPSessionPipe::AsyncAccept()
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
	bool TCPSessionPipe::AsyncReceive()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		DWORD flag = 0;
		DWORD bytes = 0;

		memset(&_receiveEvent._overlap, 0, sizeof(OVERLAPPED));

		_receiveEvent._buffer.buf = _receiveEvent._temp;
		_receiveEvent._buffer.len = sizeof(_receiveEvent._temp);

		if (WSARecv(_socket, &_receiveEvent._buffer, 1, &bytes, &flag, (LPWSAOVERLAPPED)&_receiveEvent, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				return false;
			}
		}

#endif

		return true;
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
	TCPServerPipe::TCPServerPipe(ITCPServer * server, TINY_TOOLKIT_SOCKET_TYPE socket, TINY_TOOLKIT_SOCKET_HANDLE handle) : _server(server),
																															_socket(socket),
																															_handle(handle)
	{
		_netEvent._type = NET_EVENT_TYPE::ACCEPT;
		_netEvent._socket = socket;
		_netEvent._completer = this;
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	TCPServerPipe::~TCPServerPipe()
	{
		Close();
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void TCPServerPipe::Close()
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
	 * @param data 待发送数据指针
	 * @param size 待发送数据长度
	 *
	 */
	void TCPServerPipe::Send(const void * data, std::size_t size)
	{
		(void)size;
		(void)data;
	}

	/**
	 *
	 * 回调函数
	 *
	 * @param netEvent 网络事件
	 * @param sysEvent 系统事件
	 *
	 */
	void TCPServerPipe::OnCallback(NetEvent * netEvent, void * sysEvent)
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
				TINY_TOOLKIT_ASSERT(false, "TCPServerPipe type error : {}", netEvent->_socket);

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
	void TCPServerPipe::DoAccept(NetEvent * netEvent, void * sysEvent)
	{
		(void)netEvent;
		(void)sysEvent;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		TINY_TOOLKIT_SOCKET_TYPE sock = netEvent->_socket;

		if (setsockopt(sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&_socket, sizeof(TINY_TOOLKIT_SOCKET_TYPE)) == -1)
		{
			Net::CloseSocket(sock);

			_server->OnError();

			return;
		}

		if (CreateIoCompletionPort((HANDLE)sock, _handle, (ULONG_PTR)sock, 0) != _handle)
		{
			Net::CloseSocket(sock);

			_server->OnError();

			return;
		}

		if (Net::GetRemoteAddress(sock, _netEvent._address))
		{
			_server->_remotePort = ntohs(_netEvent._address.sin_port);
			_server->_remoteHost = inet_ntoa(_netEvent._address.sin_addr);
		}

		auto session = _server->OnSessionConnect();

		if (session)
		{
			session->_cacheSize = _server->_cacheSize;

			session->_localPort = _server->_localPort;
			session->_localHost = _server->_localHost;

			session->_remotePort = _server->_remotePort;
			session->_remoteHost = _server->_remoteHost;

			auto pipe = std::make_shared<TCPSessionPipe>(session, sock, _handle);

			if (pipe->AsyncReceive())
			{
				pipe->_isConnect = true;

				session->_pipe = pipe;

				session->OnConnect();
			}
			else
			{
				Net::CloseSocket(sock);

				_server->OnSessionError(session);
			}
		}
		else
		{
			Net::CloseSocket(sock);

			_server->OnSessionError(session);
		}

		AsyncAccept();

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
				if (!Net::DisableNagle(sock) ||
					!Net::EnableLinger(sock) ||
					!Net::EnableNonBlock(sock) ||
					!Net::EnableReuseAddress(sock))
				{
					Net::CloseSocket(sock);

					_server->OnError();

					return;
				}

				_server->_remotePort = ntohs(_netEvent._address.sin_port);
				_server->_remoteHost = inet_ntoa(_netEvent._address.sin_addr);

				auto session = _server->OnSessionConnect();

				if (session)
				{
					session->_cacheSize = _server->_cacheSize;

					session->_localPort = _server->_localPort;
					session->_localHost = _server->_localHost;

					session->_remotePort = _server->_remotePort;
					session->_remoteHost = _server->_remoteHost;

					auto pipe = std::make_shared<TCPSessionPipe>(session, sock, _handle);

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
				if (!Net::DisableNagle(sock) ||
					!Net::EnableLinger(sock) ||
					!Net::EnableNonBlock(sock) ||
					!Net::EnableReuseAddress(sock))
				{
					Net::CloseSocket(sock);

					_server->OnError();

					return;
				}

				_server->_remotePort = ntohs(_netEvent._address.sin_port);
				_server->_remoteHost = inet_ntoa(_netEvent._address.sin_addr);

				auto session = _server->OnSessionConnect();

				if (session)
				{
					session->_cacheSize = _server->_cacheSize;

					session->_localPort = _server->_localPort;
					session->_localHost = _server->_localHost;

					session->_remotePort = _server->_remotePort;
					session->_remoteHost = _server->_remoteHost;

					auto pipe = std::make_shared<TCPSessionPipe>(session, sock, _handle);

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
					}
				}
				else
				{
					_server->OnSessionError(session);
				}
			}
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
	bool TCPServerPipe::AsyncSend()
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
	bool TCPServerPipe::AsyncAccept()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		TINY_TOOLKIT_SOCKET_TYPE sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

		if (sock == TINY_TOOLKIT_SOCKET_INVALID)
		{
			Net::CloseSocket(sock);

			return false;
		}

		if (!Net::DisableNagle(sock) ||
			!Net::EnableLinger(sock) ||
			!Net::EnableNonBlock(sock) ||
			!Net::EnableReuseAddress(sock))
		{
			Net::CloseSocket(sock);

			return false;
		}

		_netEvent._socket = sock;

		_netEvent._buffer.buf = _netEvent._temp;
		_netEvent._buffer.len = sizeof(_netEvent._temp);

		memset(&_netEvent._overlap, 0, sizeof(OVERLAPPED));

		if (!AcceptEx(_socket, sock, _netEvent._temp, (LPOVERLAPPED)&_netEvent))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				Net::CloseSocket(sock);

				return false;
			}
		}

#endif

		return true;
	}

	/**
	 *
	 * 异步接收
	 *
	 * @return 是否处理成功
	 *
	 */
	bool TCPServerPipe::AsyncReceive()
	{
		return true;
	}
}
