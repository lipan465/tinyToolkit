/**
 *
 *  作者: hm
 *
 *  说明: 通讯管理器
 *
 */


#include "manager.h"

#include "../system/os.h"
#include "../debug/trace.h"
#include "../utilities/net.h"
#include "../utilities/singleton.h"


namespace tinyToolkit
{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	class FunctionAddress
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		FunctionAddress()
		{
			_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~FunctionAddress()
		{
			closesocket(_socket);
		}

		/**
		 *
		 * 获取accept函数
		 *
		 * @return 函数
		 *
		 */
		LPFN_ACCEPTEX GetAcceptEx()
		{
			LPFN_ACCEPTEX function = nullptr;

			if (_socket == TINY_TOOLKIT_SOCKET_INVALID)
			{
				
			}
			else
			{
				GUID guid = WSAID_ACCEPTEX;

				DWORD byte = 0;

				WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &function, sizeof(function), &byte, nullptr, nullptr);
			}
			
			return function;
		}

		/**
		 *
		 * 获取connect函数
		 *
		 * @return 函数
		 *
		 */
		LPFN_CONNECTEX GetConnectEx()
		{
			LPFN_CONNECTEX function = nullptr;

			if (_socket == TINY_TOOLKIT_SOCKET_INVALID)
			{

			}
			else
			{
				GUID guid = WSAID_CONNECTEX;

				DWORD byte = 0;

				WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &function, sizeof(function), &byte, nullptr, nullptr);
			}

			return function;
		}

	private:
		TINY_TOOLKIT_SOCKET_TYPE _socket{ TINY_TOOLKIT_SOCKET_INVALID };
	};

