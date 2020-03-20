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
		 * 关闭会话
		 *
		 */
		void ISession::Close()
		{
			if (_pipe == nullptr)
			{
				return;
			}

			_pipe->Close();
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
			if (_pipe == nullptr)
			{
				return false;
			}

			return _pipe->Append(buffer, length);
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
			if (_pipe == nullptr)
			{
				return 0;
			}

			return _pipe->CacheSize();
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
			if (_pipe == nullptr)
			{
				return 0;
			}

			return _pipe->RemainMessageCount();
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
			if (_pipe == nullptr)
			{
				return TINY_TOOLKIT_SOCKET_INVALID;
			}

			return _pipe->Socket();
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
		const Endpoint & ISession::LocalEndpoint() const
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
		 * @param host 目标地址
		 * @param port 目标端口
		 * @param cache 缓存大小
		 * @param poller 轮询器
		 *
		 * @return 是否启动成功
		 *
		 */
		bool ITCPSession::Launch(std::string host, uint16_t port, std::size_t cache, Poller * poller)
		{
			if (poller)
			{
				return poller->LaunchTCPSession(this, std::move(host), port, cache);
			}
			else
			{
				return util::Singleton<Poller>::Instance().LaunchTCPSession(this, std::move(host), port, cache);
			}
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
		 * 套接字生成成功
		 *
		 */
		void ITCPSession::OnSocket()
		{

		}

		/**
		 *
		 * 套接字生成失败
		 *
		 */
		void ITCPSession::OnSocketFailed()
		{

		}

		/**
		 *
		 * 连接成功
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
		 * 连接失败
		 *
		 */
		void ITCPSession::OnConnectFailed()
		{

		}

		/**
		 *
		 * 发送成功
		 *
		 */
		void ITCPSession::OnSend()
		{

		}

		/**
		 *
		 * 发送失败
		 *
		 */
		void ITCPSession::OnSendFailed()
		{

		}

		/**
		 *
		 * 接收成功
		 *
		 * @param buffer 内容
		 * @param length 长度
		 *
		 * @return 使用的字节数
		 *
		 */
		std::size_t ITCPSession::OnReceive(const char * buffer, std::size_t length)
		{
			(void)buffer;

			return length;
		}

		/**
		 *
		 * 接收失败
		 *
		 */
		void ITCPSession::OnReceiveFailed()
		{

		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 启动
		 *
		 * @param host 目标地址
		 * @param port 目标端口
		 * @param cache 缓存大小
		 * @param poller 轮询器
		 *
		 * @return 是否启动成功
		 *
		 */
		bool IUDPSession::Launch(std::string host, uint16_t port, std::size_t cache, Poller * poller)
		{
			if (poller)
			{
				return poller->LaunchUDPSession(this, std::move(host), port, cache);
			}
			else
			{
				return util::Singleton<Poller>::Instance().LaunchUDPSession(this, std::move(host), port, cache);
			}
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
		 * 套接字生成成功
		 *
		 */
		void IUDPSession::OnSocket()
		{

		}

		/**
		 *
		 * 套接字生成失败
		 *
		 */
		void IUDPSession::OnSocketFailed()
		{

		}

		/**
		 *
		 * 连接成功
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
		 * 连接失败
		 *
		 */
		void IUDPSession::OnConnectFailed()
		{

		}

		/**
		 *
		 * 发送成功
		 *
		 */
		void IUDPSession::OnSend()
		{

		}

		/**
		 *
		 * 发送失败
		 *
		 */
		void IUDPSession::OnSendFailed()
		{

		}

		/**
		 *
		 * 接收成功
		 *
		 * @param buffer 内容
		 * @param length 长度
		 *
		 * @return 使用的字节数
		 *
		 */
		std::size_t IUDPSession::OnReceive(const char * buffer, std::size_t length)
		{
			(void)buffer;

			return length;
		}

		/**
		 *
		 * 接收失败
		 *
		 */
		void IUDPSession::OnReceiveFailed()
		{

		}
	}
}
