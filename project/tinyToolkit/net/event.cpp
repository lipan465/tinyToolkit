/**
 *
 *  作者: hm
 *
 *  说明: 事件
 *
 */


#include "tcp.h"

#include "../utilities/net.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	NetEvent::NetEvent()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		WSADATA wsaData{ };

		if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
		{
			return;
		}

		if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
		{
			WSACleanup();

			return;
		}

		_socket = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		_socket = kqueue();

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		_socket = epoll_create1(EPOLL_CLOEXEC);

#endif
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	NetEvent::~NetEvent()
	{
		if (IsValid())
		{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			WSACleanup();

			CloseHandle(_socket);

#else

			Net::CloseSocket(_socket);

#endif

			_socket = TINY_TOOLKIT_SOCKET_EVENT_INVALID;
		}
	}

	/**
	 *
	 * 是否有效
	 *
	 * @return 是否有效
	 *
	 */
	bool NetEvent::IsValid()
	{
		return _socket != TINY_TOOLKIT_SOCKET_EVENT_INVALID;
	}

	/**
	 *
	 * 移除监听事件
	 *
	 * @param socket 句柄
	 * @param context 上下文
	 *
	 * @return 是否移除成功
	 *
	 */
	bool NetEvent::Remove(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context)
	{
		(void)socket;
		(void)context;

		if (!IsValid())
		{
			return false;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		struct kevent event[2]{ };

		EV_SET(&event[0], socket, EVFILT_READ,  EV_DELETE, 0, 0, context);
		EV_SET(&event[1], socket, EVFILT_WRITE, EV_DELETE, 0, 0, context);

		return kevent(_socket, event, 2, nullptr, 0, nullptr) != -1;

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		struct epoll_event event{ };

		event.events =  EPOLLIN | EPOLLOUT;
		event.data.ptr = context;

		return epoll_ctl(_socket, EPOLL_CTL_DEL, socket, &event) != -1;

#else

		return true;

#endif
	}

	/**
	 *
	 * 修改监听事件
	 *
	 * @param socket 句柄
	 * @param context 上下文
	 * @param readFlag 读监听
	 * @param writeFlag 写监听
	 *
	 * @return 是否修改成功
	 *
	 */
	bool NetEvent::Modify(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context, bool readFlag, bool writeFlag)
	{
		(void)socket;
		(void)context;
		(void)readFlag;
		(void)writeFlag;

		if (!IsValid())
		{
			return false;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		struct kevent event[2]{ };

		EV_SET(&event[0], socket, EVFILT_READ,  readFlag  ? EV_ENABLE : EV_DISABLE, 0, 0, context);
		EV_SET(&event[1], socket, EVFILT_WRITE, writeFlag ? EV_ENABLE : EV_DISABLE, 0, 0, context);

		return kevent(_socket, event, 2, nullptr, 0, nullptr) != -1;

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		struct epoll_event event{ };

		event.events = (readFlag ? EPOLLIN : 0) | (writeFlag ? EPOLLOUT : 0);
		event.data.ptr = context;

		return epoll_ctl(_socket, EPOLL_CTL_MOD, socket, &event) != -1;

#else

		return false;

#endif
	}

	/**
	 *
	 * 添加监控
	 *
	 * @param socket 句柄
	 * @param context 上下文
	 * @param readFlag 读监听
	 * @param writeFlag 写监听
	 *
	 * @return 是否添加成功
	 *
	 */
	bool NetEvent::Append(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context, bool readFlag, bool writeFlag)
	{
		(void)socket;
		(void)context;
		(void)readFlag;
		(void)writeFlag;

		if (!IsValid())
		{
			return false;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		return CreateIoCompletionPort((HANDLE)socket, _socket, (ULONG_PTR)socket, 0) == _socket;

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		struct kevent event[2]{ };

		EV_SET(&event[0], socket, EVFILT_READ,  EV_ADD | (readFlag  ? EV_ENABLE : EV_DISABLE), 0, 0, context);
		EV_SET(&event[1], socket, EVFILT_WRITE, EV_ADD | (writeFlag ? EV_ENABLE : EV_DISABLE), 0, 0, context);

		return kevent(_socket, event, 2, nullptr, 0, nullptr) != -1;

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		struct epoll_event event{ };

		event.events = (readFlag ? EPOLLIN : 0) | (writeFlag ? EPOLLOUT : 0);
		event.data.ptr = context;

		return epoll_ctl(_socket, EPOLL_CTL_ADD, socket, &event) != -1;

#else

		return false;

#endif
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 构造函数
	 *
	 */
	NetEventMonitor::NetEventMonitor()
	{
		if (!IsValid())
		{
			return;
		}

		_thread = std::thread
		(
			[this]()
			{
				while (_status)
				{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

					DWORD bytes = 0;

					NetContext * context = nullptr;

					TINY_TOOLKIT_SOCKET_TYPE socket = TINY_TOOLKIT_SOCKET_INVALID;

					if (GetQueuedCompletionStatus(_event._socket, &bytes, (PULONG_PTR)&socket, (LPOVERLAPPED *)&context, TINY_TOOLKIT_SOCKET_EVENT_TIMEOUT))
					{
						if (context && context->completer)
						{
							context->bytes = bytes;

							context->completer->Logic(context, nullptr);
						}
					}
					else
					{
						if (context && context->completer)
						{
							context->bytes = bytes;

							context->completer->Logic(context, nullptr);
						}
						else
						{
							if (GetLastError() != WAIT_TIMEOUT)
							{
								break;
							}
						}
					}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

					struct kevent events[32]{ };

					struct timespec timestamp{ };

					timestamp.tv_sec = 0;
					timestamp.tv_nsec = TINY_TOOLKIT_SOCKET_EVENT_TIMEOUT * 1000000;

					int32_t count = kevent
					(
						_event._socket,
						nullptr,
						0,
						events,
						static_cast<int32_t>(sizeof(events) / sizeof(events[0])),
						&timestamp
					);

					if (count == -1)
					{
						if (errno != EINTR)
						{
							break;
						}
					}

					for (int32_t i = 0; i < count; ++i)
					{
						auto * context = reinterpret_cast<NetContext *>(events[i].udata);

						if (context && context->completer)
						{
							context->completer->Logic(context, &events[i]);
						}
					}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

					struct epoll_event events[32]{ };

					int32_t count = epoll_wait
					(
						_event._socket,
						events,
						static_cast<int32_t>(sizeof(events) / sizeof(events[0])),
						TINY_TOOLKIT_SOCKET_EVENT_TIMEOUT
					);

					if (count == -1)
					{
						if (errno != EINTR)
						{
							break;
						}
					}

					for (int32_t i = 0; i < count; ++i)
					{
						auto * context = reinterpret_cast<NetContext *>(events[i].data.ptr);

						if (context && context->completer)
						{
							context->completer->Logic(context, &events[i]);
						}
					}

#endif
				}
			}
		);
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	NetEventMonitor::~NetEventMonitor()
	{
		if (!_status)
		{
			return;
		}

		_status = false;

		if (_thread.joinable())
		{
			_thread.join();
		}
	}

	/**
	 *
	 * 是否有效
	 *
	 * @return 是否有效
	 *
	 */
	bool NetEventMonitor::IsValid()
	{
		return _event.IsValid();
	}

	/**
	 *
	 * 移除事件监控
	 *
	 * @param socket 句柄
	 * @param context 上下文
	 *
	 * @return 是否移除成功
	 *
	 */
	bool NetEventMonitor::RemoveEvent(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		return _event.Remove(socket, context);
	}

	/**
	 *
	 * 修改事件监控
	 *
	 * @param socket 句柄
	 * @param context 上下文
	 * @param readFlag 读监听
	 * @param writeFlag 写监听
	 *
	 * @return 是否修改成功
	 *
	 */
	bool NetEventMonitor::ModifyEvent(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context, bool readFlag, bool writeFlag)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		return _event.Modify(socket, context, readFlag, writeFlag);
	}

	/**
	 *
	 * 添加事件监控
	 *
	 * @param socket 句柄
	 * @param context 上下文
	 * @param readFlag 读监听
	 * @param writeFlag 写监听
	 *
	 * @return 是否添加成功
	 *
	 */
	bool NetEventMonitor::AppendEvent(TINY_TOOLKIT_SOCKET_TYPE socket, NetContext * context, bool readFlag, bool writeFlag)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		return _event.Append(socket, context, readFlag, writeFlag);
	}

	/**
	 *
	 * 启动tcp服务
	 *
	 * @param server 服务
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetEventMonitor::LaunchTCPServer(ITCPServer * server)
	{
		if (!IsValid() || server == nullptr)
		{
			return false;
		}

		auto socket = std::make_shared<TCPSocket>();

		if (!socket->IsValid())
		{
			server->OnSocketFailed();

			return false;
		}

		if (!socket->SetNoDelay(true) ||
		    !socket->SetNoBlock(true) ||
		    !socket->SetReuseAddress(true))
		{
			server->OnSocketFailed();

			socket->Close();

			return false;
		}

		server->OnSocket();

		if (socket->Bind(server->LocalAddress().host.c_str(), server->LocalAddress().port) == -1)
		{
			server->OnBindFailed();

			socket->Close();

			return false;
		}

		server->OnBind();

		if (socket->Listen(TINY_TOOLKIT_SOCKET_LISTEN_COUNT) == -1)
		{
			server->OnListenFailed();

			socket->Close();

			return false;
		}

		server->OnListen();

		auto pipe = std::make_shared<TCPServerPipe>(socket, server, this);

		if (!AppendEvent(socket->Handle(), &pipe->_acceptContext, true, false))
		{
			server->OnEventError();

			socket->Close();

			return false;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (!pipe->Accept())
		{
			server->OnAcceptFailed();

			socket->Close();

			return false;
		}

#endif

		server->_pipe = pipe;

		return true;
	}

	/**
	 *
	 * 启动tcp会话
	 *
	 * @param session 会话
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetEventMonitor::LaunchTCPSession(ITCPSession * session)
	{
		if (!IsValid() || session == nullptr)
		{
			return false;
		}

		auto socket = std::make_shared<TCPSocket>();

		if (!socket->IsValid())
		{
			session->OnSocketFailed();

			return false;
		}

		if (!socket->SetNoDelay(true) ||
		    !socket->SetNoBlock(true) ||
		    !socket->SetReuseAddress(true))
		{
			session->OnSocketFailed();

			socket->Close();

			return false;
		}

		session->OnSocket();

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (socket->Bind("0.0.0.0", 0) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			session->OnBindFailed();

			socket->Close();

			return false;
		}

		session->OnBind();

		auto pipe = std::make_shared<TCPSessionPipe>(socket, session, this);

		pipe->_sendContext.type = NetContext::TYPE::CONNECT;

		if (!AppendEvent(socket->Handle(), &pipe->_sendContext, true, false))
		{
			session->OnEventError();

			socket->Close();

			return false;
		}

		auto ret = socket->Connect(session->PeerAddress().host.c_str(), session->PeerAddress().port, &pipe->_sendContext);

		if (ret < 0)
		{
			session->OnConnectFailed();

			socket->Close();

			return false;
		}

		session->_pipe = pipe;

#else

		auto ret = socket->Connect(session->PeerAddress().host.c_str(), session->PeerAddress().port);

		if (ret == 0)
		{
			auto pipe = std::make_shared<TCPSessionPipe>(socket, session, this);

			if (!AppendEvent(socket->Handle(), &pipe->_ioContext, true, false))
			{
				session->OnEventError();

				socket->Close();

				return false;
			}

			pipe->_isConnect = true;

			session->_pipe = pipe;

			session->OnConnect();
		}
		else if (ret < 0 && errno != EINPROGRESS)
		{
			session->OnConnectFailed();

			socket->Close();

			return false;
		}
		else
		{
			auto pipe = std::make_shared<TCPSessionPipe>(socket, session, this);

			pipe->_ioContext.type = NetContext::TYPE::CONNECT;

			if (!AppendEvent(socket->Handle(), &pipe->_ioContext, false, true))
			{
				session->OnEventError();

				socket->Close();

				return false;
			}

			session->_pipe = pipe;
		}

#endif

		return true;
	}
}
