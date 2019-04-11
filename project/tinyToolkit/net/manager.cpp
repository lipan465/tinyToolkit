/**
 *
 *  作者: hm
 *
 *  说明: 通讯管理器
 *
 */


#include "manager.h"

#include "../debug/trace.h"
#include "../utilities/net.h"


namespace tinyToolkit
{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	/**
	 *
	 * 异步连接处理
	 *
	 * @param socket 句柄
	 * @param address 地址
	 * @param addressLength 地址长度
	 * @param overlapped 结构指针
	 *
	 * @return 是否处理成功
	 *
	 */
	static bool ConnectEx(TINY_TOOLKIT_SOCKET_TYPE socket, const struct sockaddr * address, int32_t addressLength, LPOVERLAPPED overlapped)
	{
		static LPFN_CONNECTEX function = nullptr;

		if (function == nullptr)
		{
			GUID guid = WSAID_CONNECTEX;

			DWORD byte = 0;

			WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &function, sizeof(function), &byte, nullptr, nullptr);
		}

		if (function)
		{
			DWORD byte = 0;

			if (!function(socket, address, addressLength, nullptr, 0, &byte, overlapped))
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

#endif

	/**
	 *
	 * 析构函数
	 *
	 */
	NetManager::~NetManager()
	{
		if (_status)
		{
			_status = false;

			if (_thread.joinable())
			{
				_thread.join();
			}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (_handle)
			{
				WSACleanup();

				CloseHandle(_handle);

				_handle = TINY_TOOLKIT_SOCKET_HANDLE_INVALID;
			}

#else

			if (_handle != TINY_TOOLKIT_SOCKET_INVALID)
			{
				Net::CloseSocket(_handle);

				_handle = TINY_TOOLKIT_SOCKET_HANDLE_INVALID;
			}

#endif
		}
	}

	/**
	 *
	 * 启动
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetManager::Launch()
	{
		if (_handle == TINY_TOOLKIT_SOCKET_HANDLE_INVALID)
		{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			WSADATA wsaData;

			if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
			{
				TINY_TOOLKIT_ASSERT(false, "Start socket error : {}", ::GetLastError());

				return false;
			}

			if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
			{
				WSACleanup();

				TINY_TOOLKIT_ASSERT(false, "Socket version error : {}", ::GetLastError());

				return false;
			}

			_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);

			if (_handle == TINY_TOOLKIT_SOCKET_HANDLE_INVALID)
			{
				TINY_TOOLKIT_ASSERT(false, "Create net manager event error : {}", ::GetLastError());

				return false;
			}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			_handle = kqueue();

			if (_handle == TINY_TOOLKIT_SOCKET_HANDLE_INVALID)
			{
				TINY_TOOLKIT_ASSERT(false, "Create net manager event error : {}", strerror(errno));

				return false;
			}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			_handle = epoll_create(TINY_TOOLKIT_NET_COUNT);

			if (_handle == TINY_TOOLKIT_SOCKET_HANDLE_INVALID)
			{
				TINY_TOOLKIT_ASSERT(false, "Create net manager event error : {}", strerror(errno));

				return false;
			}

#endif

			_status = true;
			_thread = std::thread(&NetManager::ThreadProcess, this);

			return true;
		}

		return _handle != TINY_TOOLKIT_SOCKET_HANDLE_INVALID;
	}

	/**
	 *
	 * 启动udp客户端
	 *
	 * @param client 客户端
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetManager::LaunchUDPClient(IUDPSession * client)
	{
		if (client == nullptr)
		{
			return false;
		}

		if (!Launch())
		{
			return false;
		}

		{
			std::vector<std::string> localHostList{ };
			std::vector<std::string> remoteHostList{ };

			if (Net::TraverseAddressFromHost(client->_localHost.c_str(), localHostList) &&
				Net::TraverseAddressFromHost(client->_remoteHost.c_str(), remoteHostList))
			{
				client->_localHost = std::move(localHostList.front());
				client->_remoteHost = std::move(remoteHostList.front());
			}
			else
			{
				client->OnConnectFailed();

				return false;
			}
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		TINY_TOOLKIT_SOCKET_TYPE sock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);

#else

		TINY_TOOLKIT_SOCKET_TYPE sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

#endif

		if (sock == TINY_TOOLKIT_SOCKET_INVALID)
		{
			client->OnConnectFailed();

			return false;
		}

		if (!Net::EnableNonBlock(sock) ||
			!Net::EnableReuseAddress(sock))
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}

		struct sockaddr_in localAddress{ };

		localAddress.sin_port = htons(client->_localPort);
		localAddress.sin_family = AF_INET;
		localAddress.sin_addr.s_addr = Net::AsNetByte(client->_localHost.c_str());

		if (::bind(sock, (struct sockaddr *)&localAddress, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}

		struct sockaddr_in serverAddress { };

		serverAddress.sin_port = htons(client->_remotePort);
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = Net::AsNetByte(client->_remoteHost.c_str());

		if (::connect(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}

		auto pipe = std::make_shared<UDPSessionPipe>(client, sock, _handle);

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (Net::GetLocalAddress(sock, pipe->_receiveEvent._address))
		{
			client->_localPort = ntohs(pipe->_receiveEvent._address.sin_port);
			client->_localHost = inet_ntoa(pipe->_receiveEvent._address.sin_addr);
		}

		BOOL bNewBehavior = FALSE;

		DWORD dwBytesReturned = 0;

		if (WSAIoctl(sock, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), nullptr, 0, &dwBytesReturned, nullptr, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}

		if (CreateIoCompletionPort((HANDLE)sock, _handle, (ULONG_PTR)sock, 0) != _handle)
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}

		if (pipe->AsyncReceive())
		{
			pipe->_isConnect = true;

			client->_pipe = pipe;

			client->OnConnect();
		}
		else
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		if (Net::GetLocalAddress(sock, pipe->_netEvent._address))
		{
			client->_localPort = ntohs(pipe->_netEvent._address.sin_port);
			client->_localHost = inet_ntoa(pipe->_netEvent._address.sin_addr);
		}

		struct kevent event[2]{ };

		EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&pipe->_netEvent);
		EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&pipe->_netEvent);

		if (kevent(_handle, event, 2, nullptr, 0, nullptr) == -1)
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}
		else
		{
			pipe->_isConnect = true;

			client->_pipe = pipe;

			client->OnConnect();
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		if (Net::GetLocalAddress(sock, pipe->_netEvent._address))
		{
			client->_localPort = ntohs(pipe->_netEvent._address.sin_port);
			client->_localHost = inet_ntoa(pipe->_netEvent._address.sin_addr);
		}

		struct epoll_event event{ };

		event.events = EPOLLIN;
		event.data.ptr = &pipe->_netEvent;

		if (epoll_ctl(_handle, EPOLL_CTL_ADD, sock, &event) == -1)
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}
		else
		{
			pipe->_isConnect = true;

			client->_pipe = pipe;

			client->OnConnect();
		}

#endif

		return true;
	}

	/**
	 *
	 * 启动tcp客户端
	 *
	 * @param client 客户端
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetManager::LaunchTCPClient(ITCPSession * client)
	{
		if (client == nullptr)
		{
			return false;
		}

		if (!Launch())
		{
			return false;
		}

		{
			std::vector<std::string> remoteHostList{ };

			if (Net::TraverseAddressFromHost(client->_remoteHost.c_str(), remoteHostList))
			{
				client->_remoteHost = std::move(remoteHostList.front());
			}
			else
			{
				client->OnConnectFailed();

				return false;
			}
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		TINY_TOOLKIT_SOCKET_TYPE sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

#else

		TINY_TOOLKIT_SOCKET_TYPE sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#endif

		if (sock == TINY_TOOLKIT_SOCKET_INVALID)
		{
			client->OnConnectFailed();

			return false;
		}

		if (!Net::EnableLinger(sock) ||
			!Net::EnableNoDelay(sock) ||
			!Net::EnableNonBlock(sock) ||
			!Net::EnableReuseAddress(sock))
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}

		struct sockaddr_in serverAddress{ };

		serverAddress.sin_port = htons(client->_remotePort);
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = Net::AsNetByte(client->_remoteHost.c_str());

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		sockaddr_in localAddress{ };

		localAddress.sin_port = 0;
		localAddress.sin_family = AF_INET;
		localAddress.sin_addr.s_addr = INADDR_ANY;

		if (::bind(sock, (struct sockaddr *)&localAddress, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}

		if (CreateIoCompletionPort((HANDLE)sock, _handle, (ULONG_PTR)sock, 0) != _handle)
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}

		auto pipe = new TCPSessionPipe(client, sock, _handle);

		auto netEvent = new NetEvent(NET_EVENT_TYPE::CONNECT, sock, pipe);

		if (ConnectEx(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in), (LPOVERLAPPED)netEvent))
		{
			client->_pipe = std::shared_ptr<TCPSessionPipe>(pipe);
		}
		else
		{
			delete pipe;
			delete netEvent;

			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto ret = ::connect(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in));

		if (ret == 0)
		{
			auto pipe = std::make_shared<TCPSessionPipe>(client, sock, _handle);

			if (Net::GetLocalAddress(sock, pipe->_netEvent._address))
			{
				client->_localPort = ntohs(pipe->_netEvent._address.sin_port);
				client->_localHost = inet_ntoa(pipe->_netEvent._address.sin_addr);
			}

			struct kevent event[2]{ };

			EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&pipe->_netEvent);
			EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&pipe->_netEvent);

			if (kevent(_handle, event, 2, nullptr, 0, nullptr) == -1)
			{
				Net::CloseSocket(sock);

				client->OnConnectFailed();

				return false;
			}
			else
			{
				pipe->_isConnect = true;

				client->_pipe = pipe;

				client->OnConnect();
			}
		}
		else if (ret < 0 && errno != EINPROGRESS)
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}
		else
		{
			auto pipe = new TCPSessionPipe(client, sock, _handle);

			auto netEvent = new NetEvent(NET_EVENT_TYPE::CONNECT, sock, pipe);

			struct kevent event[2]{ };

			EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_DISABLE, 0, 0, (void *)netEvent);
			EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_ENABLE,  0, 0, (void *)netEvent);

			if (kevent(_handle, event, 2, nullptr, 0, nullptr) == -1)
			{
				delete pipe;
				delete netEvent;

				Net::CloseSocket(sock);

				client->OnConnectFailed();

				return false;
			}
			else
			{
				client->_pipe = std::shared_ptr<TCPSessionPipe>(pipe);
			}
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		auto ret = ::connect(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in));

		if (ret == 0)
		{
			auto pipe = std::make_shared<TCPSessionPipe>(client, sock, _handle);

			if (Net::GetLocalAddress(sock, pipe->_netEvent._address))
			{
				client->_localPort = ntohs(pipe->_netEvent._address.sin_port);
				client->_localHost = inet_ntoa(pipe->_netEvent._address.sin_addr);
			}

			struct epoll_event event{ };

			event.events = EPOLLIN;
			event.data.ptr = &pipe->_netEvent;

			if (epoll_ctl(_handle, EPOLL_CTL_ADD, sock, &event) == -1)
			{
				Net::CloseSocket(sock);

				client->OnConnectFailed();

				return false;
			}
			else
			{
				pipe->_isConnect = true;

				client->_pipe = pipe;

				client->OnConnect();
			}
		}
		else if (ret < 0 && errno != EINPROGRESS)
		{
			Net::CloseSocket(sock);

			client->OnConnectFailed();

			return false;
		}
		else
		{
			auto pipe = new TCPSessionPipe(client, sock, _handle);

			auto netEvent = new NetEvent(NET_EVENT_TYPE::CONNECT, sock, pipe);

			struct epoll_event event{ };

			event.events = EPOLLOUT;
			event.data.ptr = netEvent;

			if (epoll_ctl(_handle, EPOLL_CTL_ADD, sock, &event) == -1)
			{
				delete pipe;
				delete netEvent;

				Net::CloseSocket(sock);

				client->OnConnectFailed();

				return false;
			}
			else
			{
				client->_pipe = std::shared_ptr<TCPSessionPipe>(pipe);
			}
		}

#endif

		return true;
	}

	/**
	 *
	 * 启动tcp服务器
	 *
	 * @param server 服务器
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetManager::LaunchTCPServer(ITCPServer * server)
	{
		if (server == nullptr)
		{
			return false;
		}

		if (!Launch())
		{
			return false;
		}

		{
			std::vector<std::string> localHostList{ };

			if (Net::TraverseAddressFromHost(server->_localHost.c_str(), localHostList))
			{
				server->_localHost = std::move(localHostList.front());
			}
			else
			{
				server->OnError();

				return false;
			}
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		TINY_TOOLKIT_SOCKET_TYPE sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

#else

		TINY_TOOLKIT_SOCKET_TYPE sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#endif


		if (sock == TINY_TOOLKIT_SOCKET_INVALID)
		{
			server->OnError();

			return false;
		}

		if (!Net::EnableLinger(sock) ||
			!Net::EnableNoDelay(sock) ||
			!Net::EnableNonBlock(sock) ||
			!Net::EnableReuseAddress(sock))
		{
			Net::CloseSocket(sock);

			server->OnError();

			return false;
		}

		struct sockaddr_in localAddress{ };

		localAddress.sin_port = htons(server->_localPort);
		localAddress.sin_family = AF_INET;
		localAddress.sin_addr.s_addr = Net::AsNetByte(server->_localHost.c_str());

		if (::bind(sock, (struct sockaddr *)&localAddress, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			Net::CloseSocket(sock);

			server->OnError();

			return false;
		}

		if (::listen(sock, TINY_TOOLKIT_SOCKET_LISTEN_COUNT) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			Net::CloseSocket(sock);

			server->OnError();

			return false;
		}

		auto pipe = std::make_shared<TCPServerPipe>(server, sock, _handle);

		if (Net::GetLocalAddress(sock, pipe->_netEvent._address))
		{
			server->_localPort = ntohs(pipe->_netEvent._address.sin_port);
			server->_localHost = inet_ntoa(pipe->_netEvent._address.sin_addr);
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (CreateIoCompletionPort((HANDLE)sock, _handle, (ULONG_PTR)sock, 0) != _handle)
		{
			Net::CloseSocket(sock);

			server->OnError();

			return false;
		}

		if (pipe->AsyncAccept())
		{
			server->_pipe = pipe;
		}
		else
		{
			Net::CloseSocket(sock);

			server->OnError();

			return false;
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		struct kevent event[2]{ };

		EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&pipe->_netEvent);
		EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&pipe->_netEvent);

		if (kevent(_handle, event, 2, nullptr, 0, nullptr) == -1)
		{
			Net::CloseSocket(sock);

			server->OnError();

			return false;
		}
		else
		{
			server->_pipe = pipe;
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		struct epoll_event event{ };

		event.events = EPOLLIN;
		event.data.ptr = &pipe->_netEvent;

		if (epoll_ctl(_handle, EPOLL_CTL_ADD, sock, &event) == -1)
		{
			Net::CloseSocket(sock);

			server->OnError();

			return false;
		}
		else
		{
			server->_pipe = pipe;
		}

#endif

		return true;
	}

	/**
	 *
	 * app线程逻辑函数
	 *
	 */
	void NetManager::ThreadProcess()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		while (_status)
		{
			DWORD bytes = 0;

			NetEvent * eventValue = nullptr;

			TINY_TOOLKIT_SOCKET_TYPE socket = TINY_TOOLKIT_SOCKET_INVALID;

			if (GetQueuedCompletionStatus(_handle, &bytes, (PULONG_PTR)&socket, (LPOVERLAPPED *)&eventValue, 10))
			{
				if (eventValue && eventValue->_completer)
				{
					eventValue->_bytes = bytes;

					eventValue->_completer->OnCallback(eventValue, nullptr);
				}
			}
			else
			{
				if (GetLastError() == WAIT_TIMEOUT)
				{
					continue;
				}
				else
				{
					if (eventValue && eventValue->_completer)
					{
						eventValue->_bytes = bytes;

						eventValue->_completer->OnCallback(eventValue, nullptr);
					}
					else
					{
						TINY_TOOLKIT_ASSERT(false, ::GetLastError());

						return;
					}
				}
			}
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		struct kevent events[TINY_TOOLKIT_NET_COUNT]{ };

		while (_status)
		{
			struct timespec timeout{ };

			timeout.tv_sec = 0;
			timeout.tv_nsec = 10000000; /// 10毫秒

			int32_t count = kevent(_handle, nullptr, 0, events, TINY_TOOLKIT_NET_COUNT, &timeout);

			if (count == -1)
			{
				if (errno != EINTR)
				{
					TINY_TOOLKIT_ASSERT(false, strerror(errno));

					return;
				}

				continue;
			}

			for (int32_t i = 0; i < count; ++i)
			{
				auto * eventValue = reinterpret_cast<NetEvent *>(events[i].udata);

				if (eventValue && eventValue->_completer)
				{
					eventValue->_completer->OnCallback(eventValue, &events[i]);
				}
			}
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		struct epoll_event events[TINY_TOOLKIT_NET_COUNT]{ };

		while (_status)
		{
			int32_t count = epoll_wait(_handle, events, TINY_TOOLKIT_NET_COUNT, 10);

			if (count == -1)
			{
				if (errno != EINTR)
				{
					TINY_TOOLKIT_ASSERT(false, strerror(errno));

					return;
				}

				continue;
			}

			for (int32_t i = 0; i < count; ++i)
			{
				auto * eventValue = reinterpret_cast<NetEvent *>(events[i].data.ptr);

				if (eventValue && eventValue->_completer)
				{
					eventValue->_completer->OnCallback(eventValue, &events[i]);
				}
			}
		}

#endif
	}
}
