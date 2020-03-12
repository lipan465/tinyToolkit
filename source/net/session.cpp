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
		 * 事件错误
		 *
		 */
		void ISession::OnError()
		{

		}

		/**
		 *
		 * 套接字生成成功
		 *
		 */
		void ISession::OnSocket()
		{

		}

		/**
		 *
		 * 套接字生成失败
		 *
		 */
		void ISession::OnSocketFailed()
		{

		}

		/**
		 *
		 * 绑定成功
		 *
		 */
		void ISession::OnBind()
		{

		}

		/**
		 *
		 * 绑定失败
		 *
		 */
		void ISession::OnBindFailed()
		{

		}

		/**
		 *
		 * 连接成功
		 *
		 */
		void ISession::OnConnect()
		{

		}

		/**
		 *
		 * 断开连接
		 *
		 */
		void ISession::OnDisconnect()
		{

		}

		/**
		 *
		 * 连接失败
		 *
		 */
		void ISession::OnConnectFailed()
		{

		}

		/**
		 *
		 * 发送成功
		 *
		 */
		void ISession::OnSend()
		{

		}

		/**
		 *
		 * 发送失败
		 *
		 */
		void ISession::OnSendFailed()
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
		std::size_t ISession::OnReceive(const char * buffer, std::size_t length)
		{
			(void)buffer;

			return length;
		}

		/**
		 *
		 * 接收失败
		 *
		 */
		void ISession::OnReceiveFailed()
		{

		}

		/**
		 *
		 * 关闭会话
		 *
		 */
		void ISession::Close()
		{
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
			return _pipe->Append(buffer, length);
		}

		/**
		 *
		 * 是否缓存已满
		 *
		 * @return 是否缓存已满
		 *
		 */
		bool ISession::IsCacheFull()
		{
			return _pipe->IsCacheFull();
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
			return _pipe->RemainMessageCount();
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
	}
}
