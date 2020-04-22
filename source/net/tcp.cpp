/**
 *
 *  作者: hm
 *
 *  说明: tcp
 *
 */


#include "tcp.h"
#include "server.h"

#include "../ip/socket.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <MSWSock.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <sys/event.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <sys/epoll.h>
#
#endif


namespace tinyToolkit
{
	namespace net
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param session 会话
		 * @param adaptor 适配器
		 *
		 */
		TCPSessionChannel::TCPSessionChannel(ITCPSession * session, std::shared_ptr<TCPAdaptor> adaptor) : _cache(session->_receiveCacheSize),
		                                                                                                   _session(session),
		                                                                                                   _adaptor(std::move(adaptor))
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			_sendContext.completer = this;
			_sendContext.optionType = NET_OPTION_TYPE::SEND;

			_receiveContext.completer = this;
			_receiveContext.optionType = NET_OPTION_TYPE::RECEIVE;

		#else

			_ioContext.completer = this;
			_ioContext.optionType = NET_OPTION_TYPE::IO;

		#endif
		}

		/**
		 *
		 * 关闭会话
		 *
		 */
		void TCPSessionChannel::Close()
		{
			if (!_adaptor->IsValid())
			{
				return;
			}

			_session->Pollers()->RemoveEvent(_adaptor->Socket(), nullptr);

			if (_isConnect)
			{
				_isConnect = false;

				_adaptor->Close();

				_session->OnDisconnect();
			}
		}

		/**
		 *
		 * 处理流程
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void TCPSessionChannel::Process(Context * netContext, void * sysContext)
		{
			switch (netContext->optionType)
			{
				case NET_OPTION_TYPE::IO:
				{
					DoIO(netContext, sysContext);

					break;
				}

				case NET_OPTION_TYPE::SEND:
				{
					DoSend(netContext, sysContext);

					break;
				}

				case NET_OPTION_TYPE::CONNECT:
				{
					DoConnect(netContext, sysContext);

					break;
				}

				case NET_OPTION_TYPE::RECEIVE:
				{
					DoReceive(netContext, sysContext);

					break;
				}

				default:
				{
					throw std::logic_error("TCPSessionChannel type error");
				}
			}
		}

		/**
		 *
		 * 接收数据
		 *
		 * @return 是否接收成功
		 *
		 */
		bool TCPSessionChannel::Receive()
		{
			if (!_isConnect)
			{
				return false;
			}

			auto freeLen = static_cast<int32_t>(_cache.FreeLength());
			auto readable = ip::Socket::ReadableSize(_adaptor->Socket());

			if (readable > freeLen)
			{
				readable = freeLen;
			}

			_cache.Organize(readable);

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (_adaptor->Receive(_cache.Buffer(), _cache.OverLength(), &_receiveContext) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				return false;
			}

		#else

			_ioContext.bytes = _adaptor->Receive(_cache.Buffer(), _cache.OverLength(), nullptr);

			if (_ioContext.bytes == 0)
			{
				return false;
			}
			else if (_ioContext.bytes < 0)
			{
				if (errno != EINTR && errno != EAGAIN)
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
		 * @param buffer 内容
		 * @param length 长度
		 *
		 * @return 是否发送成功
		 *
		 */
		bool TCPSessionChannel::Send(const void * buffer, std::size_t length)
		{
			if (!_isConnect || buffer == nullptr || length == 0)
			{
				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (_adaptor->Send(const_cast<void *>(buffer), length, &_sendContext) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				return false;
			}

		#else

			_ioContext.bytes = _adaptor->Send(const_cast<void *>(buffer), length, nullptr);

			if (_ioContext.bytes == 0)
			{
				return false;
			}
			else if (_ioContext.bytes < 0)
			{
				if (errno != EINTR && errno != EAGAIN)
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
		bool TCPSessionChannel::Append(const void * buffer, std::size_t length)
		{
			if (!_isConnect || buffer == nullptr || length == 0)
			{
				return false;
			}

			std::lock_guard<std::mutex> lock(_mutex);

			_messageQueue.push(std::make_shared<Message>(buffer, length));

			if (!_isSend)
			{
				_isSend = true;

			#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

				auto & message = _messageQueue.front();

				if (!Send(message->buffer, message->length))
				{
					_session->OnError();

					Close();

					return false;
				}

			#else

				if (_adaptor->IsValid() &&
				    !_session->Pollers()->ModifyEvent(_adaptor->Socket(), &_ioContext, true, true))
				{
					_session->OnError();

					Close();

					return false;
				}

			#endif
			}

			return true;
		}

		/**
		 *
		 * 套接字
		 *
		 * @return 套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE TCPSessionChannel::Socket() const
		{
			return _adaptor->Socket();
		}

		/**
		 *
		 * 交互处理
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void TCPSessionChannel::DoIO(Context * netContext, void * sysContext)
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
				_session->OnError();

				Close();

				return;
			}

			if (context->filter & EVFILT_READ)
			{
				DoReceive(netContext, sysContext);
			}

			if (context->filter & EVFILT_WRITE)
			{
				DoSend(netContext, sysContext);
			}

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			auto context = reinterpret_cast<const struct epoll_event *>(sysContext);

			if (context->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
			{
				_session->OnError();

				Close();

				return;
			}

			if (context->events & EPOLLIN)
			{
				DoReceive(netContext, sysContext);
			}

			if (context->events & EPOLLOUT)
			{
				DoSend(netContext, sysContext);
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
		void TCPSessionChannel::DoSend(Context * netContext, void * sysContext)
		{
			(void)netContext;
			(void)sysContext;

			if (!_isConnect)
			{
				return;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (_sendContext.bytes == 0)
			{
				_session->OnError();

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

			if (_isConnect && !Send(message->buffer, message->length))
			{
				_session->OnError();

				Close();

				return;
			}

		#else

			{
				std::lock_guard<std::mutex> lock(_mutex);

				if (_messageQueue.empty())
				{
					return;
				}
			}

			int32_t max = 0;

			auto & message = _messageQueue.front();

			while (++max <= 3 && message->offset < message->length)
			{
				if (!Send(message->buffer + message->offset, message->length - message->offset))
				{
					if (_ioContext.bytes < 0)
					{
						_session->OnError();
					}

					Close();

					return;
				}

				if (_ioContext.bytes < 0)
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
			}

			std::lock_guard<std::mutex> lock(_mutex);

			if (_messageQueue.empty())
			{
				_isSend = false;

				if (_adaptor->IsValid() &&
				    !_session->Pollers()->ModifyEvent(_adaptor->Socket(), &_ioContext, true, false))
				{
					_session->OnError();

					Close();

					return;
				}
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
		void TCPSessionChannel::DoReceive(Context * netContext, void * sysContext)
		{
			(void)netContext;
			(void)sysContext;

			if (!_isConnect)
			{
				return;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (_receiveContext.bytes == 0)
			{
				_session->OnError();

				Close();

				return;
			}

			_cache.DisplacementWrite(_receiveContext.bytes);

			_cache.DisplacementRead
			(
				_session->OnReceive(_cache.Buffer(), _cache.ContentLength())
			);

			if (_isConnect && !Receive())
			{
				_session->OnError();

				Close();

				return;
			}

		#else

			int32_t max = 0;

			do
			{
				if (!Receive())
				{
					if (_ioContext.bytes < 0)
					{
						_session->OnError();
					}

					Close();

					return;
				}

				if (_ioContext.bytes < 0)
				{
					break;
				}

				_cache.DisplacementWrite(_ioContext.bytes);

				_cache.DisplacementRead
				(
					_session->OnReceive(_cache.Buffer(), _cache.ContentLength())
				);
			}
			while (++max <= 3 && _ioContext.bytes > 0);

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
		void TCPSessionChannel::DoConnect(Context * netContext, void * sysContext)
		{
			(void)netContext;
			(void)sysContext;

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (::setsockopt(_adaptor->Socket(), SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, nullptr, 0) == -1)
			{
				_session->OnError();

				Close();

				return;
			}

			_receiveContext.optionType = NET_OPTION_TYPE::RECEIVE;

			_isConnect = true;

			_session->OnConnect();

			if (!Receive())
			{
				_session->OnError();

				Close();

				return;
			}

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			auto context = reinterpret_cast<const struct kevent *>(sysContext);

			if (context->flags & EV_ERROR)
			{
				_session->OnError();

				Close();

				return;
			}

			_ioContext.optionType = NET_OPTION_TYPE::IO;

			_isConnect = true;

			if (_adaptor->IsValid() &&
			    !_session->Pollers()->ModifyEvent(_adaptor->Socket(), &_ioContext, true, false))
			{
				_session->OnError();

				Close();

				return;
			}

			_session->OnConnect();

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			auto context = reinterpret_cast<const struct epoll_event *>(sysContext);

			if (context->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
			{
				_session->OnError();

				Close();

				return;
			}

			_ioContext.optionType = NET_OPTION_TYPE::IO;

			_isConnect = true;

			if (_adaptor->IsValid() &&
			    !_session->Pollers()->ModifyEvent(_adaptor->Socket(), &_ioContext, true, false))
			{
				_session->OnError();

				Close();

				return;
			}

			_session->OnConnect();

		#endif
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 构造函数
		 *
		 * @param server 服务
		 * @param adaptor 适配器
		 *
		 */
		TCPServerChannel::TCPServerChannel(ITCPServer * server, std::shared_ptr<TCPAdaptor> adaptor) : _server(server),
		                                                                                               _adaptor(std::move(adaptor))
		{
			_acceptContext.completer = this;
			_acceptContext.optionType = NET_OPTION_TYPE::ACCEPT;
		}

		/**
		 *
		 * 关闭会话
		 *
		 */
		void TCPServerChannel::Close()
		{
			if (!_adaptor->IsValid())
			{
				return;
			}

			_server->Pollers()->RemoveEvent(_adaptor->Socket(), nullptr);

			if (_isListen)
			{
				_isListen = false;

				_adaptor->Close();

				_server->OnShutdown();
			}
		}

		/**
		 *
		 * 处理流程
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void TCPServerChannel::Process(Context * netContext, void * sysContext)
		{
			switch (netContext->optionType)
			{
				case NET_OPTION_TYPE::ACCEPT:
				{
					DoAccept(netContext, sysContext);

					break;
				}

				default:
				{
					throw std::logic_error("TCPServerChannel type error");
				}
			}
		}

		/**
		 *
		 * 接受连接
		 *
		 * @return 是否连接成功
		 *
		 */
		bool TCPServerChannel::Accept()
		{
			if (!_isListen)
			{
				return false;
			}

			return _adaptor->Accept(&_acceptContext) != TINY_TOOLKIT_SOCKET_ERROR;
		}

		/**
		 *
		 * 套接字
		 *
		 * @return 套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE TCPServerChannel::Socket() const
		{
			return _adaptor->Socket();
		}

		/**
		 *
		 * 连接处理
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		void TCPServerChannel::DoAccept(Context * netContext, void * sysContext)
		{
			(void)netContext;
			(void)sysContext;

			if (!_isListen)
			{
				return;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			auto listenSocket = _adaptor->Socket();
			auto acceptSocket = _acceptContext.socket;

			if (::setsockopt(acceptSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<const char *>(&listenSocket), sizeof(listenSocket)) == -1)
			{
				ip::Socket::Close(acceptSocket);

				_server->OnError();

				return;
			}

			auto adaptor = std::make_shared<TCPAdaptor>(acceptSocket);

			adaptor->SetBlock(false);
			adaptor->SetDelay(false);
			adaptor->SetReuseAddress(true);

			auto session = _server->OnAccept();

			if (session)
			{
				[&]()
				{
					session->_peerEndpoint = _server->_peerEndpoint;
					session->_localEndpoint = _server->_localEndpoint;

					auto channel = std::make_shared<TCPSessionChannel>(session, adaptor);

					if (adaptor->IsValid() &&
						!session->Pollers()->AppendEvent(adaptor->Socket(), &channel->_receiveContext, true, false))
					{
						session->OnError();

						session->Close();

						return;
					}

					channel->_isConnect = true;

					session->_channel = channel;

					session->OnConnect();

					if (!channel->Receive())
					{
						session->OnError();

						session->Close();

						return;
					}
				}();
			}
			else
			{
				adaptor->Close();
			}

			if (_isListen && !Accept())
			{
				_server->OnError();

				return;
			}

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			auto context = reinterpret_cast<const struct kevent *>(sysContext);

			if (context->flags & EV_ERROR)
			{
				_server->OnError();

				Close();

				return;
			}

			if (!Accept())
			{
				_server->OnError();

				return;
			}

			auto adaptor = std::make_shared<TCPAdaptor>(_acceptContext.socket);

			adaptor->SetBlock(false);
			adaptor->SetDelay(false);
			adaptor->SetReusePort(true);
			adaptor->SetReuseAddress(true);

			auto session = _server->OnAccept();

			if (session)
			{
				[&]()
				{
					session->_peerEndpoint = _server->_peerEndpoint;
					session->_localEndpoint = _server->_localEndpoint;

					auto channel = std::make_shared<TCPSessionChannel>(session, adaptor);

					if (adaptor->IsValid() &&
						!session->Pollers()->AppendEvent(adaptor->Socket(), &channel->_ioContext, true, false))
					{
						session->OnError();

						session->Close();

						return;
					}

					channel->_isConnect = true;

					session->_channel = channel;

					session->OnConnect();
				}();
			}
			else
			{
				adaptor->Close();
			}

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			auto context = reinterpret_cast<const struct epoll_event *>(sysContext);

			if (context->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
			{
				_server->OnError();

				Close();

				return;
			}

			if (!Accept())
			{
				_server->OnError();

				return;
			}

			auto adaptor = std::make_shared<TCPAdaptor>(_acceptContext.socket);

			adaptor->SetBlock(false);
			adaptor->SetDelay(false);
			adaptor->SetReusePort(true);
			adaptor->SetReuseAddress(true);

			auto session = _server->OnAccept();

			if (session)
			{
				[&]()
				{
					session->_peerEndpoint = _server->_peerEndpoint;
					session->_localEndpoint = _server->_localEndpoint;

					auto channel = std::make_shared<TCPSessionChannel>(session, adaptor);

					if (adaptor->IsValid() &&
						!session->Pollers()->AppendEvent(adaptor->Socket(), &channel->_ioContext, true, false))
					{
						session->OnError();

						session->Close();

						return;
					}

					channel->_isConnect = true;

					session->_channel = channel;

					session->OnConnect();
				}();
			}
			else
			{
				adaptor->Close();
			}

		#endif
		}
	}
}
