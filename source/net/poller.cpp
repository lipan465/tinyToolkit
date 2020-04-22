/**
 *
 *  作者: hm
 *
 *  说明: 事件
 *
 */


#include "tcp.h"
#include "udp.h"
#include "poller.h"
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
		 */
		Poller::Poller()
		{
			_socket = ip::Socket::PollSocket();

			if (!IsValid())
			{
				return;
			}

			_thread = std::thread
			(
				[this]()
				{
					while (_status)
					{
					#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

						DWORD bytes = 0;

						Context * context = nullptr;

						TINY_TOOLKIT_SOCKET_TYPE socket = TINY_TOOLKIT_SOCKET_INVALID;

						if (GetQueuedCompletionStatus(_socket, &bytes, (PULONG_PTR)&socket, (LPOVERLAPPED *)&context, TINY_TOOLKIT_SOCKET_POLL_TIMEOUT))
						{
							if (context && context->completer)
							{
								context->bytes = bytes;

								context->completer->Process(context, nullptr);
							}
						}
						else
						{
							if (context && context->completer)
							{
								context->bytes = bytes;

								context->completer->Process(context, nullptr);
							}
							else
							{
								if (GetLastError() != WAIT_TIMEOUT)
								{
									break;
								}
							}
						}

					#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

						struct kevent events[32]{ };

						struct timespec timestamp{ };

						timestamp.tv_sec = 0;
						timestamp.tv_nsec = TINY_TOOLKIT_SOCKET_POLL_TIMEOUT * 1000000;

						int32_t count = kevent
						(
							_socket,
							nullptr,
							0,
							events,
							static_cast<int32_t>(sizeof(events) / sizeof(events[0])),
							&timestamp
						);

						if (count == -1)
						{
							if (errno != EINTR)
							{
								break;
							}
						}

						for (int32_t i = 0; i < count; ++i)
						{
							auto * context = reinterpret_cast<Context *>(events[i].udata);

							if (context && context->completer)
							{
								context->completer->Process(context, &events[i]);
							}
						}

					#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

						struct epoll_event events[32]{ };

						int32_t count = epoll_wait
						(
							_socket,
							events,
							static_cast<int32_t>(sizeof(events) / sizeof(events[0])),
							TINY_TOOLKIT_SOCKET_POLL_TIMEOUT
						);

						if (count == -1)
						{
							if (errno != EINTR)
							{
								break;
							}
						}

						for (int32_t i = 0; i < count; ++i)
						{
							auto * context = reinterpret_cast<Context *>(events[i].data.ptr);

							if (context && context->completer)
							{
								context->completer->Process(context, &events[i]);
							}
						}

					#else

						break;

					#endif
					}
				}
			);
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		Poller::~Poller()
		{
			if (!_status)
			{
				return;
			}

			_status = false;

			if (_thread.joinable())
			{
				_thread.join();
			}

			ip::Socket::ClosePoll(_socket);

			_socket = TINY_TOOLKIT_SOCKET_POLL_INVALID;
		}

		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool Poller::IsValid()
		{
			return ip::Socket::IsValidPoll(_socket);
		}

		/**
		 *
		 * 启动tcp服务
		 *
		 * @param server 服务
		 * @param host 本地地址
		 * @param port 本地端口
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Poller::LaunchTCPServer(ITCPServer * server, std::string host, uint16_t port)
		{
			if (!IsValid() || server == nullptr)
			{
				return false;
			}

			server->_peerEndpoint.host = "0.0.0.0";
			server->_peerEndpoint.port = 0;

			server->_localEndpoint.host = std::move(host);
			server->_localEndpoint.port = port;

			auto adaptor = std::make_shared<TCPAdaptor>();

			if (!adaptor->IsValid())
			{
				server->OnError();

				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (!adaptor->SetDelay(false) ||
			    !adaptor->SetBlock(false) ||
			    !adaptor->SetReuseAddress(true))
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

		#else

			if (!adaptor->SetDelay(false) ||
			    !adaptor->SetBlock(false) ||
			    !adaptor->SetReusePort(true) ||
			    !adaptor->SetReuseAddress(true))
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

		#endif

			if (adaptor->BindV4(server->_localEndpoint) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

			if (adaptor->Listen(TINY_TOOLKIT_SOCKET_LISTEN_COUNT) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

			auto channel = std::make_shared<TCPServerChannel>(server, adaptor);

			if (!AppendEvent(adaptor->Socket(), &channel->_acceptContext, true, false))
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (!channel->Accept())
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

		#endif

			server->_channel = channel;

			return true;
		}

		/**
		 *
		 * 启动udp服务
		 *
		 * @param server 服务
		 * @param host 本地地址
		 * @param port 本地端口
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Poller::LaunchUDPServer(IUDPServer * server, std::string host, uint16_t port)
		{
			if (!IsValid() || server == nullptr)
			{
				return false;
			}

			server->_peerEndpoint.host = "0.0.0.0";
			server->_peerEndpoint.port = 0;

			server->_localEndpoint.host = std::move(host);
			server->_localEndpoint.port = port;

			auto adaptor = std::make_shared<UDPAdaptor>();

			if (!adaptor->IsValid())
			{
				server->OnError();

				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (!adaptor->SetBlock(false) ||
			    !adaptor->SetReuseAddress(true))
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

			BOOL bNewBehavior = FALSE;

			DWORD dwBytesReturned = 0;

			if (::WSAIoctl(adaptor->Socket(), SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), nullptr, 0, &dwBytesReturned, nullptr, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

		#else

			if (!adaptor->SetBlock(false) ||
			    !adaptor->SetReusePort(true) ||
			    !adaptor->SetReuseAddress(true))
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

		#endif

			if (adaptor->BindV4(server->_localEndpoint) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

			auto channel = std::make_shared<UDPServerChannel>(server, adaptor);

			if (!AppendEvent(adaptor->Socket(), &channel->_acceptContext, true, false))
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (!channel->Accept())
			{
				server->OnError();

				adaptor->Close();

				return false;
			}

		#endif

			server->_channel = channel;

			return true;
		}

		/**
		 *
		 * 启动tcp会话
		 *
		 * @param session 会话
		 * @param host 目标地址
		 * @param port 目标端口
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Poller::LaunchTCPSession(ITCPSession * session, std::string host, uint16_t port)
		{
			if (!IsValid() || session == nullptr)
			{
				return false;
			}

			session->_peerEndpoint.host = std::move(host);
			session->_peerEndpoint.port = port;

			session->_localEndpoint.host = "0.0.0.0";
			session->_localEndpoint.port = 0;

			auto adaptor = std::make_shared<TCPAdaptor>();

			if (!adaptor->IsValid())
			{
				session->OnError();

				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (!adaptor->SetDelay(false) ||
			    !adaptor->SetBlock(false) ||
			    !adaptor->SetReuseAddress(true))
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

			if (adaptor->BindV4(session->_localEndpoint) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

			auto channel = std::make_shared<TCPSessionChannel>(session, adaptor);

			channel->_receiveContext.optionType = NET_OPTION_TYPE::CONNECT;

			if (!AppendEvent(adaptor->Socket(), &channel->_receiveContext, true, false))
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

			auto ret = adaptor->ConnectV4(session->_peerEndpoint, &channel->_receiveContext);

			if (ret < 0)
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

			session->_channel = channel;

		#else

			if (!adaptor->SetDelay(false) ||
			    !adaptor->SetBlock(false) ||
			    !adaptor->SetReusePort(true) ||
			    !adaptor->SetReuseAddress(true))
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

			auto ret = adaptor->ConnectV4(session->_peerEndpoint, nullptr);

			if (ret == 0)
			{
				auto channel = std::make_shared<TCPSessionChannel>(session, adaptor);

				if (!AppendEvent(adaptor->Socket(), &channel->_ioContext, true, false))
				{
					session->OnError();

					adaptor->Close();

					return false;
				}

				channel->_isConnect = true;

				session->_channel = channel;

				session->OnConnect();
			}
			else if (ret < 0 && errno != EINPROGRESS)
			{
				session->OnError();

				adaptor->Close();

				return false;
			}
			else
			{
				auto channel = std::make_shared<TCPSessionChannel>(session, adaptor);

				channel->_ioContext.optionType = NET_OPTION_TYPE::CONNECT;

				if (!AppendEvent(adaptor->Socket(), &channel->_ioContext, false, true))
				{
					session->OnError();

					adaptor->Close();

					return false;
				}

				session->_channel = channel;
			}

		#endif

			return true;
		}

		/**
		 *
		 * 启动udp会话
		 *
		 * @param session 会话
		 * @param host 目标地址
		 * @param port 目标端口
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Poller::LaunchUDPSession(IUDPSession * session, std::string host, uint16_t port)
		{
			if (!IsValid() || session == nullptr)
			{
				return false;
			}

			session->_peerEndpoint.host = std::move(host);
			session->_peerEndpoint.port = port;

			session->_localEndpoint.host = "0.0.0.0";
			session->_localEndpoint.port = 0;

			auto adaptor = std::make_shared<UDPAdaptor>();

			if (!adaptor->IsValid())
			{
				session->OnError();

				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (!adaptor->SetBlock(false) ||
			    !adaptor->SetReuseAddress(true))
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

			BOOL bNewBehavior = FALSE;

			DWORD dwBytesReturned = 0;

			if (::WSAIoctl(adaptor->Socket(), SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), nullptr, 0, &dwBytesReturned, nullptr, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

		#else

			if (!adaptor->SetBlock(false) ||
			    !adaptor->SetReusePort(true) ||
			    !adaptor->SetReuseAddress(true))
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

		#endif

			if (adaptor->BindV4(session->_localEndpoint) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

			if (adaptor->ConnectV4(session->_peerEndpoint, nullptr) == TINY_TOOLKIT_SOCKET_ERROR)
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			auto channel = std::make_shared<UDPSessionChannel>(session, adaptor);

			if (!AppendEvent(adaptor->Socket(), &channel->_receiveContext, true, false))
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

			channel->_isConnect = true;

			session->_channel = channel;

			session->OnConnect();

			if (!channel->Receive())
			{
				session->OnError();

				session->Close();

				return false;
			}

		#else

			auto channel = std::make_shared<UDPSessionChannel>(session, adaptor);

			if (!AppendEvent(adaptor->Socket(), &channel->_ioContext, true, false))
			{
				session->OnError();

				adaptor->Close();

				return false;
			}

			channel->_isConnect = true;

			session->_channel = channel;

			session->OnConnect();

		#endif

			return true;
		}

		/**
		 *
		 * 移除事件监控
		 *
		 * @param socket 句柄
		 * @param context 上下文
		 *
		 * @return 是否移除成功
		 *
		 */
		bool Poller::RemoveEvent(TINY_TOOLKIT_SOCKET_TYPE socket, Context * context)
		{
			(void)context;

			std::lock_guard<std::mutex> lock(_mutex);

			if (!IsValid() || !ip::Socket::IsValid(socket))
			{
				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], socket, EVFILT_READ,  EV_DELETE, 0, 0, context);
			EV_SET(&event[1], socket, EVFILT_WRITE, EV_DELETE, 0, 0, context);

			return kevent(_socket, event, 2, nullptr, 0, nullptr) != -1;

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			struct epoll_event event{ };

			event.events =  EPOLLIN | EPOLLOUT;
			event.data.ptr = context;

			return epoll_ctl(_socket, EPOLL_CTL_DEL, socket, &event) != -1;

		#else

			return true;

		#endif
		}

		/**
		 *
		 * 修改事件监控
		 *
		 * @param socket 句柄
		 * @param context 上下文
		 * @param readFlag 读监听
		 * @param writeFlag 写监听
		 *
		 * @return 是否修改成功
		 *
		 */
		bool Poller::ModifyEvent(TINY_TOOLKIT_SOCKET_TYPE socket, Context * context, bool readFlag, bool writeFlag)
		{
			(void)context;
			(void)readFlag;
			(void)writeFlag;

			std::lock_guard<std::mutex> lock(_mutex);

			if (!IsValid() || !ip::Socket::IsValid(socket))
			{
				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], socket, EVFILT_READ,  readFlag  ? EV_ENABLE : EV_DISABLE, 0, 0, context);
			EV_SET(&event[1], socket, EVFILT_WRITE, writeFlag ? EV_ENABLE : EV_DISABLE, 0, 0, context);

			return kevent(_socket, event, 2, nullptr, 0, nullptr) != -1;

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			struct epoll_event event{ };

			event.events = (readFlag ? EPOLLIN : 0) | (writeFlag ? EPOLLOUT : 0);
			event.data.ptr = context;

			return epoll_ctl(_socket, EPOLL_CTL_MOD, socket, &event) != -1;

		#else

			return false;

		#endif
		}

		/**
		 *
		 * 添加事件监控
		 *
		 * @param socket 句柄
		 * @param context 上下文
		 * @param readFlag 读监听
		 * @param writeFlag 写监听
		 *
		 * @return 是否添加成功
		 *
		 */
		bool Poller::AppendEvent(TINY_TOOLKIT_SOCKET_TYPE socket, Context * context, bool readFlag, bool writeFlag)
		{
			(void)context;
			(void)readFlag;
			(void)writeFlag;

			std::lock_guard<std::mutex> lock(_mutex);

			if (!IsValid() || !ip::Socket::IsValid(socket))
			{
				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			return CreateIoCompletionPort
			(
				reinterpret_cast<HANDLE>(socket),
				_socket,
				static_cast<ULONG_PTR>(socket),
				0
			) == _socket;

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			struct kevent event[2]{ };

			EV_SET(&event[0], socket, EVFILT_READ,  EV_ADD | (readFlag  ? EV_ENABLE : EV_DISABLE), 0, 0, context);
			EV_SET(&event[1], socket, EVFILT_WRITE, EV_ADD | (writeFlag ? EV_ENABLE : EV_DISABLE), 0, 0, context);

			return kevent(_socket, event, 2, nullptr, 0, nullptr) != -1;

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			struct epoll_event event{ };

			event.events = (readFlag ? EPOLLIN : 0) | (writeFlag ? EPOLLOUT : 0);
			event.data.ptr = context;

			return epoll_ctl(_socket, EPOLL_CTL_ADD, socket, &event) != -1;

		#else

			return false;

		#endif
		}
	}
}
