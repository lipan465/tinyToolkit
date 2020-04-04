#ifndef __TINY_TOOLKIT__NET__SESSION__H__
#define __TINY_TOOLKIT__NET__SESSION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 会话
 *
 */


#include "channel.h"
#include "endpoint.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <functional>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <functional>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <functional>
#
#endif


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API ISession
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
			explicit ISession(Poller * poller = nullptr);

			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~ISession() = default;

			/**
			 *
			 * 关闭会话
			 *
			 */
			void Close();

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
			bool Send(const void * buffer, std::size_t length);

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
			 * 剩余消息个数
			 *
			 * @return 剩余消息个数
			 *
			 */
			std::size_t RemainMessageCount() const;

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

		class TINY_TOOLKIT_API TCPSession : public ISession
		{
			friend class Poller;
			friend class UDPServerChannel;
			friend class TCPServerChannel;
			friend class UDPSessionChannel;
			friend class TCPSessionChannel;

			using ISession::ISession;

		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			~TCPSession() override = default;

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
			 * 断开连接
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnDisconnect(std::function<void()> callback);

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
			 * 发送数据
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnSend(std::function<void(bool)> callback);

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
			 * 会话连接
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnConnect(std::function<void(bool)> callback);

			/**
			 *
			 * 接收数据
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnReceive(std::function<std::size_t(bool, const char *, std::size_t)> callback);

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
			bool Launch(std::string host, uint16_t port, std::size_t cache);

		private:
			std::function<void()> _onError{ };
			std::function<void()> _onDisconnect{ };

			std::function<void(bool)> _onBind{ };
			std::function<void(bool)> _onSend{ };
			std::function<void(bool)> _onSocket{ };
			std::function<void(bool)> _onConnect{ };

			std::function<std::size_t(bool, const char *, std::size_t)> _onReceive{ };
		};

		class TINY_TOOLKIT_API UDPSession : public ISession
		{
			friend class Poller;
			friend class UDPServerChannel;
			friend class TCPServerChannel;
			friend class UDPSessionChannel;
			friend class TCPSessionChannel;

			using ISession::ISession;

		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			~UDPSession() override = default;

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
			 * 断开连接
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnDisconnect(std::function<void()> callback);

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
			 * 发送数据
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnSend(std::function<void(bool)> callback);

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
			 * 会话连接
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnConnect(std::function<void(bool)> callback);

			/**
			 *
			 * 接收数据
			 *
			 * @param callback 回调函数
			 *
			 */
			void OnReceive(std::function<std::size_t(bool, const char *, std::size_t)> callback);

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
			bool Launch(std::string host, uint16_t port, std::size_t cache);

		private:
			std::function<void()> _onError{ };
			std::function<void()> _onDisconnect{ };

			std::function<void(bool)> _onBind{ };
			std::function<void(bool)> _onSend{ };
			std::function<void(bool)> _onSocket{ };
			std::function<void(bool)> _onConnect{ };

			std::function<std::size_t(bool, const char *, std::size_t)> _onReceive{ };
		};
	}
}


#endif // __TINY_TOOLKIT__NET__SESSION__H__
