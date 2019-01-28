/**
 *
 *  作者: hm
 *
 *  说明: udp服务
 *
 */


#include "udp.h"
#include "socket.h"

#include "../debug/trace.h"
#include "../utilities/time.h"
#include "../utilities/address.h"
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
	 * @param type 事件类型
	 *
	 */
	UDPClientPipe::UDPClientPipe(int32_t managerSocket, int32_t sessionSocket, IUDPSession * session, EVENT_TYPE type) : _managerSocket(managerSocket),
																														 _sessionSocket(sessionSocket),
																														 _session(session)
	{
		eventValue.type = type;
		eventValue.socket = sessionSocket;
		eventValue.callback = std::bind(&UDPClientPipe::OnCallBack, this, std::placeholders::_1, std::placeholders::_2);
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void UDPClientPipe::Close()
	{
		if (_sessionSocket != -1)
		{
			isConnect = false;

			epoll_ctl(_managerSocket, EPOLL_CTL_DEL, _sessionSocket, nullptr);

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
	void UDPClientPipe::Send(const char * host, uint16_t port, const void * value, std::size_t size)
	{
		if (isConnect)
		{
			_sendQueue.emplace(host, port, value, size);

			struct epoll_event event{ };

			event.events = EPOLLIN | EPOLLOUT;
			event.data.ptr = (void *)&eventValue;

			if (epoll_ctl(_managerSocket, EPOLL_CTL_MOD, _sessionSocket, &event) == -1)
			{
				Close();
			}
		}
	}

	/**
	 *
	 * 回调函数
	 *
	 * @param currentEventValue 当前事件数据
	 * @param currentEvent 当前事件
	 *
	 */
	void UDPClientPipe::OnCallBack(const EventValue * currentEventValue, const struct epoll_event & currentEvent)
	{
		switch(currentEventValue->type)
		{
			case EVENT_TYPE::CONNECT:
			{
				epoll_ctl(_managerSocket, EPOLL_CTL_DEL, _sessionSocket, nullptr);

				if (currentEvent.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
				{
					Close();

					if (_session)
					{
						_session->OnConnectFailed();
					}

					return;
				}

				if (currentEvent.events & EPOLLOUT)
				{
					struct epoll_event event{ };

					event.events = EPOLLIN;
					event.data.ptr = (void *)&eventValue;

					if (epoll_ctl(_managerSocket, EPOLL_CTL_ADD, _sessionSocket, &event) == -1)
					{
						Close();

						if (_session)
						{
							_session->OnConnectFailed();
						}

						return;
					}
					else
					{
						isConnect = true;

						eventValue.type = EVENT_TYPE::TRANSMIT;

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

					return;
				}

				break;
			}

			case EVENT_TYPE::TRANSMIT:
			{
				if (currentEvent.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
				{
					Close();

					return;
				}
				else
				{
					if (!isConnect)
					{
						return;
					}

					if (currentEvent.events & EPOLLIN)
					{
						auto tick = Time::Microseconds();

						static char buffer[TINY_TOOLKIT_MB];

						while (isConnect && Time::Microseconds() - tick <= 1000)
						{
							struct sockaddr_in address{ };

							address.sin_port = htons(_session->_port);
							address.sin_family = AF_INET;
							address.sin_addr.s_addr = tinyToolkit::Address::AsNetByte(_session->_host);

							std::size_t addressLen = sizeof(struct sockaddr);

							auto len = ::recvfrom(_sessionSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, (socklen_t *)&addressLen);

							if (len < 0 && errno == EAGAIN)
							{
								return;
							}
							else if (len > 0)
							{
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

					if (currentEvent.events & EPOLLOUT)
					{
						std::size_t count = 0;

						std::size_t length = _sendQueue.front().value.size();

						const char * value = _sendQueue.front().value.c_str();

						struct sockaddr_in address{ };

						address.sin_port = htons(_sendQueue.front().port);
						address.sin_family = AF_INET;
						address.sin_addr.s_addr = tinyToolkit::Address::AsNetByte(_sendQueue.front().host);

						while (isConnect && count < length)
						{
							auto len = ::sendto(_sessionSocket, value + count, length - count, 0, (struct sockaddr *)&address, sizeof(struct sockaddr_in));

							if (len > 0)
							{
								count += len;

								if (count == length)
								{
									struct epoll_event event{ };

									event.events = EPOLLIN;
									event.data.ptr = (void *)&eventValue;

									if (epoll_ctl(_managerSocket, EPOLL_CTL_MOD, _sessionSocket, &event) == -1)
									{
										Close();

										return;
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

					if (!isConnect)
					{
						Close();

						return;
					}
				}

				break;
			}

			default:
			{
				TINY_TOOLKIT_DEBUG("UDPClientPipe type error : {}", currentEventValue->socket)

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
	 * @param session 会话
	 * @param type 事件类型
	 *
	 */
	UDPServerPipe::UDPServerPipe(int32_t managerSocket, int32_t sessionSocket, IUDPServer * server, EVENT_TYPE type) : _managerSocket(managerSocket),
																													   _sessionSocket(sessionSocket),
																													   _server(server)
	{
		eventValue.type = type;
		eventValue.socket = _sessionSocket;
		eventValue.callback = std::bind(&UDPServerPipe::OnCallBack, this, std::placeholders::_1, std::placeholders::_2);
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
			isConnect = false;

			epoll_ctl(_managerSocket, EPOLL_CTL_DEL, _sessionSocket, nullptr);

			::close(_sessionSocket);

			_sessionSocket = -1;

			if (_server)
			{
				_server->OnDisconnect();
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
	void UDPServerPipe::Send(const char * host, uint16_t port, const void * value, std::size_t size)
	{
		if (isConnect)
		{
			_sendQueue.emplace(host, port, value, size);

			struct epoll_event event{ };

			event.events = EPOLLIN | EPOLLOUT;
			event.data.ptr = (void *)&eventValue;

			if (epoll_ctl(_managerSocket, EPOLL_CTL_MOD, _sessionSocket, &event) == -1)
			{
				Close();
			}
		}
	}

	/**
	 *
	 * 回调函数
	 *
	 * @param currentEventValue 当前事件数据
	 * @param currentEvent 当前事件
	 *
	 */
	void UDPServerPipe::OnCallBack(const EventValue * currentEventValue, const struct epoll_event & currentEvent)
	{
		switch(currentEventValue->type)
		{
			case EVENT_TYPE::CONNECT:
			{
				epoll_ctl(_managerSocket, EPOLL_CTL_DEL, _sessionSocket, nullptr);

				if (currentEvent.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
				{
					Close();

					if (_server)
					{
						_server->OnConnectFailed();
					}

					return;
				}

				if (currentEvent.events & EPOLLOUT)
				{
					struct epoll_event event{ };

					event.events = EPOLLIN;
					event.data.ptr = (void *)&eventValue;

					if (epoll_ctl(_managerSocket, EPOLL_CTL_ADD, _sessionSocket, &event) == -1)
					{
						Close();

						if (_server)
						{
							_server->OnConnectFailed();
						}

						return;
					}
					else
					{
						isConnect = true;

						eventValue.type = EVENT_TYPE::TRANSMIT;

						if (_server)
						{
							_server->OnConnect();
						}
					}
				}
				else
				{
					Close();

					if (_server)
					{
						_server->OnConnectFailed();
					}

					return;
				}

				break;
			}

			case EVENT_TYPE::TRANSMIT:
			{
				if (currentEvent.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
				{
					Close();

					return;
				}
				else
				{
					if (!isConnect)
					{
						return;
					}

					if (currentEvent.events & EPOLLIN)
					{
						auto tick = Time::Microseconds();

						static char buffer[TINY_TOOLKIT_MB];

						while (isConnect && Time::Microseconds() - tick <= 1000)
						{
							struct sockaddr_in address{ };

							address.sin_port = htons(_server->_port);
							address.sin_family = AF_INET;
							address.sin_addr.s_addr = tinyToolkit::Address::AsNetByte(_server->_host);

							std::size_t addressLen = sizeof(struct sockaddr);

							auto len = ::recvfrom(_sessionSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, (socklen_t *)&addressLen);

							if (len < 0 && errno == EAGAIN)
							{
								return;
							}
							else if (len > 0)
							{
								if (_server)
								{
									_server->OnReceive(inet_ntoa(address.sin_addr), ntohs(address.sin_port), buffer, static_cast<std::size_t>(len));
								}
							}
							else
							{
								Close();

								return;
							}
						}
					}

					if (currentEvent.events & EPOLLOUT)
					{
						std::size_t count = 0;

						std::size_t length = _sendQueue.front().value.size();

						const char * value = _sendQueue.front().value.c_str();

						struct sockaddr_in address{ };

						address.sin_port = htons(_sendQueue.front().port);
						address.sin_family = AF_INET;
						address.sin_addr.s_addr = tinyToolkit::Address::AsNetByte(_sendQueue.front().host);

						while (isConnect && count < length)
						{
							auto len = ::sendto(_sessionSocket, value + count, length - count, 0, (struct sockaddr *)&address, sizeof(struct sockaddr_in));

							if (len > 0)
							{
								count += len;

								if (count == length)
								{
									struct epoll_event event{ };

									event.events = EPOLLIN;
									event.data.ptr = (void *)&eventValue;

									if (epoll_ctl(_managerSocket, EPOLL_CTL_MOD, _sessionSocket, &event) == -1)
									{
										Close();

										return;
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

					if (!isConnect)
					{
						Close();

						return;
					}
				}

				break;
			}

			default:
			{
				TINY_TOOLKIT_DEBUG("UDPServerPipe type error : {}", currentEventValue->socket)

				break;
			}
		}
	}
}
