/**
 *
 *  作者: hm
 *
 *  说明: tcp
 *
 */


#include "tcp.h"

#include "../utilities/net.h"
#include "../utilities/time.h"
#include "../utilities/string.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param socket 句柄
	 * @param session 会话
	 * @param monitor 监控
	 *
	 */
	TCPSessionPipe::TCPSessionPipe(std::shared_ptr<NetSocket> socket, ITCPSession * session, NetEventMonitor * monitor) : _cache(session->_cacheSize),
	                                                                                                                      _socket(std::move(socket)),
	                                                                                                                      _session(session),
	                                                                                                                      _monitor(monitor)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		_sendContext.type = NetContext::TYPE::SEND;
		_sendContext.socket = _socket->Handle();
		_sendContext.completer = this;

		_receiveContext.type = NetContext::TYPE::RECEIVE;
		_receiveContext.socket = _socket->Handle();
		_receiveContext.completer = this;

#else

		_ioContext.type = NetContext::TYPE::IO;
		_ioContext.socket = _socket->Handle();
		_ioContext.completer = this;

#endif
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void TCPSessionPipe::Close()
	{
		if (!_socket->IsValid())
		{
			return;
		}

		_isConnect = false;

		if (!_monitor->RemoveEvent(SocketHandle(), nullptr))
		{
			_session->OnEventError();
		}

		_socket->Close();

		_session->OnDisconnect();
	}

	/**
	 *
	 * 接受连接
	 *
	 * @return 是否连接成功
	 *
	 */
	bool TCPSessionPipe::Accept()
	{
		return true;
	}

	/**
	 *
	 * 接收数据
	 *
	 * @return 是否接收成功
	 *
	 */
	bool TCPSessionPipe::Receive()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (_socket->Receive(_receiveContext.cache, sizeof(_receiveContext.cache), &_receiveContext) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				return false;
			}
		}

#else

		int32_t len = _socket->Receive(_ioContext.cache, sizeof(_ioContext.cache), nullptr);

		if (len > 0)
		{
			_ioContext.bytes = static_cast<std::size_t>(len);
		}
		else
		{
			_ioContext.bytes = 0;

			if (errno != EAGAIN)
			{
				return false;
			}
		}

#endif

		return true;
	}

	/**
	 *
	 * 发送数据
	 *
	 * @param buffer 待发送数据缓冲区指针
	 * @param length 待发送数据缓冲区长度
	 *
	 * @return 是否发送成功
	 *
	 */
	bool TCPSessionPipe::Send(const char * buffer, std::size_t length)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (_socket->Send(buffer, length, &_sendContext) == TINY_TOOLKIT_SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				return false;
			}
		}

#else

		int32_t len = _socket->Send(buffer, length, nullptr);

		if (len > 0)
		{
			_ioContext.bytes = static_cast<std::size_t>(len);
		}
		else
		{
			_ioContext.bytes = 0;

			if (errno != EAGAIN)
			{
				return false;
			}
		}

