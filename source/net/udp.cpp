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
		UDPSessionPipe::UDPSessionPipe(IUDPSession * session, Poller * poller, std::shared_ptr<UDPAdaptor> adaptor) : _cache(session->_cacheSize),
		                                                                                                              _poller(poller),
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
		UDPSessionPipe::~UDPSessionPipe()
		{
			_adaptor->Close();
		}

		/**
		 *
		 * 关闭会话
		 *
		 */
		void UDPSessionPipe::Close()
		{
			if (!_adaptor->IsValid())
			{
				return;
			}

			_poller->RemoveEvent(_adaptor->Socket(), nullptr);

			if (_isConnect)
			{
				_isConnect = false;

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
		void UDPSessionPipe::Process(Context * netContext, void * sysContext)
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
					throw std::logic_error("UDPSessionPipe type error");
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
		bool UDPSessionPipe::Receive()
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
		bool UDPSessionPipe::Send(const void * buffer, std::size_t length)
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
		bool UDPSessionPipe::Append(const void * buffer, std::size_t length)
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
					_session->OnSendFailed();

					Close();

					return false;
				}

			#else

				if (_isConnect &&
					_adaptor->IsValid() &&
				    !_poller->ModifyEvent(_adaptor->Socket(), &_ioContext, true, true))
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
		 * 缓存大小
		 *
		 * @return 缓存大小
		 *
		 */
		std::size_t UDPSessionPipe::CacheSize() const
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
		std::size_t UDPSessionPipe::RemainMessageCount() const
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
		TINY_TOOLKIT_SOCKET_TYPE UDPSessionPipe::Socket() const
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
		void UDPSessionPipe::DoIO(Context * netContext, void * sysContext)
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
				int32_t max = 0;

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

					_cache.DisplacementWrite(_ioContext.bytes);
					_cache.DisplacementRead(_session->OnReceive(_cache.Buffer(), _cache.ContentLength()));
				}
				while (++max <= 3 && _ioContext.bytes);
			}

			if (context->filter & EVFILT_WRITE)
			{
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
				}

				std::lock_guard<std::mutex> lock(_mutex);

				if (_messageQueue.empty())
				{
					_isSend = false;

					if (_isConnect &&
						_adaptor->IsValid() &&
						!_poller->ModifyEvent(_adaptor->Socket(), &_ioContext, true, false))
					{
						_session->OnError();

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
				int32_t max = 0;

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

					_cache.DisplacementWrite(_ioContext.bytes);
					_cache.DisplacementRead(_session->OnReceive(_cache.Buffer(), _cache.ContentLength()));
				}
				while (++max <= 3 && _ioContext.bytes);
			}

			if (context->events & EPOLLOUT)
			{
				int32_t max = 0;

				auto & message = _messageQueue.front();

				while (++max <= 3 && message->offset < message->length)
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
				}

				std::lock_guard<std::mutex> lock(_mutex);

				if (_messageQueue.empty())
				{
					_isSend = false;

					if (_isConnect &&
					    _adaptor->IsValid() &&
					    !_poller->ModifyEvent(_adaptor->Socket(), &_ioContext, true, false))
					{
						_session->OnError();

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
		void UDPSessionPipe::DoSend(Context * netContext, void * sysContext)
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
		void UDPSessionPipe::DoReceive(Context * netContext, void * sysContext)
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
				_session->OnReceiveFailed();

				Close();

				return;
			}

			_cache.DisplacementWrite(_receiveContext.bytes);
			_cache.DisplacementRead(_session->OnReceive(_cache.Buffer(), _cache.ContentLength()));

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
		void UDPSessionPipe::DoConnect(Context * netContext, void * sysContext)
		{
			(void)netContext;
			(void)sysContext;

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (setsockopt(_adaptor->Socket(), SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, nullptr, 0) == -1)
			{
				_session->OnConnectFailed();

				Close();

				return;
			}

			_sendContext.optionType = NET_OPTION_TYPE::SEND;

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

			_ioContext.optionType = NET_OPTION_TYPE::IO;

			_isConnect = true;

			if (_adaptor->IsValid() &&
				!_poller->ModifyEvent(_adaptor->Socket(), &_ioContext, true, false))
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
				_session->OnConnectFailed();

				Close();

				return;
			}

			_ioContext.optionType = NET_OPTION_TYPE::IO;

			_isConnect = true;

			if (_isConnect &&
			    _adaptor->IsValid() &&
			    !_poller->ModifyEvent(_adaptor->Socket(), &_ioContext, true, false))
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
		 * @param poller 轮询器
		 * @param adaptor 适配器
		 *
		 */
		UDPServerPipe::UDPServerPipe(IUDPServer * server, Poller * poller, std::shared_ptr<UDPAdaptor> adaptor) : _poller(poller),
		                                                                                                          _server(server),
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
		UDPServerPipe::~UDPServerPipe()
		{
			_adaptor->Close();
		}

		/**
		 *
		 * 关闭会话
		 *
		 */
		void UDPServerPipe::Close()
		{
			if (!_adaptor->IsValid())
			{
				return;
			}

			_poller->RemoveEvent(_adaptor->Socket(), nullptr);

			if (_isListen)
			{
				_isListen = false;

				_server->OnDisconnect();
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
		void UDPServerPipe::Process(Context * netContext, void * sysContext)
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
					throw std::logic_error("UDPServerPipe type error");
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
		bool UDPServerPipe::Accept()
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
		std::size_t UDPServerPipe::CacheSize() const
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
		TINY_TOOLKIT_SOCKET_TYPE UDPServerPipe::Socket() const
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
		void UDPServerPipe::DoAccept(Context * netContext, void * sysContext)
		{
			(void)netContext;
			(void)sysContext;

			if (!_isListen)
			{
				return;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			_server->_peerEndpoint.port = htons(_acceptContext.address.v4.sin_port);
			_server->_peerEndpoint.host = ip::Address::AsString(_acceptContext.address.v4.sin_addr);

			auto adaptor = std::make_shared<UDPAdaptor>(_acceptContext.socket);

			adaptor->SetBlock(false);
			adaptor->SetReuseAddress(true);

			auto session = _server->OnAccept();

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
						session->OnError();

						adaptor->Close();

						return;
					}

					if (adaptor->BindV4(session->_localEndpoint) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						session->OnError();

						adaptor->Close();

						return;
					}

					if (adaptor->ConnectV4(session->_peerEndpoint, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						session->OnConnectFailed();

						adaptor->Close();

						return;
					}

					auto pipe = std::make_shared<UDPSessionPipe>(session, _poller, adaptor);

					if (_isListen &&
						!_poller->AppendEvent(adaptor->Socket(), &pipe->_receiveContext, true, false))
					{
						session->OnError();

						session->Close();

						return;
					}

					pipe->_isConnect = true;

					session->_pipe = pipe;

					session->OnConnect();

					_acceptContext.temp[_acceptContext.bytes] = '\0';

					session->OnReceive(_acceptContext.temp, _acceptContext.bytes);

					if (!pipe->Receive())
					{
						session->OnReceiveFailed();

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

			_server->_peerEndpoint.port = htons(_acceptContext.address.v4.sin_port);
			_server->_peerEndpoint.host = ip::Address::AsString(_acceptContext.address.v4.sin_addr);

			auto adaptor = std::make_shared<UDPAdaptor>(_acceptContext.socket);

			adaptor->SetBlock(false);
			adaptor->SetReuseAddress(true);

			auto session = _server->OnAccept();

			if (session)
			{
				[&]()
				{
					session->_cacheSize = _server->_cacheSize;
					session->_peerEndpoint = _server->_peerEndpoint;
					session->_localEndpoint = _server->_localEndpoint;

					if (adaptor->BindV4(session->_localEndpoint) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						session->OnError();

						adaptor->Close();

						return;
					}

					if (adaptor->ConnectV4(session->_peerEndpoint, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						session->OnConnectFailed();

						adaptor->Close();

						return;
					}

					auto pipe = std::make_shared<UDPSessionPipe>(session, _poller, adaptor);

					if (_isListen &&
					    !_poller->AppendEvent(adaptor->Socket(), &pipe->_ioContext, true, false))
					{
						session->OnError();

						session->Close();

						return;
					}

					pipe->_isConnect = true;

					session->_pipe = pipe;

					session->OnConnect();

					_acceptContext.temp[_acceptContext.bytes] = '\0';

					session->OnReceive(_acceptContext.temp, _acceptContext.bytes);

					if (!pipe->Receive())
					{
						session->OnReceiveFailed();

						session->Close();

						return;
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
				_server->OnAcceptFailed();

				Close();

				return;
			}

			if (!Accept())
			{
				_server->OnAcceptFailed();

				return;
			}

			_server->_peerEndpoint.port = htons(_acceptContext.address.v4.sin_port);
			_server->_peerEndpoint.host = ip::Address::AsString(_acceptContext.address.v4.sin_addr);

			auto adaptor = std::make_shared<UDPAdaptor>(_acceptContext.socket);

			adaptor->SetBlock(false);
			adaptor->SetReuseAddress(true);

			auto session = _server->OnAccept();

			if (session)
			{
				[&]()
				{
					session->_cacheSize = _server->_cacheSize;
					session->_peerEndpoint = _server->_peerEndpoint;
					session->_localEndpoint = _server->_localEndpoint;

					if (adaptor->BindV4(session->_localEndpoint) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						session->OnError();

						adaptor->Close();

						return;
					}

					if (adaptor->ConnectV4(session->_peerEndpoint, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
					{
						session->OnConnectFailed();

						adaptor->Close();

						return;
					}

					auto pipe = std::make_shared<UDPSessionPipe>(session, _poller, adaptor);

					if (_isListen &&
					    !_poller->AppendEvent(adaptor->Socket(), &pipe->_ioContext, true, false))
					{
						session->OnError();

						session->Close();

						return;
					}

					pipe->_isConnect = true;

					session->_pipe = pipe;

					session->OnConnect();

					_acceptContext.temp[_acceptContext.bytes] = '\0';

					session->OnReceive(_acceptContext.temp, _acceptContext.bytes);

					if (!pipe->Receive())
					{
						session->OnReceiveFailed();

						session->Close();

						return;
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
