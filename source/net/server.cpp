/**
 *
 *  作者: hm
 *
 *  说明: 服务
 *
 */


#include "poller.h"
#include "server.h"

#include "../ip/socket.h"

#include "../util/singleton.h"


namespace tinyToolkit
{
	namespace net
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param poller 轮询器
		 *
		 */
		IServer::IServer(Poller * poller) : _poller(poller ? poller : &util::Singleton<Poller>::Instance())
		{

		}

		/**
		 *
		 * 关闭会话
		 *
		 */
		void IServer::Close()
		{
			if (_channel == nullptr)
			{
				return;
			}

			_channel->Close();
		}

		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool IServer::IsValid() const
		{
			if (_channel == nullptr)
			{
				return false;
			}

			return _channel->Socket() != TINY_TOOLKIT_SOCKET_INVALID;
		}

		/**
		 *
		 * 轮询器
		 *
		 * @return 轮询器
		 *
		 */
		Poller * IServer::Pollers()
		{
			return _poller;
		}

		/**
		 *
		 * 套接字
		 *
		 * @return 套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE IServer::Socket() const
		{
			if (_channel == nullptr)
			{
				return TINY_TOOLKIT_SOCKET_INVALID;
			}

			return _channel->Socket();
		}

		/**
		 *
		 * 目标端点
		 *
		 * @return 目标端点
		 *
		 */
		const Endpoint & IServer::PeerEndpoint() const
		{
			if (_channel && !_peerEndpoint.IsValid())
			{
				auto endpoint = ip::Socket::PeerEndpointV4(_channel->Socket());

				_peerEndpoint.host = endpoint.first;
				_peerEndpoint.port = endpoint.second;
			}

			return _peerEndpoint;
		}

		/**
		 *
		 * 本地端点
		 *
		 * @return 本地端点
		 *
		 */
		const Endpoint & IServer::LocalEndpoint() const
		{
			if (_channel && !_localEndpoint.IsValid())
			{
				auto endpoint = ip::Socket::LocalEndpointV4(_channel->Socket());

				_localEndpoint.host = endpoint.first;
				_localEndpoint.port = endpoint.second;
			}

			return _localEndpoint;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 监听
		 *
		 * @param host 本地地址
		 * @param port 本地端口
		 *
		 * @return 是否监听成功
		 *
		 */
		bool ITCPServer::Listen(std::string host, uint16_t port)
		{
			return Pollers()->LaunchTCPServer(this, std::move(host), port);
		}

		/**
		 *
		 * 事件错误
		 *
		 */
		void ITCPServer::OnError()
		{

		}

		/**
		 *
		 * 关闭连接
		 *
		 */
		void ITCPServer::OnShutdown()
		{

		}

		/**
		 *
		 * 接收会话
		 *
		 * @return 会话对象
		 *
		 */
		tinyToolkit::net::ITCPSession * ITCPServer::OnAccept()
		{
			return nullptr;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 监听
		 *
		 * @param host 本地地址
		 * @param port 本地端口
		 *
		 * @return 是否监听成功
		 *
		 */
		bool IUDPServer::Listen(std::string host, uint16_t port)
		{
			return Pollers()->LaunchUDPServer(this, std::move(host), port);
		}

		/**
		 *
		 * 事件错误
		 *
		 */
		void IUDPServer::OnError()
		{

		}

		/**
		 *
		 * 关闭连接
		 *
		 */
		void IUDPServer::OnShutdown()
		{

		}

		/**
		 *
		 * 接收会话
		 *
		 * @return 会话对象
		 *
		 */
		tinyToolkit::net::IUDPSession * IUDPServer::OnAccept()
		{
			return nullptr;
		}
	}
}