#endif

		return true;
	}

	/**
	 *
	 * 追加数据
	 *
	 * @param buffer 待追加数据缓冲区指针
	 * @param length 待追加数据缓冲区长度
	 *
	 * @return 是否追加成功
	 *
	 */
	bool TCPSessionPipe::Append(const void * buffer, std::size_t length)
	{
		if (!_isConnect)
		{
			return false;
		}

		if (buffer == nullptr || length == 0)
		{
			return false;
		}

		std::lock_guard<std::mutex> lock(_mutex);

		_messageQueue.push(std::make_shared<NetMessage>(buffer, length));

		if (!_isSend)
		{
			_isSend = true;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			auto & message = _messageQueue.front();

			if (!Send(message->buffer, message->length))
			{
				_session->OnSendFailed();

				Close();

				return false;
			}

#else

			if (_socket->IsValid() && !_monitor->ModifyEvent(SocketHandle(), &_ioContext, true, true))
			{
				_session->OnEventError();

				Close();

				return false;
			}

#endif
		}

		return true;
	}

	/**
	 *
	 * 逻辑处理
	 *
	 * @param netContext 网络上下文
	 * @param sysContext 系统上下文
	 *
	 */
	void TCPSessionPipe::Logic(NetContext * netContext, void * sysContext)
	{
		switch (netContext->type)
		{
			case NetContext::TYPE::IO:
			{
				DoIO(netContext, sysContext);

				break;
			}

			case NetContext::TYPE::SEND:
			{
				DoSend(netContext, sysContext);

				break;
			}

			case NetContext::TYPE::CONNECT:
			{
				DoConnect(netContext, sysContext);

				break;
			}

			case NetContext::TYPE::RECEIVE:
			{
				DoReceive(netContext, sysContext);

				break;
			}

			default:
			{
				throw std::logic_error("TCPSessionPipe type error");
			}
		}
	}

	/**
	 *
	 * 剩余消息个数
	 *
	 * @return 剩余消息个数
	 *
	 */
	std::size_t TCPSessionPipe::RemainMessageCount()
	{
		return _messageQueue.size();
	}

	/**
	 *
	 * 句柄
	 *
	 * @return 句柄
	 *
	 */
	TINY_TOOLKIT_SOCKET_TYPE TCPSessionPipe::SocketHandle()
	{
		return _socket->Handle();
	}

	/**
	 *
	 * 交互处理
	 *
	 * @param netContext 网络上下文
	 * @param sysContext 系统上下文
	 *
	 */
	void TCPSessionPipe::DoIO(NetContext * netContext, void * sysContext)
	{
		(void)netContext;
		(void)sysContext;

		if (!_isConnect)
		{
			return;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto context = reinterpret_cast<const struct kevent *>(sysContext);

		if (context->flags & EV_ERROR)
		{
			Close();

			return;
		}

		if (context->filter & EVFILT_READ)
		{
			auto lastTime = Time::Microseconds();

			do
			{
				if (!Receive())
				{
					_session->OnReceiveFailed();

					Close();

					return;
				}

				if (_ioContext.bytes == 0)
				{
					break;
				}

				if (!_cache.Append(_ioContext.cache, _ioContext.bytes))
				{
					if (_session->OnCacheFull())
					{
						Close();

						return;
					}
				}

				_cache.Displacement(_session->OnReceive(_cache.Value(), _cache.Length()));
			}
			while ((Time::Microseconds() - lastTime) <= 1000);
		}

		if (context->filter & EVFILT_WRITE)
		{
			auto & message = _messageQueue.front();

			while (message->offset < message->length)
			{
				if (!Send(message->buffer + message->offset, message->length - message->offset))
				{
					_session->OnSendFailed();

					Close();

					return;
				}

				if (_ioContext.bytes == 0)
				{
					break;
				}

				message->offset += _ioContext.bytes;

				if (message->offset == message->length)
				{
					_messageQueue.pop();

					_session->OnSend();

					break;
				}
				else if (message->offset > message->length)
				{
					throw std::length_error(String::Format("send message error [offset={}, length={}]", message->offset, message->length));
				}
			}

			std::lock_guard<std::mutex> lock(_mutex);

			if (_messageQueue.empty())
			{
				_isSend = false;

				if (_socket->IsValid() && !_monitor->ModifyEvent(SocketHandle(), &_ioContext, true, false))
				{
					_session->OnEventError();

					Close();

					return;
				}
			}
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		auto context = reinterpret_cast<const struct epoll_event *>(sysContext);

		if (context->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
		{
			Close();

			return;
		}

		if (context->events & EPOLLIN)
		{
			auto lastTime = Time::Microseconds();

			do
			{
				if (!Receive())
				{
					_session->OnReceiveFailed();

					Close();

					return;
				}

				if (_ioContext.bytes == 0)
				{
					break;
				}

				if (!_cache.Append(_ioContext.cache, _ioContext.bytes))
				{
					if (_session->OnCacheFull())
					{
						Close();

						return;
					}
				}

				_cache.Displacement(_session->OnReceive(_cache.Value(), _cache.Length()));
			}
			while ((Time::Microseconds() - lastTime) <= 1000);
		}

		if (context->events & EPOLLOUT)
		{
			auto & message = _messageQueue.front();

			while (message->offset < message->length)
			{
				if (!Send(message->buffer + message->offset, message->length - message->offset))
				{
					_session->OnSendFailed();

					Close();

					return;
				}

				if (_ioContext.bytes == 0)
				{
					break;
				}

				message->offset += _ioContext.bytes;

				if (message->offset == message->length)
				{
					_messageQueue.pop();

					_session->OnSend();

					break;
				}
				else if (message->offset > message->length)
				{
					throw std::length_error(String::Format("send message error [offset={}, length={}]", message->offset, message->length));
				}
			}

			std::lock_guard<std::mutex> lock(_mutex);

			if (_messageQueue.empty())
			{
				_isSend = false;

				if (_socket->IsValid() && !_monitor->ModifyEvent(SocketHandle(), &_ioContext, true, false))
				{
					_session->OnEventError();

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
	 * @param netContext 网络上下文
	 * @param sysContext 系统上下文
	 *
	 */
	void TCPSessionPipe::DoSend(NetContext * netContext, void * sysContext)
	{
		(void)netContext;
		(void)sysContext;

		if (!_isConnect)
		{
			return;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (netContext->bytes == 0)
		{
			_session->OnSendFailed();

			Close();

			return;
		}

		_messageQueue.pop();

		_session->OnSend();

		{
			std::lock_guard<std::mutex> lock(_mutex);

			if (_messageQueue.empty())
			{
				_isSend = false;

				return;
			}
		}

		auto & message = _messageQueue.front();

		if (!Send(message->buffer, message->length))
		{
			_session->OnSendFailed();

			Close();

			return;
		}

#endif
	}

	/**
	 *
	 * 接收处理
	 *
	 * @param netContext 网络上下文
	 * @param sysContext 系统上下文
	 *
	 */
	void TCPSessionPipe::DoReceive(NetContext * netContext, void * sysContext)
	{
		(void)netContext;
		(void)sysContext;

		if (!_isConnect)
		{
			return;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (netContext->bytes == 0)
		{
			_session->OnReceiveFailed();

			Close();

			return;
		}

		if (!_cache.Append(netContext->cache, netContext->bytes))
		{
			if (_session->OnCacheFull())
			{
				Close();

				return;
			}
		}

		_cache.Displacement(_session->OnReceive(_cache.Value(), _cache.Length()));

		if (!Receive())
		{
			_session->OnReceiveFailed();

			Close();

			return;
		}

#endif
	}

	/**
	 *
	 * 连接处理
	 *
	 * @param netContext 网络上下文
	 * @param sysContext 系统上下文
	 *
	 */
	void TCPSessionPipe::DoConnect(NetContext * netContext, void * sysContext)
	{
		(void)netContext;
		(void)sysContext;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (setsockopt(_socket->Handle(), SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, nullptr, 0) == -1)
		{
			_session->OnConnectFailed();

			Close();

			return;
		}

		netContext->type = NetContext::TYPE::SEND;

		_isConnect = true;

		_session->OnConnect();

		if (!Receive())
		{
			_session->OnReceiveFailed();

			Close();

			return;
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto context = reinterpret_cast<const struct kevent *>(sysContext);

		if (context->flags & EV_ERROR)
		{
			_session->OnConnectFailed();

			Close();

			return;
		}

		netContext->type = NetContext::TYPE::IO;

		_isConnect = true;

		_session->OnConnect();

		if (_socket->IsValid() && !_monitor->ModifyEvent(SocketHandle(), netContext, true, false))
		{
			_session->OnEventError();

			Close();

			return;
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		auto context = reinterpret_cast<const struct epoll_event *>(sysContext);

		if (context->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
		{
			_session->OnConnectFailed();

			Close();

			return;
		}

		netContext->type = NetContext::TYPE::IO;

		_isConnect = true;

		_session->OnConnect();

		if (_socket->IsValid() && !_monitor->ModifyEvent(SocketHandle(), netContext, true, false))
		{
			_session->OnEventError();

			Close();

			return;
		}

#endif
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 构造函数
	 *
	 * @param socket 句柄
	 * @param server 服务
	 * @param monitor 监控
	 *
	 */
	TCPServerPipe::TCPServerPipe(std::shared_ptr<NetSocket> socket, ITCPServer * server, NetEventMonitor * monitor) : _server(server),
	                                                                                                                  _monitor(monitor),
	                                                                                                                  _socket(std::move(socket))
	{
		_acceptContext.type = NetContext::TYPE::ACCEPT;
		_acceptContext.socket = _socket->Handle();
		_acceptContext.completer = this;
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void TCPServerPipe::Close()
	{
		if (!_socket->IsValid())
		{
			return;
		}

		_isListen = false;

		if (!_monitor->RemoveEvent(SocketHandle(), nullptr))
		{
			_server->OnEventError();
		}

		_socket->Close();

		_server->OnDisconnect();
	}

	/**
	 *
	 * 接受连接
	 *
	 * @return 是否连接成功
	 *
	 */
	bool TCPServerPipe::Accept()
	{
		if (!_isListen)
		{
			return false;
		}

		return _socket->Accept(&_acceptContext) == 0;
	}

	/**
	 *
	 * 接收数据
	 *
	 * @return 是否接收成功
	 *
	 */
	bool TCPServerPipe::Receive()
	{
		return false;
	}

	/**
	 *
	 * 发送数据
	 *
	 * @param buffer 待发送数据缓冲区指针
	 * @param length 待发送数据缓冲区长度
	 *
	 * @return 是否发送成功
	 *
	 */
	bool TCPServerPipe::Send(const char * buffer, std::size_t length)
	{
		(void)buffer;
		(void)length;

		return false;
	}

	/**
	 *
	 * 追加数据
	 *
	 * @param buffer 待追加数据缓冲区指针
	 * @param length 待追加数据缓冲区长度
	 *
	 * @return 是否追加成功
	 *
	 */
	bool TCPServerPipe::Append(const void * buffer, std::size_t length)
	{
		(void)buffer;
		(void)length;

		return false;
	}

	/**
	 *
	 * 逻辑处理
	 *
	 * @param netContext 网络上下文
	 * @param sysContext 系统上下文
	 *
	 */
	void TCPServerPipe::Logic(NetContext * netContext, void * sysContext)
	{
		switch (netContext->type)
		{
			case NetContext::TYPE::ACCEPT:
			{
				DoAccept(netContext, sysContext);

				break;
			}

			default:
			{
				throw std::logic_error("TCPServerPipe type error");
			}
		}
	}

	/**
	 *
	 * 剩余消息个数
	 *
	 * @return 剩余消息个数
	 *
	 */
	std::size_t TCPServerPipe::RemainMessageCount()
	{
		return 0;
	}

	/**
	 *
	 * 句柄
	 *
	 * @return 句柄
	 *
	 */
	TINY_TOOLKIT_SOCKET_TYPE TCPServerPipe::SocketHandle()
	{
		return _socket->Handle();
	}

	/**
	 *
	 * 连接处理
	 *
	 * @param netContext 网络上下文
	 * @param sysContext 系统上下文
	 *
	 */
	void TCPServerPipe::DoAccept(NetContext * netContext, void * sysContext)
	{
		(void)netContext;
		(void)sysContext;

		if (!_isListen)
		{
			return;
		}

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		auto listenSocket = SocketHandle();
		auto acceptSocket = netContext->acceptSocket;

		if (setsockopt(acceptSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<const char *>(&listenSocket), sizeof(TINY_TOOLKIT_SOCKET_TYPE)) == -1)
		{
			Net::CloseSocket(acceptSocket);

			_server->OnAcceptFailed();

			return;
		}

		if (!_monitor->AppendEvent(acceptSocket, netContext, true, false))
		{
			Net::CloseSocket(acceptSocket);

			_server->OnEventError();

			if (!Accept())
			{
				_server->OnAcceptFailed();
			}

			return;
		}

		auto session = _server->OnAccept();

		if (session)
		{
			session->_cacheSize = _server->_cacheSize;
			session->_localAddress = _server->_localAddress;

			auto pipe = std::make_shared<TCPSessionPipe>(std::make_shared<NetSocket>(acceptSocket), session, _monitor);

			pipe->_isConnect = true;

			session->_pipe = pipe;

			session->OnConnect();

			if (!pipe->Receive())
			{
				session->OnReceiveFailed();

				session->Close();

				return;
			}
		}
		else
		{
			Net::CloseSocket(acceptSocket);
		}

		if (!Accept())
		{
			_server->OnAcceptFailed();

			return;
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

		auto context = reinterpret_cast<const struct kevent *>(sysContext);

		if (context->flags & EV_ERROR)
		{
			_server->OnAcceptFailed();

			Close();

			return;
		}

		if (!Accept())
		{
			_server->OnAcceptFailed();

			return;
		}

		auto acceptSocket = netContext->acceptSocket;

		if (!Net::SetNoDelay(acceptSocket, true) ||
		    !Net::SetNoBlock(acceptSocket, true) ||
		    !Net::SetReuseAddress(acceptSocket, true))
		{
			Net::CloseSocket(acceptSocket);

			_server->OnAcceptFailed();

			return;
		}

		auto session = _server->OnAccept();

		if (session)
		{
			session->_cacheSize = _server->_cacheSize;
			session->_localAddress = _server->_localAddress;

			auto pipe = std::make_shared<TCPSessionPipe>(std::make_shared<NetSocket>(acceptSocket), session, _monitor);

			pipe->_isConnect = true;

			session->_pipe = pipe;

			session->OnConnect();

			if (!_monitor->AppendEvent(acceptSocket, &pipe->_ioContext, true, false))
			{
				session->OnEventError();

				session->Close();

				return;
			}
		}
		else
		{
			Net::CloseSocket(acceptSocket);
		}

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		auto context = reinterpret_cast<const struct epoll_event *>(sysContext);

		if (context->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
		{
			_server->OnAcceptFailed();

			Close();

			return;
		}

		if (!Accept())
		{
			_server->OnAcceptFailed();

			return;
		}

		auto acceptSocket = netContext->acceptSocket;

		if (!Net::SetNoDelay(acceptSocket, true) ||
		    !Net::SetNoBlock(acceptSocket, true) ||
		    !Net::SetReuseAddress(acceptSocket, true))
		{
			Net::CloseSocket(acceptSocket);

			_server->OnAcceptFailed();

			return;
		}

		auto session = _server->OnAccept();

		if (session)
		{
			session->_cacheSize = _server->_cacheSize;
			session->_localAddress = _server->_localAddress;

			auto pipe = std::make_shared<TCPSessionPipe>(std::make_shared<NetSocket>(acceptSocket), session, _monitor);

			pipe->_isConnect = true;

			session->_pipe = pipe;

			session->OnConnect();

			if (!_monitor->AppendEvent(acceptSocket, &pipe->_ioContext, true, false))
			{
				session->OnEventError();

				session->Close();

				return;
			}
		}
		else
		{
			Net::CloseSocket(acceptSocket);
		}

#endif
	}
}
