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
		 * 事件错误
		 *
		 */
		void IServer::OnError()
		{

		}

		/**
		 *
		 * 套接字生成成功
		 *
		 */
		void IServer::OnSocket()
		{

		}

		/**
		 *
		 * 套接字生成失败
		 *
		 */
		void IServer::OnSocketFailed()
		{

		}

		/**
		 *
		 * 绑定成功
		 *
		 */
		void IServer::OnBind()
		{

		}

		/**
		 *
		 * 绑定失败
		 *
		 */
		void IServer::OnBindFailed()
		{

		}

		/**
		 *
		 * 监听成功
		 *
		 */
		void IServer::OnListen()
		{

		}

		/**
		 *
		 * 监听失败
		 *
		 */
		void IServer::OnListenFailed()
		{

		}

		/**
		 *
		 * 接受连接成功
		 *
		 * @return 会话
		 *
		 */
		tinyToolkit::net::ITCPSession * IServer::OnAccept()
		{
			return nullptr;
		}

		/**
		 *
		 * 接受连接失败
		 *
		 */
		void IServer::OnAcceptFailed()
		{

		}

		/**
		 *
		 * 断开连接
		 *
		 */
		void IServer::OnDisconnect()
		{

		}

		/**
		 *
		 * 关闭会话
		 *
		 */
		void IServer::Close()
		{
			_pipe->Close();
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
			return _cacheSize;
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
			if (_pipe && !_peerEndpoint.IsValid())
			{
				auto endpoint = ip::Socket::PeerEndpointV4(_pipe->Socket());

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
			if (_pipe && !_localEndpoint.IsValid())
			{
				auto endpoint = ip::Socket::LocalEndpointV4(_pipe->Socket());

				_localEndpoint.host = endpoint.first;
				_localEndpoint.port = endpoint.second;
			}

			return _localEndpoint;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


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
		bool ITCPServer::Launch(std::string host, uint16_t port, std::size_t cache, Poller * poller)
		{
			if (poller)
			{
				return poller->LaunchTCPServer(this, std::move(host), port, cache);
			}
			else
			{
				return util::Singleton<Poller>::Instance().LaunchTCPServer(this, std::move(host), port, cache);
			}
		}
	}
}
