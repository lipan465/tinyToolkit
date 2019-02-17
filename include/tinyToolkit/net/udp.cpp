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
#include "../utilities/time.h"
#include "../container/operator.h"


namespace tinyToolkit
{
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

			/// todo

#else

			epoll_ctl(_managerSocket, EPOLL_CTL_DEL, _sessionSocket, nullptr);

#endif

			::close(_sessionSocket);

			_sessionSocket = -1;

			if (_session)
			{
				_session->OnDisconnect();
			}

			tinyToolkit::ContainerOperator::Clear(_sendQueue);
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

			/// todo

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

		/// todo

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

		/// todo

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
				address.sin_addr.s_addr = tinyToolkit::Net::AsNetByte(_session->_localHost);

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
				address.sin_addr.s_addr = tinyToolkit::Net::AsNetByte(_sendQueue.front()._host);

				while (_isConnect && count < length)
				{
					auto len = ::sendto(_sessionSocket, value + count, length - count, 0, (struct sockaddr *)&address, sizeof(struct sockaddr_in));

					if (len > 0)
					{
						count += len;

						if (count == length)
						{
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

							_sendQueue.pop();
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
				TINY_TOOLKIT_DEBUG("UDPSessionPipe type error : {}", netEvent->_socket)

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

			/// todo

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

		/// todo

#else

		auto currentEvent = reinterpret_cast<const struct epoll_event *>(sysEvent);

		if (currentEvent->events & EPOLLIN)
		{

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
				TINY_TOOLKIT_DEBUG("UDPServerPipe type error : {}", netEvent->_socket)

				break;
			}
		}
	}
}
