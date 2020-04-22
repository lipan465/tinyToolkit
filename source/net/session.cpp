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
		 * 设置接收缓存大小
		 *
		 * @param size 缓存大小
		 *
		 */
		void ISession::SetReceiveCacheSize(std::size_t size)
		{
			_receiveCacheSize = size;
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
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool ISession::IsValid()
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
		Poller * ISession::Pollers()
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
		 * 连接
		 *
		 * @param host 目标地址
		 * @param port 目标端口
		 *
		 * @return 是否连接成功
		 *
		 */
		bool ITCPSession::Connect(std::string host, uint16_t port)
		{
			return Pollers()->LaunchTCPSession(this, std::move(host), port);
		}

		/**
		 *
		 * 事件错误
		 *
		 */
		void ITCPSession::OnError()
		{

		}

		/**
		 *
		 * 会话连接
		 *
		 * @param callback 回调函数
		 *
		 */
		void ITCPSession::OnConnect()
		{

		}

		/**
		 *
		 * 断开连接
		 *
		 */
		void ITCPSession::OnDisconnect()
		{

		}

		/**
		 *
		 * 发送数据
		 *
		 */
		void ITCPSession::OnSend()
		{

		}

		/**
		 *
		 * 接收数据
		 *
		 * @param buffer 内容
		 * @param length 长度
		 *
		 * @return 偏移长度
		 *
		 */
		std::size_t ITCPSession::OnReceive(const char * buffer, std::size_t length)
		{
			(void)buffer;

			return length;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 连接
		 *
		 * @param host 目标地址
		 * @param port 目标端口
		 *
		 * @return 是否连接成功
		 *
		 */
		bool IUDPSession::Connect(std::string host, uint16_t port)
		{
			return Pollers()->LaunchUDPSession(this, std::move(host), port);
		}

		/**
		 *
		 * 事件错误
		 *
		 */
		void IUDPSession::OnError()
		{

		}

		/**
		 *
		 * 会话连接
		 *
		 * @param callback 回调函数
		 *
		 */
		void IUDPSession::OnConnect()
		{

		}

		/**
		 *
		 * 断开连接
		 *
		 */
		void IUDPSession::OnDisconnect()
		{

		}

		/**
		 *
		 * 发送数据
		 *
		 */
		void IUDPSession::OnSend()
		{

		}

		/**
		 *
		 * 接收数据
		 *
		 * @param buffer 内容
		 * @param length 长度
		 *
		 * @return 偏移长度
		 *
		 */
		std::size_t IUDPSession::OnReceive(const char * buffer, std::size_t length)
		{
			(void)buffer;

			return length;
		}
	}
}
