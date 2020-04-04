/**
 *
 *  作者: hm
 *
 *  说明: 会话
 *
 */


#include "poller.h"
#include "session.h"

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
		ISession::ISession(Poller * poller) : _poller(poller ? poller : &util::Singleton<Poller>::Instance())
		{

		}

		/**
		 *
		 * 关闭会话
		 *
		 */
		void ISession::Close()
		{
			if (_channel == nullptr)
			{
				return;
			}

			_channel->Close();
		}

		/**
		 *
		 * 添加消息
		 *
		 * @param buffer 内容
		 * @param length 长度
		 *
		 * @return 是否添加成功
		 *
		 */
		bool ISession::Send(const void * buffer, std::size_t length)
		{
			if (_channel == nullptr)
			{
				return false;
			}

			return _channel->Append(buffer, length);
		}

		/**
		 *
		 * 轮询器
		 *
		 * @return 轮询器
		 *
		 */
		Poller * ISession::Pollers()
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
		std::size_t ISession::CacheSize() const
		{
			if (_channel == nullptr)
			{
				return 0;
			}

			return _channel->CacheSize();
		}

		/**
		 *
		 * 剩余消息个数
		 *
		 * @return 剩余消息个数
		 *
		 */
		std::size_t ISession::RemainMessageCount() const
		{
			if (_channel == nullptr)
			{
				return 0;
			}

			return _channel->RemainMessageCount();
		}

		/**
		 *
		 * 套接字
		 *
		 * @return 套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE ISession::Socket() const
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
		const Endpoint & ISession::PeerEndpoint() const
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
		const Endpoint & ISession::LocalEndpoint() const
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
		 */
		void TCPSession::OnError(std::function<void()> callback)
		{
			_onError = std::move(callback);
		}

		/**
		 *
		 * 断开连接
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPSession::OnDisconnect(std::function<void()> callback)
		{
			_onDisconnect = std::move(callback);
		}

		/**
		 *
		 * 绑定地址
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPSession::OnBind(std::function<void(bool)> callback)
		{
			_onBind = std::move(callback);
		}

		/**
		 *
		 * 发送数据
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPSession::OnSend(std::function<void(bool)> callback)
		{
			_onSend = std::move(callback);
		}

		/**
		 *
		 * 套接字生成
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPSession::OnSocket(std::function<void(bool)> callback)
		{
			_onSocket = std::move(callback);
		}

		/**
		 *
		 * 会话连接
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPSession::OnConnect(std::function<void(bool)> callback)
		{
			_onConnect = std::move(callback);
		}

		/**
		 *
		 * 接收数据
		 *
		 * @param callback 回调函数
		 *
		 */
		void TCPSession::OnReceive(std::function<std::size_t(bool, const char *, std::size_t)> callback)
		{
			_onReceive = std::move(callback);
		}

		/**
		 *
		 * 启动
		 *
		 * @param host 目标地址
		 * @param port 目标端口
		 * @param cache 缓存大小
		 *
		 * @return 是否启动成功
		 *
		 */
		bool TCPSession::Launch(std::string host, uint16_t port, std::size_t cache)
		{
			return Pollers()->LaunchTCPSession(this, std::move(host), port, cache);
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 事件错误
		 *
		 */
		void UDPSession::OnError(std::function<void()> callback)
		{
			_onError = std::move(callback);
		}

		/**
		 *
		 * 断开连接
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPSession::OnDisconnect(std::function<void()> callback)
		{
			_onDisconnect = std::move(callback);
		}

		/**
		 *
		 * 绑定地址
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPSession::OnBind(std::function<void(bool)> callback)
		{
			_onBind = std::move(callback);
		}

		/**
		 *
		 * 发送数据
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPSession::OnSend(std::function<void(bool)> callback)
		{
			_onSend = std::move(callback);
		}

		/**
		 *
		 * 套接字生成
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPSession::OnSocket(std::function<void(bool)> callback)
		{
			_onSocket = std::move(callback);
		}

		/**
		 *
		 * 会话连接
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPSession::OnConnect(std::function<void(bool)> callback)
		{
			_onConnect = std::move(callback);
		}

		/**
		 *
		 * 接收数据
		 *
		 * @param callback 回调函数
		 *
		 */
		void UDPSession::OnReceive(std::function<std::size_t(bool, const char *, std::size_t)> callback)
		{
			_onReceive = std::move(callback);
		}

		/**
		 *
		 * 启动
		 *
		 * @param host 目标地址
		 * @param port 目标端口
		 * @param cache 缓存大小
		 *
		 * @return 是否启动成功
		 *
		 */
		bool UDPSession::Launch(std::string host, uint16_t port, std::size_t cache)
		{
			return Pollers()->LaunchUDPSession(this, std::move(host), port, cache);
		}
	}
}
