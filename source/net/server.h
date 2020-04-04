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
			friend class Poller;
			friend class UDPServerChannel;
			friend class TCPServerChannel;
			friend class UDPSessionChannel;
			friend class TCPSessionChannel;

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
			 * 轮询器
			 *
			 * @return 轮询器
			 *
			 */
			Poller * Pollers();

			/**
			 *
			 * 缓存大小
			 *
			 * @return 缓存大小
			 *
			 */
			std::size_t CacheSize() const;

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

			std::size_t _cacheSize{ 0 };

			std::shared_ptr<IChannel> _channel{ };
		};

		class TINY_TOOLKIT_API TCPServer : public IServer
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
			~TCPServer() override = default;

			/**
			 *
			 * 事件错误
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnError(std::function<void()> callback);

			/**
			 *
			 * 关闭连接
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnShutdown(std::function<void()> callback);

			/**
			 *
			 * 绑定地址
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnBind(std::function<void(bool)> callback);

			/**
			 *
			 * 套接字生成
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnSocket(std::function<void(bool)> callback);

			/**
			 *
			 * 监听地址
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnListen(std::function<void(bool)> callback);

			/**
			 *
			 * 接收会话
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnAccept(std::function<tinyToolkit::net::TCPSession *(bool)> callback);

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
			bool Launch(std::string host, uint16_t port, std::size_t cache);

		private:
			std::function<void()> _onError{ };
			std::function<void()> _onShutdown{ };

			std::function<void(bool)> _onBind{ };
			std::function<void(bool)> _onSocket{ };
			std::function<void(bool)> _onListen{ };

			std::function<tinyToolkit::net::TCPSession *(bool)> _onAccept{ };
		};

		class TINY_TOOLKIT_API UDPServer : public IServer
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
			~UDPServer() override = default;

			/**
			 *
			 * 事件错误
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnError(std::function<void()> callback);

			/**
			 *
			 * 关闭连接
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnShutdown(std::function<void()> callback);

			/**
			 *
			 * 绑定地址
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnBind(std::function<void(bool)> callback);

			/**
			 *
			 * 套接字生成
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnSocket(std::function<void(bool)> callback);

			/**
			 *
			 * 监听地址
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnListen(std::function<void(bool)> callback);

			/**
			 *
			 * 接收会话
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnAccept(std::function<tinyToolkit::net::UDPSession *(bool)> callback);

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
			bool Launch(std::string host, uint16_t port, std::size_t cache);

		private:
			std::function<void()> _onError{ };
			std::function<void()> _onShutdown{ };

			std::function<void(bool)> _onBind{ };
			std::function<void(bool)> _onSocket{ };
			std::function<void(bool)> _onListen{ };

			std::function<tinyToolkit::net::UDPSession *(bool)> _onAccept{ };
		};
	}
}


#endif // __TINY_TOOLKIT__NET__SERVER__H__
