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
			friend class UDPServerPipe;
			friend class TCPServerPipe;
			friend class UDPSessionPipe;
			friend class TCPSessionPipe;

		public:
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

		private:
			mutable Endpoint _peerEndpoint{ };
			mutable Endpoint _localEndpoint{ };

			std::size_t _cacheSize{ 0 };

			std::shared_ptr<IPipe> _pipe{ };
		};

		class TINY_TOOLKIT_API ITCPServer : public IServer
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			~ITCPServer() override = default;

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
			bool Launch(std::string host, uint16_t port, std::size_t cache, Poller * poller = nullptr);

			/**
			 *
			 * 事件错误
			 *
			 */
			virtual void OnError();

			/**
			 *
			 * 套接字生成成功
			 *
			 */
			virtual void OnSocket();

			/**
			 *
			 * 套接字生成失败
			 *
			 */
			virtual void OnSocketFailed();

			/**
			 *
			 * 绑定成功
			 *
			 */
			virtual void OnBind();

			/**
			 *
			 * 绑定失败
			 *
			 */
			virtual void OnBindFailed();

			/**
			 *
			 * 监听成功
			 *
			 */
			virtual void OnListen();

			/**
			 *
			 * 监听失败
			 *
			 */
			virtual void OnListenFailed();

			/**
			 *
			 * 接受连接成功
			 *
			 * @return 会话
			 *
			 */
			virtual tinyToolkit::net::ITCPSession * OnAccept();

			/**
			 *
			 * 接受连接失败
			 *
			 */
			virtual void OnAcceptFailed();

			/**
			 *
			 * 断开连接
			 *
			 */
			virtual void OnDisconnect();
		};

		class TINY_TOOLKIT_API IUDPServer : public IServer
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			~IUDPServer() override = default;

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
			bool Launch(std::string host, uint16_t port, std::size_t cache, Poller * poller = nullptr);

			/**
			 *
			 * 事件错误
			 *
			 */
			virtual void OnError();

			/**
			 *
			 * 套接字生成成功
			 *
			 */
			virtual void OnSocket();

			/**
			 *
			 * 套接字生成失败
			 *
			 */
			virtual void OnSocketFailed();

			/**
			 *
			 * 绑定成功
			 *
			 */
			virtual void OnBind();

			/**
			 *
			 * 绑定失败
			 *
			 */
			virtual void OnBindFailed();

			/**
			 *
			 * 接受连接成功
			 *
			 * @return 会话
			 *
			 */
			virtual tinyToolkit::net::IUDPSession * OnAccept();

			/**
			 *
			 * 接受连接失败
			 *
			 */
			virtual void OnAcceptFailed();

			/**
			 *
			 * 断开连接
			 *
			 */
			virtual void OnDisconnect();
		};
	}
}


#endif // __TINY_TOOLKIT__NET__SERVER__H__
