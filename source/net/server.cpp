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
		 * 缓存大小
		 *
		 * @return 缓存大小
		 *
		 */
		std::size_t IServer::CacheSize() const
		{
			if (_channel == nullptr)
			{
				return 0;
			}

			return _channel->CacheSize();
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
		 * 事件错误
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPServer::OnError(std::function<void()> callback)
		{
			_onError = std::move(callback);
		}

		/**
		 *
		 * 关闭连接
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPServer::OnShutdown(std::function<void()> callback)
		{
			_onShutdown = std::move(callback);
		}

		/**
		 *
		 * 绑定地址
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPServer::OnBind(std::function<void(bool)> callback)
		{
			_onBind = std::move(callback);
		}

		/**
		 *
		 * 套接字生成
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPServer::OnSocket(std::function<void(bool)> callback)
		{
			_onSocket = std::move(callback);
		}

		/**
		 *
		 * 监听地址
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPServer::OnListen(std::function<void(bool)> callback)
		{
			_onListen = std::move(callback);
		}

		/**
		 *
		 * 接收会话
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPServer::OnAccept(std::function<tinyToolkit::net::TCPSession *(bool)> callback)
		{
			_onAccept = std::move(callback);
		}

		/**
		 *
		 * 启动
		 *
		 * @param host 本地地址
		 * @param port 本地端口
		 * @param cache 缓存大小
		 * @param poller 轮询器
		 *
		 * @return 是否启动成功
		 *
		 */
		bool TCPServer::Launch(std::string host, uint16_t port, std::size_t cache)
		{
			return Pollers()->LaunchTCPServer(this, std::move(host), port, cache);
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 事件错误
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPServer::OnError(std::function<void()> callback)
		{
			_onError = std::move(callback);
		}

		/**
		 *
		 * 关闭连接
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPServer::OnShutdown(std::function<void()> callback)
		{
			_onShutdown = std::move(callback);
		}

		/**
		 *
		 * 绑定地址
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPServer::OnBind(std::function<void(bool)> callback)
		{
			_onBind = std::move(callback);
		}

		/**
		 *
		 * 套接字生成
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPServer::OnSocket(std::function<void(bool)> callback)
		{
			_onSocket = std::move(callback);
		}

		/**
		 *
		 * 监听地址
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPServer::OnListen(std::function<void(bool)> callback)
		{
			_onListen = std::move(callback);
		}

		/**
		 *
		 * 接收会话
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPServer::OnAccept(std::function<tinyToolkit::net::UDPSession *(bool)> callback)
		{
			_onAccept = std::move(callback);
		}

		/**
		 *
		 * 启动
		 *
		 * @param host 本地地址
		 * @param port 本地端口
		 * @param cache 缓存大小
		 *
		 * @return 是否启动成功
		 *
		 */
		bool UDPServer::Launch(std::string host, uint16_t port, std::size_t cache)
		{
			return Pollers()->LaunchUDPServer(this, std::move(host), port, cache);
		}
	}
}
