/**
 *
 *  作者: hm
 *
 *  说明: 管理器
 *
 */


#include "socket.h"
#include "manager.h"

#include "../debug/trace.h"
#include "../utilities/address.h"


namespace tinyToolkit
{
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
	 * 析构函数
	 *
	 */
	NetWorkManager::~NetWorkManager()
	{
		if (_status)
		{
			if (_thread.joinable())
			{
				_status = false;

				_thread.join();
			}
		}
	}

	/**
	 *
	 * 启动tcp客户端
	 *
	 * @param client 客户端
	 * @param host 主机地址
	 * @param port 主机端口
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetWorkManager::LaunchTCPClient(ITCPSession * client, const std::string & host, uint16_t port)
	{
		if (!Launch())
		{
			return false;
		}

		int32_t sock = ::socket(AF_INET, SOCK_STREAM, 0);

		if (sock == -1)
		{
			TINY_TOOLKIT_DEBUG("socket : {}", strerror(errno))

			return false;
		}

		client->_host = Address::ParseHost(host);
		client->_port = port;

		if (!Socket::SetNonBlocking(sock))
		{
			::close(sock);

			client->OnConnectFailed();

			return false;
		}

		struct sockaddr_in address{ };

		address.sin_port = htons(client->_port);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Address::AsNetByte(client->_host);

		int32_t ret = ::connect(sock, (struct sockaddr *)&address, sizeof(address));

		if (ret == 0)
		{
			auto pipe = std::make_shared<TCPClientPipe>(_socket, sock, client, EVENT_TYPE::TRANSMIT);

			struct epoll_event event{ };

			event.events = EPOLLIN;
			event.data.ptr = (void *)&pipe->eventValue;

			if (epoll_ctl(_socket, EPOLL_CTL_ADD, sock, &event) == -1)
			{
				::close(sock);

				client->OnConnectFailed();

				return false;
			}
			else
			{
				client->_pipe = pipe;

				pipe->isConnect = true;

				client->OnConnect();
			}
		}
		else if (ret < 0 && errno != EINPROGRESS)
		{
			::close(sock);

			client->OnConnectFailed();

			return false;
		}
		else
		{
			auto pipe = std::make_shared<TCPClientPipe>(_socket, sock, client, EVENT_TYPE::CONNECT);

			struct epoll_event event{ };

			event.events = EPOLLOUT;
			event.data.ptr = (void *)&pipe->eventValue;

			if (epoll_ctl(_socket, EPOLL_CTL_ADD, sock, &event) == -1)
			{
				::close(sock);

				client->OnConnectFailed();

				return false;
			}
			else
			{
				client->_pipe = pipe;
			}
		}

		return true;
	}

	/**
	 *
	 * 启动tcp服务器
	 *
	 * @param server 服务器
	 * @param host 主机地址
	 * @param port 主机端口
	 *
	 * @return 是否启动成功
	 *
	 */
	bool NetWorkManager::LaunchTCPServer(ITCPServer * server, const std::string & host, uint16_t port)
	{
		if (!Launch())
		{
			return false;
		}

		int32_t sock = ::socket(AF_INET, SOCK_STREAM, 0);

		if (sock == -1)
		{
			TINY_TOOLKIT_DEBUG("socket : {}", strerror(errno))

			return false;
		}

		server->_host = Address::ParseHost(host);
		server->_port = port;

		if (!Socket::SetNonBlocking(sock) || !Socket::SetReuseAddress(sock))
		{
			::close(sock);

			server->OnError();

			return false;
		}

		struct sockaddr_in address{ };

		address.sin_port = htons(server->_port);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Address::AsNetByte(server->_host);

		if (::bind(sock, (sockaddr *)&address, sizeof(address)) == -1)
		{
			::close(sock);

			server->OnError();

			return false;
		}

		if (::listen(sock, 20) == -1)
		{
			::close(sock);

			server->OnError();

			return false;
		}

		auto pipe = new TCPServerPipe(_socket, sock, server, EVENT_TYPE::ACCEPT);

		struct epoll_event event{ };

		event.events = EPOLLIN;
		event.data.ptr = (void *)&pipe->eventValue;

		if (epoll_ctl(_socket, EPOLL_CTL_ADD, sock, &event) == -1)
		{
			delete pipe;

			::close(sock);

			server->OnError();

			return false;
		}
		else
		{
			server->_pipe = pipe;
		}

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
		if (_socket == -1)
		{
			_socket = epoll_create(TINY_TOOLKIT_EPOLL_COUNT);

			if (_socket == -1)
			{
				TINY_TOOLKIT_DEBUG("create socket error : {}", strerror(errno))

				return false;
			}

			_status = true;
			_thread = std::thread(&NetWorkManager::ThreadProcess, this);

			return true;
		}

		return _socket != -1;
	}

	/**
	 *
	 * app线程逻辑函数
	 *
	 */
	void NetWorkManager::ThreadProcess()
	{
		static struct epoll_event events[TINY_TOOLKIT_EPOLL_COUNT];

		while (_status)
		{
			int32_t count = epoll_wait(_socket, events, TINY_TOOLKIT_EPOLL_COUNT, 1);

			if (count == -1)
			{
				TINY_TOOLKIT_DEBUG("epoll_wait error : {}", strerror(errno))

				return;
			}

			for (int32_t i = 0; i < count; ++i)
			{
				struct epoll_event & currentEvent = events[i];

				auto * eventValue = (struct EventValue *)currentEvent.data.ptr;

				if (eventValue && eventValue->callback)
				{
					eventValue->callback(eventValue, currentEvent);
				}
			}
		}
	}
}
