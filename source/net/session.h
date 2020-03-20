#ifndef __TINY_TOOLKIT__NET__SESSION__H__
#define __TINY_TOOLKIT__NET__SESSION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 会话
 *
 */


#include "pipe.h"
#include "endpoint.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <memory>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#
#
#endif


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API ISession
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

		private:
			mutable Endpoint _peerEndpoint{ };
			mutable Endpoint _localEndpoint{ };

			std::size_t _cacheSize{ 0 };

			std::shared_ptr<IPipe> _pipe{ };
		};

		class TINY_TOOLKIT_API ITCPSession : public ISession
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			~ITCPSession() override = default;

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
			 * 连接成功
			 *
			 */
			virtual void OnConnect();

			/**
			 *
			 * 连接失败
			 *
			 */
			virtual void OnConnectFailed();

			/**
			 *
			 * 发送成功
			 *
			 */
			virtual void OnSend();

			/**
			 *
			 * 发送失败
			 *
			 */
			virtual void OnSendFailed();

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
			virtual std::size_t OnReceive(const char * buffer, std::size_t length);

			/**
			 *
			 * 接收失败
			 *
			 */
			virtual void OnReceiveFailed();

			/**
			 *
			 * 断开连接
			 *
			 */
			virtual void OnDisconnect();
		};

		class TINY_TOOLKIT_API IUDPSession : public ISession
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			~IUDPSession() override = default;

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
			 * 连接成功
			 *
			 */
			virtual void OnConnect();

			/**
			 *
			 * 连接失败
			 *
			 */
			virtual void OnConnectFailed();

			/**
			 *
			 * 发送成功
			 *
			 */
			virtual void OnSend();

			/**
			 *
			 * 发送失败
			 *
			 */
			virtual void OnSendFailed();

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
			virtual std::size_t OnReceive(const char * buffer, std::size_t length);

			/**
			 *
			 * 接收失败
			 *
			 */
			virtual void OnReceiveFailed();

			/**
			 *
			 * 断开连接
			 *
			 */
			virtual void OnDisconnect();
		};
	}
}


#endif // __TINY_TOOLKIT__NET__SESSION__H__
