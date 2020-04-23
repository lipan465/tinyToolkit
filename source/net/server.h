#ifndef __TINY_TOOLKIT__NET__SERVER__H__
#define __TINY_TOOLKIT__NET__SERVER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 服务
 *
 */


#include "session.h"


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API IServer
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param poller 轮询器
			 *
			 */
			explicit IServer(Poller * poller = nullptr);

			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~IServer() = default;

			/**
			 *
			 * 关闭会话
			 *
			 */
			void Close();

			/**
			 *
			 * 是否有效
			 *
			 * @return 是否有效
			 *
			 */
			bool IsValid() const;

			/**
			 *
			 * 轮询器
			 *
			 * @return 轮询器
			 *
			 */
			Poller * Pollers();

			/**
			 *
			 * 套接字
			 *
			 * @return 套接字
			 *
			 */
			TINY_TOOLKIT_SOCKET_TYPE Socket() const;

			/**
			 *
			 * 目标端点
			 *
			 * @return 目标端点
			 *
			 */
			const Endpoint & PeerEndpoint() const;

			/**
			 *
			 * 本地端点
			 *
			 * @return 本地端点
			 *
			 */
			const Endpoint & LocalEndpoint() const;

		protected:
			mutable Endpoint _peerEndpoint{ };
			mutable Endpoint _localEndpoint{ };

			Poller * _poller{ nullptr };

			std::shared_ptr<IChannel> _channel{ };
		};

		class TINY_TOOLKIT_API ITCPServer : public IServer
		{
			friend class Poller;
			friend class UDPServerChannel;
			friend class TCPServerChannel;
			friend class UDPSessionChannel;
			friend class TCPSessionChannel;

			using IServer::IServer;

		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			~ITCPServer() override = default;

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
			bool Listen(std::string host, uint16_t port);

		protected:
			/**
			 *
			 * 事件错误
			 *
			 */
			virtual void OnError();

			/**
			 *
			 * 关闭连接
			 *
			 */
			virtual void OnShutdown();

			/**
			 *
			 * 接收会话
			 *
			 * @return 会话对象
			 *
			 */
			virtual tinyToolkit::net::ITCPSession * OnAccept();
		};

		class TINY_TOOLKIT_API IUDPServer : public IServer
		{
			friend class Poller;
			friend class UDPServerChannel;
			friend class TCPServerChannel;
			friend class UDPSessionChannel;
			friend class TCPSessionChannel;

			using IServer::IServer;

		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			~IUDPServer() override = default;

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
			bool Listen(std::string host, uint16_t port);

		protected:
			/**
			 *
			 * 事件错误
			 *
			 */
			virtual void OnError();

			/**
			 *
			 * 关闭连接
			 *
			 */
			virtual void OnShutdown();

			/**
			 *
			 * 接收会话
			 *
			 * @return 会话对象
			 *
			 */
			virtual tinyToolkit::net::IUDPSession * OnAccept();
		};
	}
}


#endif // __TINY_TOOLKIT__NET__SERVER__H__
