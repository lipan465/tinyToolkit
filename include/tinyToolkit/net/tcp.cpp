/**
 *
 *  作者: hm
 *
 *  说明: tcp服务
 *
 */


#include "tcp.h"
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
	TCPClientPipe::TCPClientPipe(int32_t managerSocket, int32_t sessionSocket, ITCPSession * session, EVENT_TYPE type) : _managerSocket(managerSocket),
																														 _sessionSocket(sessionSocket),
																														 _session(session)
	{
		eventValue.type = type;
		eventValue.socket = sessionSocket;
		eventValue.callback = std::bind(&TCPClientPipe::OnCallBack, this, std::placeholders::_1, std::placeholders::_2);
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void TCPClientPipe::Close()
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
	 * @param value 待发送数据
	 * @param size 待发送数据长度
	 *
	 */
	void TCPClientPipe::Send(const void * value, std::size_t size)
	{
		if (isConnect)
		{
			_sendQueue.emplace(_session->_host.c_str(), _session->_port, value, size);

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
	void TCPClientPipe::OnCallBack(const EventValue * currentEventValue, const struct epoll_event & currentEvent)
	{
		switch(currentEventValue->type)
		{
			case EVENT_TYPE::CONNECT:
			{
				if (!Socket::SetNodelay(_sessionSocket))
				{
					Close();

					if (_session)
					{
						_session->OnConnectFailed();
					}

					return;
				}

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
							auto len = ::recv(_sessionSocket, buffer, sizeof(buffer), 0);

							if (len < 0 && errno == EAGAIN)
							{
								return;
							}
							else if (len > 0)
							{
								if (_session)
								{
									_session->OnReceive(buffer, static_cast<std::size_t>(len));
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

						while (isConnect && count < length)
						{
							auto len = ::send(_sessionSocket, value + count, length - count, 0);

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
				TINY_TOOLKIT_DEBUG("TCPClientPipe type error : {}", currentEventValue->socket)

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
	TCPServerPipe::TCPServerPipe(int32_t managerSocket, int32_t sessionSocket, ITCPServer * server, EVENT_TYPE type) : _managerSocket(managerSocket),
																													   _sessionSocket(sessionSocket),
																													   _server(server)
	{
		eventValue.type = type;
		eventValue.socket = sessionSocket;
		eventValue.callback = std::bind(&TCPServerPipe::OnCallBack, this, std::placeholders::_1, std::placeholders::_2);
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void TCPServerPipe::Close()
	{
		if (_sessionSocket != -1)
		{
			epoll_ctl(_managerSocket, EPOLL_CTL_DEL, _sessionSocket, nullptr);

			::close(_sessionSocket);

			_sessionSocket = -1;

			if (_server)
			{
				_server->_pipe = nullptr;

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
	 *
	 */
	void TCPServerPipe::Send(const void * /* value */, std::size_t /* size */)
	{

	}

	/**
	 *
	 * 回调函数
	 *
	 * @param currentEventValue 当前事件数据
	 * @param currentEvent 当前事件
	 *
	 */
	void TCPServerPipe::OnCallBack(const EventValue * currentEventValue, const struct epoll_event & currentEvent)
	{
		switch (currentEventValue->type)
		{
			case EVENT_TYPE::ACCEPT:
			{
				if (currentEvent.events & EPOLLIN)
				{
					struct sockaddr_in address{ };

					std::size_t addressLen = sizeof(address);

					int32_t sock = ::accept(_sessionSocket, (struct sockaddr *)&address, (socklen_t *)&addressLen);

					if (sock >= 0)
					{
						if (!Socket::SetNodelay(sock) || !Socket::SetNonBlocking(sock) || !Socket::SetReuseAddress(sock))
						{
							::close(sock);

							return;
						}

						uint16_t port = ntohs(address.sin_port);

						std::string host = inet_ntoa(address.sin_addr);

						auto session = _server->OnNewConnect(host, port);

						if (session)
						{
							session->_host = host;
							session->_port = port;

							auto pipe = std::make_shared<TCPClientPipe>(_managerSocket, sock, session, EVENT_TYPE::TRANSMIT);

							struct epoll_event event{ };

							event.events = EPOLLIN;
							event.data.ptr = (void *)&pipe->eventValue;

							if (epoll_ctl(_managerSocket, EPOLL_CTL_ADD, sock, &event) == -1)
							{
								::close(sock);

								_server->OnSessionError(session);

								return;
							}
							else
							{
								pipe->isConnect = true;

								session->_pipe = pipe;

								session->OnConnect();
							}
						}
					}
				}

				break;
			}

			default:
			{
				TINY_TOOLKIT_DEBUG("TCPServerPipe type error : {}", currentEventValue->socket)

				break;
			}
		}
	}
}
