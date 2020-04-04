/**
 *
 *  作者: hm
 *
 *  说明: udp
 *
 */


#include "udp.h"
#include "server.h"

#include "../ip/socket.h"
#include "../ip/address.h"


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
		 * @param poller 轮询器
		 * @param adaptor 适配器
		 *
		 */
		UDPSessionChannel::UDPSessionChannel(UDPSession * session, std::shared_ptr<UDPAdaptor> adaptor) : _cache(session->_cacheSize),
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
		 * 析构函数
		 *
		 */
		UDPSessionChannel::~UDPSessionChannel()
		{
			_adaptor->Close();
		}

		/**
		 *
		 * 关闭会话
		 *
		 */
		void UDPSessionChannel::Close()
		{
			if (!_adaptor->IsValid())
			{
				return;
			}

			_session->Pollers()->RemoveEvent(_adaptor->Socket(), nullptr);

			if (_isConnect)
			{
				_isConnect = false;

				if (_session->_onDisconnect)
				{
					_session->_onDisconnect();
				}
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
		void UDPSessionChannel::Process(Context * netContext, void * sysContext)
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
					throw std::logic_error("UDPSessionChannel type error");
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
		bool UDPSessionChannel::Receive()
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

			auto len = _adaptor->Receive(_cache.Buffer(), _cache.OverLength(), nullptr);

			if (len > 0)
			{
				_ioContext.bytes = static_cast<std::size_t>(len);
			}
			else
			{
				_ioContext.bytes = 0;

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
		bool UDPSessionChannel::Send(const void * buffer, std::size_t length)
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

			auto len = _adaptor->Send(const_cast<void *>(buffer), length, nullptr);

			if (len > 0)
			{
				_ioContext.bytes = static_cast<std::size_t>(len);
			}
			else
			{
				_ioContext.bytes = 0;

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
		bool UDPSessionChannel::Append(const void * buffer, std::size_t length)
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
					if (_session->_onSend)
					{
						_session->_onSend(false);
					}

					Close();

					return false;
				}

			#else

				if (_isConnect &&
				    _adaptor->IsValid() &&
				    !_session->Pollers()->ModifyEvent(_adaptor->Socket(), &_ioContext, true, true))
				{
					if (_session->_onError)
					{
						_session->_onError();
					}

					Close();

					return false;
				}

			#endif
			}

			return true;
		}

		/**
		 *
		 * 缓存大小
		 *
		 * @return 缓存大小
		 *
		 */
		std::size_t UDPSessionChannel::CacheSize() const
		{
			return _session->_cacheSize;
		}

		/**
		 *
		 * 剩余消息个数
		 *
		 * @return 剩余消息个数
		 *
		 */
		std::size_t UDPSessionChannel::RemainMessageCount() const
		{
			return _messageQueue.size();
		}

		/**
		 *
		 * 套接字
		 *
		 * @return 套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE UDPSessionChannel::Socket() const
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
		void UDPSessionChannel::DoIO(Context * netContext, void * sysContext)
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
				if (_session->_onError)
				{
					_session->_onError();
				}

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
				if (_session->_onError)
				{
					_session->_onError();
				}

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
		void UDPSessionChannel::DoSend(Context * netContext, void * sysContext)
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
				if (_session->_onSend)
				{
					_session->_onSend(false);
				}

				Close();

				return;
			}

			_messageQueue.pop();

			if (_session->_onSend)
			{
				_session->_onSend(true);
			}

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
				if (_session->_onSend)
				{
					_session->_onSend(false);
				}

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
					if (_session->_onSend)
					{
						_session->_onSend(false);
					}

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

					if (_session->_onSend)
					{
						_session->_onSend(true);
					}

					break;
				}
			}

			std::lock_guard<std::mutex> lock(_mutex);

			if (_messageQueue.empty())
			{
				_isSend = false;

				if (_isConnect &&
				    _adaptor->IsValid() &&
				    !_session->Pollers()->ModifyEvent(_adaptor->Socket(), &_ioContext, true, false))
				{
					if (_session->_onError)
					{
						_session->_onError();
					}

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
		void UDPSessionChannel::DoReceive(Context * netContext, void * sysContext)
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
				if (_session->_onReceive)
				{
					_session->_onReceive(false, nullptr, 0);
				}

				Close();

				return;
			}

			_cache.DisplacementWrite(_receiveContext.bytes);

			if (_session->_onReceive)
			{
				_cache.DisplacementRead
				(
					_session->_onReceive(true, _cache.Buffer(), _cache.ContentLength())
				);
			}

			if (!Receive())
			{
				if (_session->_onReceive)
				{
					_session->_onReceive(false, nullptr, 0);
				}

				Close();

				return;
			}

		#else

			int32_t max = 0;

			do
			{
				if (!Receive())
				{
					if (_session->_onReceive)
					{
						_session->_onReceive(false, nullptr, 0);
					}

					Close();

					return;
				}

				if (_ioContext.bytes == 0)
				{
					break;
				}

				_cache.DisplacementWrite(_ioContext.bytes);

				if (_session->_onReceive)
				{
					_cache.DisplacementRead
					(
						_session->_onReceive(true, _cache.Buffer(), _cache.ContentLength())
					);
				}
			}
			while (++max <= 3 && _ioContext.bytes);

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
		void UDPSessionChannel::DoConnect(Context * netContext, void * sysContext)
		{
			(void)netContext;
			(void)sysContext;

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (setsockopt(_adaptor->Socket(), SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, nullptr, 0) == -1)
			{
				if (_session->_onConnect)
				{
					_session->_onConnect(false);
				}

				Close();

				return;
			}

			_receiveContext.optionType = NET_OPTION_TYPE::RECEIVE;

			_isConnect = true;

			if (_session->_onConnect)
			{
				_session->_onConnect(true);
			}

			if (!Receive())
			{
				if (_session->_onReceive)
				{
					_session->_onReceive(false, nullptr, 0);
				}

				Close();

				return;
			}

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			auto context = reinterpret_cast<const struct kevent *>(sysContext);

			if (context->flags & EV_ERROR)
			{
				if (_session->_onConnect)
				{
					_session->_onConnect(false);
				}

				Close();

				return;
			}

			_ioContext.optionType = NET_OPTION_TYPE::IO;

			_isConnect = true;

			if (_isConnect &&
			    _adaptor->IsValid() &&
			    !_session->Pollers()->ModifyEvent(_adaptor->Socket(), &_ioContext, true, false))
			{
				if (_session->_onError)
				{
					_session->_onError();
				}

				Close();

				return;
			}

			if (_session->_onConnect)
			{
				_session->_onConnect(true);
			}

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			auto context = reinterpret_cast<const struct epoll_event *>(sysContext);

			if (context->events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
			{
				if (_session->_onConnect)
				{
					_session->_onConnect(false);
				}

				Close();

				return;
			}

			_ioContext.optionType = NET_OPTION_TYPE::IO;

			_isConnect = true;

			if (_isConnect &&
			    _adaptor->IsValid() &&
			    !_session->Pollers()->ModifyEvent(_adaptor->Socket(), &_ioContext, true, false))
			{
				if (_session->_onError)
				{
					_session->_onError();
				}

				Close();

				return;
			}

			if (_session->_onConnect)
			{
				_session->_onConnect(true);
			}

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
		UDPServerChannel::UDPServerChannel(UDPServer * server, std::shared_ptr<UDPAdaptor> adaptor) : _server(server),
		                                                                                              _adaptor(std::move(adaptor))
		{
			_acceptContext.completer = this;
			_acceptContext.optionType = NET_OPTION_TYPE::ACCEPT;
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		UDPServerChannel::~UDPServerChannel()
		{
			_adaptor->Close();
		}

		/**
		 *
		 * 关闭会话
		 *
		 */
		void UDPServerChannel::Close()
		{
			if (!_adaptor->IsValid())
			{
				return;
			}

			_server->Pollers()->RemoveEvent(_adaptor->Socket(), nullptr);

			if (_isListen)
			{
				_isListen = false;

				if (_server->_onShutdown)
				{
					_server->_onShutdown();
				}
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
		void UDPServerChannel::Process(Context * netContext, void * sysContext)
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
					throw std::logic_error("UDPServerChannel type error");
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
		bool UDPServerChannel::Accept()
		{
			if (!_isListen)
			{
				return false;
			}

			return _adaptor->Accept(&_acceptContext) != TINY_TOOLKIT_SOCKET_ERROR;
		}

		/**
		 *
		 * 缓存大小
		 *
		 * @return 缓存大小
		 *
		 */
		std::size_t UDPServerChannel::CacheSize() const
		{
			return _server->_cacheSize;
		}

		/**
		 *
		 * 套接字
		 *
		 * @return 套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE UDPServerChannel::Socket() const
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
		void UDPServerChannel::DoAccept(Context * netContext, void * sysContext)
		{
			(void)netContext;
			(void)sysContext;

			if (!_isListen)
			{
				return;
			}

			if (!_server->_onAccept)
			{
				return;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			_server->_peerEndpoint.port = htons(_acceptContext.address.v4.sin_port);
			_server->_peerEndpoint.host = ip::Address::AsString(_acceptContext.address.v4.sin_addr);

			auto adaptor = std::make_shared<UDPAdaptor>(_acceptContext.socket);

			adaptor->SetBlock(false);
			adaptor->SetReuseAddress(true);

			auto session = _server->_onAccept(true);

			if (session)
			{
				[&]()
				{
					session->_cacheSize = _server->_cacheSize;
					session->_peerEndpoint = _server->_peerEndpoint;
					session->_localEndpoint = _server->_localEndpoint;

					BOOL bNewBehavior = FALSE;

					DWORD dwBytesReturned = 0;

					if (::WSAIoctl(adaptor->Socket(), SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), nullptr, 0, &dwBytesReturned, nullptr, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						if (session->_onError)
						{
							session->_onError();
						}

						adaptor->Close();

						return;
					}

					if (adaptor->BindV4(session->_localEndpoint) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						if (session->_onBind)
						{
							session->_onBind(false);
						}

						adaptor->Close();

						return;
					}

					if (session->_onBind)
					{
						session->_onBind(true);
					}

					if (adaptor->ConnectV4(session->_peerEndpoint, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						if (session->_onConnect)
						{
							session->_onConnect(false);
						}

						adaptor->Close();

						return;
					}

					auto channel = std::make_shared<UDPSessionChannel>(session, adaptor);

					if (_isListen &&
					    !session->Pollers()->AppendEvent(adaptor->Socket(), &channel->_receiveContext, true, false))
					{
						if (session->_onError)
						{
							session->_onError();
						}

						session->Close();

						return;
					}

					channel->_isConnect = true;

					session->_channel = channel;

					if (session->_onConnect)
					{
						session->_onConnect(true);
					}

					if (_acceptContext.bytes > channel->_cache.OverLength())
					{
						_acceptContext.bytes = channel->_cache.OverLength();
					}

					memcpy(channel->_cache.Buffer(), _acceptContext.temp, _acceptContext.bytes);

					channel->_cache.DisplacementWrite(_acceptContext.bytes);

					if (session->_onReceive)
					{
						channel->_cache.DisplacementRead
						(
							session->_onReceive(true, channel->_cache.Buffer(), channel->_cache.ContentLength())
						);
					}

					if (!channel->Receive())
					{
						if (session->_onReceive)
						{
							session->_onReceive(false, nullptr, 0);
						}

						session->Close();

						return;
					}
				}();
			}
			else
			{
				adaptor->Close();
			}

			if (!Accept())
			{
				_server->_onAccept(false);

				return;
			}

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			auto context = reinterpret_cast<const struct kevent *>(sysContext);

			if (context->flags & EV_ERROR)
			{
				if (_server->_onError)
				{
					_server->_onError();
				}

				Close();

				return;
			}

			if (!Accept())
			{
				_server->_onAccept(false);

				return;
			}

			_server->_peerEndpoint.port = htons(_acceptContext.address.v4.sin_port);
			_server->_peerEndpoint.host = ip::Address::AsString(_acceptContext.address.v4.sin_addr);

			auto adaptor = std::make_shared<UDPAdaptor>(_acceptContext.socket);

			adaptor->SetBlock(false);
			adaptor->SetReusePort(true);
			adaptor->SetReuseAddress(true);

			auto session = _server->_onAccept(true);

			if (session)
			{
				[&]()
				{
					session->_cacheSize = _server->_cacheSize;
					session->_peerEndpoint = _server->_peerEndpoint;
					session->_localEndpoint = _server->_localEndpoint;

					if (adaptor->BindV4(session->_localEndpoint) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						if (session->_onBind)
						{
							session->_onBind(false);
						}

						adaptor->Close();

						return;
					}

					if (session->_onBind)
					{
						session->_onBind(true);
					}

					if (adaptor->ConnectV4(session->_peerEndpoint, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						if (session->_onConnect)
						{
							session->_onConnect(false);
						}

						adaptor->Close();

						return;
					}

					auto channel = std::make_shared<UDPSessionChannel>(session, adaptor);

					if (_isListen &&
					    !session->Pollers()->AppendEvent(adaptor->Socket(), &channel->_ioContext, true, false))
					{
						if (session->_onError)
						{
							session->_onError();
						}

						session->Close();

						return;
					}

					channel->_isConnect = true;

					session->_channel = channel;

					if (session->_onConnect)
					{
						session->_onConnect(true);
					}

					if (_acceptContext.bytes > channel->_cache.OverLength())
					{
						_acceptContext.bytes = channel->_cache.OverLength();
					}

					memcpy(channel->_cache.Buffer(), _acceptContext.temp, _acceptContext.bytes);

					channel->_cache.DisplacementWrite(_acceptContext.bytes);

					if (session->_onReceive)
					{
						channel->_cache.DisplacementRead
						(
							session->_onReceive(true, channel->_cache.Buffer(), channel->_cache.ContentLength())
						);
					}
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
				if (_server->_onError)
				{
					_server->_onError();
				}

				Close();

				return;
			}

			if (!Accept())
			{
				_server->_onAccept(false);

				return;
			}

			_server->_peerEndpoint.port = htons(_acceptContext.address.v4.sin_port);
			_server->_peerEndpoint.host = ip::Address::AsString(_acceptContext.address.v4.sin_addr);

			auto adaptor = std::make_shared<UDPAdaptor>(_acceptContext.socket);

			adaptor->SetBlock(false);
			adaptor->SetReusePort(true);
			adaptor->SetReuseAddress(true);

			auto session = _server->_onAccept(true);

			if (session)
			{
				[&]()
				{
					session->_cacheSize = _server->_cacheSize;
					session->_peerEndpoint = _server->_peerEndpoint;
					session->_localEndpoint = _server->_localEndpoint;

					if (adaptor->BindV4(session->_localEndpoint) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						if (session->_onBind)
						{
							session->_onBind(false);
						}

						adaptor->Close();

						return;
					}

					if (session->_onBind)
					{
						session->_onBind(true);
					}

					if (adaptor->ConnectV4(session->_peerEndpoint, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						if (session->_onConnect)
						{
							session->_onConnect(false);
						}

						adaptor->Close();

						return;
					}

					auto channel = std::make_shared<UDPSessionChannel>(session, adaptor);

					if (_isListen &&
					    !session->Pollers()->AppendEvent(adaptor->Socket(), &channel->_ioContext, true, false))
					{
						if (session->_onError)
						{
							session->_onError();
						}

						session->Close();

						return;
					}

					channel->_isConnect = true;

					session->_channel = channel;

					if (session->_onConnect)
					{
						session->_onConnect(true);
					}

					if (_acceptContext.bytes > channel->_cache.OverLength())
					{
						_acceptContext.bytes = channel->_cache.OverLength();
					}

					memcpy(channel->_cache.Buffer(), _acceptContext.temp, _acceptContext.bytes);

					channel->_cache.DisplacementWrite(_acceptContext.bytes);

					if (session->_onReceive)
					{
						channel->_cache.DisplacementRead
						(
							session->_onReceive(true, channel->_cache.Buffer(), channel->_cache.ContentLength())
						);
					}
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
