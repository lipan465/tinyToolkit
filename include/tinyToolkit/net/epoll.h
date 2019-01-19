#ifndef __TINY_TOOLKIT__NET__NETWORK__H__
#define __TINY_TOOLKIT__NET__NETWORK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 网络服务
 *
 */


#include "tcp.h"
#include "udp.h"

#include "../utilities/thread.h"
#include "../utilities/singleton.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetWorkManager : public ApplicationThread
	{
	public:
		/**
		 *
		 * 获取单例
		 *
		 * @return 单例对象引用
		 *
		 */
		static NetWorkManager & Instance()
		{
			return Singleton<NetWorkManager>::Instance();
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~NetWorkManager() override
		{
			StopApplicationThread();
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
		bool LaunchTCPClient(ITCPSession * client, const std::string & host, uint16_t port)
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

			struct sockaddr_in address = { 0 };

			address.sin_port = htons(client->_port);
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = Address::AsNetByte(client->_host);

			int32_t ret = ::connect(sock, (struct sockaddr *)&address, sizeof(address));

			if (ret == 0)
			{
				auto pipe = std::make_shared<TCPClientPipe>(_socket, sock, client, EVENT_TYPE::TRANSMIT);

				struct epoll_event event = { 0 };

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

				struct epoll_event event = { 0 };

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
		bool LaunchTCPServer(ITCPServer * server, const std::string & host, uint16_t port)
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

			struct sockaddr_in address = { 0 };

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

			struct epoll_event event = { 0 };

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

	protected:
		/**
		 *
		 * 启动
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch()
		{
			if (_socket == -1)
			{
				_socket = epoll_create(TINY_TOOLKIT_EPOLL_COUNT);

				if (_socket == -1)
				{
					TINY_TOOLKIT_DEBUG("create socket error : {}", strerror(errno))

					return false;
				}

				return StartApplicationThread();
			}

			return _socket != -1;
		}

		/**
		 *
		 * app线程逻辑函数
		 *
		 */
		void ThreadProcess() override
		{
			static struct epoll_event events[TINY_TOOLKIT_EPOLL_COUNT];

			while (ApplicationThreadStatus())
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

	protected:
		int32_t _socket{ -1 };
	};
}


#endif // __TINY_TOOLKIT__NET__NETWORK__H__