#endif

	/**
	 *
	 * 获取本地名称
	 *
	 * @param socket 句柄
	 * @param host 地址
	 * @param port 端口
	 *
	 * @return 是否获取成功
	 *
	 */
	static bool GetLocalName(TINY_TOOLKIT_SOCKET_TYPE socket, std::string & host, uint16_t & port)
	{
		struct sockaddr_in address { };

		std::size_t addressLen = sizeof(address);

		if (::getsockname(socket, (struct sockaddr *)&address, (socklen_t *)&addressLen) == 0)
		{
			host = inet_ntoa(address.sin_addr);

			port = ntohs(address.sin_port);

			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 *
	 * 获取单例
	 *
	 * @return 单例对象引用
	 *
	 */
	NetWorkManager & NetWorkManager::Instance()
	{
		return Singleton<NetWorkManager>::Instance();
	}

	/**
	 *
	 * 构造函数
	 *
	 */
	NetWorkManager::NetWorkManager()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		_managerHandle.handle = nullptr;

#else

		_handle.socket = TINY_TOOLKIT_SOCKET_INVALID;

#endif
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	NetWorkManager::~NetWorkManager()
	{
		if (_status)
		{
			_status = false;

			if (_thread.joinable())
			{
				_thread.join();
			}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (_managerHandle.handle)
			{
				::CloseHandle(_managerHandle.handle);

				_managerHandle.handle = nullptr;
			}

#else

			if (_handle.socket != TINY_TOOLKIT_SOCKET_INVALID)
			{
				::close(_handle.socket);

				_handle.socket = TINY_TOOLKIT_SOCKET_INVALID;
			}

#endif
		}
	}

	/**
	 *
	 * 启动udp客户端
	 *
	 * @param client 客户端
	 * @param host 主机地址
	 * @param port 主机端口
	 * @param sSize 发送缓冲区大小
	 * @param rSize 接受缓冲区大小
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetWorkManager::LaunchUDPClient(IUDPSession * client, const std::string & host, uint16_t port, std::size_t sSize, std::size_t rSize)
	{
		if (!Launch())
		{
			return false;
		}

		std::vector<std::string> hostList{ };

		if (!Net::TraverseAddressFromHost(host.c_str(), hostList))
		{
			client->OnConnectFailed();

			return false;
		}

		client->_remotePort = port;
		client->_remoteHost = hostList.front();

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#else

		TINY_TOOLKIT_SOCKET_TYPE sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (sock == TINY_TOOLKIT_SOCKET_INVALID)
		{
			client->OnConnectFailed();

			return false;
		}

		if (!Net::EnableNonBlock(sock) ||
			!Net::EnableReuseAddress(sock))
		{
			::close(sock);

			client->OnConnectFailed();

			return false;
		}

		struct sockaddr_in serverAddress{ };

		serverAddress.sin_port = htons(port);
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = Net::AsNetByte(host.c_str());

		if (::connect(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			::close(sock);

			client->OnConnectFailed();

			return false;
		}

		GetLocalName(sock, client->_localHost, client->_localPort);

		auto pipe = std::make_shared<UDPSessionPipe>(_handle, client, sock, sSize, rSize);

	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		struct kevent event[2]{ };

		EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&pipe->_netEvent);
		EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&pipe->_netEvent);

		if (kevent(_handle.socket, event, 2, nullptr, 0, nullptr) == -1)
		{
			::close(sock);

			client->OnConnectFailed();

			return false;
		}
		else
		{
			pipe->_isConnect = true;

			client->_pipe = pipe;

			client->Send("", 1);

			client->OnConnect();
		}

	#else

		struct epoll_event event{ };

		event.events = EPOLLIN;
		event.data.ptr = &pipe->_netEvent;

		if (epoll_ctl(_managerHandle.socket, EPOLL_CTL_ADD, sock, &event) == -1)
		{
			::close(sock);

			client->OnConnectFailed();

			return false;
		}
		else
		{
			pipe->_isConnect = true;

			client->_pipe = pipe;

			client->Send("", 1);

			client->OnConnect();
		}

	#endif

#endif

		return true;
	}

	/**
	 *
	 * 启动udp服务器
	 *
	 * @param server 服务器
	 * @param host 主机地址
	 * @param port 主机端口
	 * @param sSize 发送缓冲区大小
	 * @param rSize 接受缓冲区大小
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetWorkManager::LaunchUDPServer(IUDPServer * server, const std::string & host, uint16_t port, std::size_t sSize, std::size_t rSize)
	{
		if (!Launch())
		{
			return false;
		}

		std::vector<std::string> hostList{ };

		if (!Net::TraverseAddressFromHost(host.c_str(), hostList))
		{
			server->OnError();

			return false;
		}

		server->_port = port;
		server->_host = hostList.front();

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#else

		TINY_TOOLKIT_SOCKET_TYPE sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (sock == TINY_TOOLKIT_SOCKET_INVALID)
		{
			server->OnError();

			return false;
		}

		if (!Net::EnableNonBlock(sock) ||
			!Net::EnableReuseAddress(sock))
		{
			::close(sock);

			server->OnError();

			return false;
		}

		struct sockaddr_in address{ };

		address.sin_port = htons(server->_port);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Net::AsNetByte(server->_host.c_str());

		if (::bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr)) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			::close(sock);

			server->OnError();

			return false;
		}

		auto pipe = std::make_shared<UDPServerPipe>(_handle, server, sock, sSize, rSize);

	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		struct kevent event[2]{ };

		EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&pipe->_netEvent);
		EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&pipe->_netEvent);

		if (kevent(_handle.socket, event, 2, nullptr, 0, nullptr) == -1)
		{
			::close(sock);

			server->OnError();

			return false;
		}
		else
		{
			server->_pipe = pipe;
		}

	#else

		struct epoll_event event{ };

		event.events = EPOLLIN;
		event.data.ptr = &pipe->_netEvent;

		if (epoll_ctl(_managerHandle.socket, EPOLL_CTL_ADD, sock, &event) == -1)
		{
			::close(sock);

			server->OnError();

			return false;
		}
		else
		{
			server->_pipe = pipe;
		}

	#endif

#endif

		return true;
	}

	/**
	 *
	 * 启动tcp客户端
	 *
	 * @param client 客户端
	 * @param host 主机地址
	 * @param port 主机端口
	 * @param sSize 发送缓冲区大小
	 * @param rSize 接受缓冲区大小
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetWorkManager::LaunchTCPClient(ITCPSession * client, const std::string & host, uint16_t port, std::size_t sSize, std::size_t rSize)
	{
		if (!Launch())
		{
			return false;
		}

		std::vector<std::string> hostList{ };

		if (!Net::TraverseAddressFromHost(host.c_str(), hostList))
		{
			client->OnConnectFailed();

			return false;
		}

		client->_remotePort = port;
		client->_remoteHost = hostList.front();

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		TINY_TOOLKIT_SOCKET_TYPE sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

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
			::close(sock);

			client->OnConnectFailed();

			return false;
		}

		sockaddr_in bindAddress{ };

		bindAddress.sin_family = AF_INET;

		if (::bind(sock, (struct sockaddr *)&bindAddress, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			::close(sock);

			client->OnConnectFailed();

			return false;
		}

		if (CreateIoCompletionPort((HANDLE)sock, (HANDLE)_managerHandle.handle, (ULONG_PTR)socket, 0) != _managerHandle.handle)
		{
			::close(sock);

			client->OnConnectFailed();

			return false;
		}

		GetLocalName(sock, client->_localHost, client->_localPort);

		auto pipe = std::make_shared<TCPSessionPipe>(_managerHandle, client, sock, sSize, rSize);

		auto * netEvent = new NetEvent(NET_EVENT_TYPE::CONNECT, sock, pipe.get());

		DWORD bytes = 0;

		struct sockaddr_in serverAddress { };

		serverAddress.sin_port = htons(client->_remotePort);
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = Net::AsNetByte(client->_remoteHost.c_str());

		LPFN_CONNECTEX connectEx = Singleton<FunctionAddress>::Instance().GetConnectEx();

		if (connectEx(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in), nullptr, 0, &bytes, (LPOVERLAPPED)netEvent) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				::close(sock);

				delete netEvent;

				client->OnConnectFailed();

				return false;
			}
		}

		client->_pipe = pipe;

#else

		TINY_TOOLKIT_SOCKET_TYPE sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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
			::close(sock);

			client->OnConnectFailed();

			return false;
		}

		struct sockaddr_in serverAddress { };

		serverAddress.sin_port = htons(client->_remotePort);
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = Net::AsNetByte(client->_remoteHost.c_str());

		int32_t ret = ::connect(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in));

		if (ret == 0)
		{
			GetLocalName(sock, client->_localHost, client->_localPort);

			auto pipe = std::make_shared<TCPSessionPipe>(_handle, client, sock, sSize, rSize);

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&pipe->_netEvent);
			EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&pipe->_netEvent);

			if (kevent(_handle.socket, event, 2, nullptr, 0, nullptr) == -1)
			{
				::close(sock);

				client->OnConnectFailed();

				return false;
			}
			else
			{
				pipe->_isConnect = true;

				client->_pipe = pipe;

				client->OnConnect();
			}

		#else

			struct epoll_event event{ };

			event.events = EPOLLIN;
			event.data.ptr = &pipe->_netEvent;

			if (epoll_ctl(_managerHandle.socket, EPOLL_CTL_ADD, sock, &event) == -1)
			{
				::close(sock);

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
		}
		else if (ret < 0 && errno != EINPROGRESS)
		{
			::close(sock);

			client->OnConnectFailed();

			return false;
		}
		else
		{
			GetLocalName(sock, client->_localHost, client->_localPort);

			auto pipe = std::make_shared<TCPSessionPipe>(_handle, client, sock, sSize, rSize);

			auto * netEvent = new NetEvent(NET_EVENT_TYPE::CONNECT, sock, pipe.get());

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_DISABLE, 0, 0, (void *)netEvent);
			EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_ENABLE,  0, 0, (void *)netEvent);

			if (kevent(_handle.socket, event, 2, nullptr, 0, nullptr) == -1)
			{
				::close(sock);

				delete netEvent;

				client->OnConnectFailed();

				return false;
			}
			else
			{
				client->_pipe = pipe;
			}

		#else

			struct epoll_event event{ };

			event.events = EPOLLOUT;
			event.data.ptr = netEvent;

			if (epoll_ctl(_managerHandle.socket, EPOLL_CTL_ADD, sock, &event) == -1)
			{
				::close(sock);

				delete netEvent;

				client->OnConnectFailed();

				return false;
			}
			else
			{
				client->_pipe = pipe;
			}

		#endif
		}

#endif

		return true;
	}

	/**
	 *
	 * 启动tcp服务器
	 *
	 * @param server 服务器
	 * @param host 主机地址
	 * @param port 主机端口
	 * @param sSize 发送缓冲区大小
	 * @param rSize 接受缓冲区大小
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetWorkManager::LaunchTCPServer(ITCPServer * server, const std::string & host, uint16_t port, std::size_t sSize, std::size_t rSize)
	{
		if (!Launch())
		{
			return false;
		}

		std::vector<std::string> hostList{ };

		if (!Net::TraverseAddressFromHost(host.c_str(), hostList))
		{
			server->OnError();

			return false;
		}

		server->_port = port;
		server->_host = hostList.front();

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

#else

		TINY_TOOLKIT_SOCKET_TYPE sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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
			::close(sock);

			server->OnError();

			return false;
		}

		struct sockaddr_in address{ };

		address.sin_port = htons(server->_port);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Net::AsNetByte(server->_host.c_str());

		if (::bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			::close(sock);

			server->OnError();

			return false;
		}

		if (::listen(sock, 20) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			::close(sock);

			server->OnError();

			return false;
		}

		auto pipe = std::make_shared<TCPServerPipe>(_handle, server, sock, sSize, rSize);

	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		struct kevent event[2]{ };

		EV_SET(&event[0], sock, EVFILT_READ,  EV_ADD | EV_ENABLE,  0, 0, (void *)&pipe->_netEvent);
		EV_SET(&event[1], sock, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, (void *)&pipe->_netEvent);

		if (kevent(_handle.socket, event, 2, nullptr, 0, nullptr) == -1)
		{
			::close(sock);

			server->OnError();

			return false;
		}
		else
		{
			server->_pipe = pipe;
		}

	#else

		struct epoll_event event{ };

		event.events = EPOLLIN;
		event.data.ptr = &pipe->_netEvent;

		if (epoll_ctl(_managerHandle.socket, EPOLL_CTL_ADD, sock, &event) == -1)
		{
			::close(sock);

			server->OnError();

			return false;
		}
		else
		{
			server->_pipe = pipe;
		}

	#endif

#endif

		return true;
	}

	/**
	 *
	 * 启动
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetWorkManager::Launch()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (_managerHandle.handle == nullptr)
		{
			WSADATA wsaData;

			if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
			{
				TINY_TOOLKIT_ASSERT(false, "Start socket error : {}", OS::LastErrorMessage());

				return false;
			}

			if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
			{
				WSACleanup();

				TINY_TOOLKIT_ASSERT(false, "Socket version error : {}", OS::LastErrorMessage());

				return false;
			}

			_managerHandle.handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);

			if (_managerHandle.handle == nullptr)
			{
				TINY_TOOLKIT_ASSERT(false, "Create net manager event error : {}", OS::LastErrorMessage());

				return false;
			}

			_status = true;
			_thread = std::thread(&NetWorkManager::ThreadProcess, this);

			return true;
		}

		return _managerHandle.handle != nullptr;

#else

		if (_handle.socket == TINY_TOOLKIT_SOCKET_INVALID)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			_handle.socket = kqueue();

		#else

			_managerHandle.socket = epoll_create(TINY_TOOLKIT_NET_COUNT);

		#endif

			if (_handle.socket == TINY_TOOLKIT_SOCKET_INVALID)
			{
				TINY_TOOLKIT_ASSERT(false, "Create net manager event error : {}", OS::LastErrorMessage());

				return false;
			}

			_status = true;
			_thread = std::thread(&NetWorkManager::ThreadProcess, this);

			return true;
		}

		return _handle.socket != TINY_TOOLKIT_SOCKET_INVALID;

#endif
	}

	/**
	 *
	 * app线程逻辑函数
	 *
	 */
	void NetWorkManager::ThreadProcess()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		while (_status)
		{
			DWORD bytes = 0;

			NetEvent * eventValue = nullptr;

			TINY_TOOLKIT_SOCKET_TYPE socket = TINY_TOOLKIT_SOCKET_INVALID;

			if (GetQueuedCompletionStatus(_managerHandle.handle, &bytes, (PULONG_PTR)&socket, (LPOVERLAPPED *)&eventValue, 10) == 0)
			{
				if (GetLastError() != WAIT_TIMEOUT)
				{
					TINY_TOOLKIT_ASSERT(false, OS::LastErrorMessage());

					return;
				}

				continue;
			}

			if (eventValue && eventValue->_completer)
			{
				eventValue->_bytes = bytes;

				eventValue->_completer->OnCallback(eventValue, nullptr);
			}
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		struct kevent events[TINY_TOOLKIT_NET_COUNT]{ };

		while (_status)
		{
			struct timespec timeout{ };

			timeout.tv_sec = 0;
			timeout.tv_nsec = 10000000; /// 10毫秒

			int32_t count = kevent(_handle.socket, nullptr, 0, events, TINY_TOOLKIT_NET_COUNT, &timeout);

			if (count == -1)
			{
				if (errno != EINTR)
				{
					TINY_TOOLKIT_ASSERT(false, OS::LastErrorMessage());

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

#else

		struct epoll_event events[TINY_TOOLKIT_NET_COUNT]{ };

		while (_status)
		{
			int32_t count = epoll_wait(_managerHandle.socket, events, TINY_TOOLKIT_NET_COUNT, 10);

			if (count == -1)
			{
				if (errno != EINTR)
				{
					TINY_TOOLKIT_ASSERT(false, OS::LastErrorMessage())

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
