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


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API ISession
		{
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
			 * 设置接收缓存大小
			 *
			 * @param size 缓存大小
			 *
			 */
			void SetReceiveCacheSize(std::size_t size);

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

			std::size_t _receiveCacheSize{ 0 };

			std::shared_ptr<IChannel> _channel{ };
		};

		class TINY_TOOLKIT_API ITCPSession : public ISession
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
			~ITCPSession() override = default;

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
			bool Connect(std::string host, uint16_t port);

		protected:
			/**
			 *
			 * 事件错误
			 *
			 */
			virtual void OnError();

			/**
			 *
			 * 会话连接
			 *
			 */
			virtual void OnConnect();

			/**
			 *
			 * 断开连接
			 *
			 */
			virtual void OnDisconnect();

			/**
			 *
			 * 发送数据
			 *
			 */
			virtual void OnSend();

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
			virtual std::size_t OnReceive(const char * buffer, std::size_t length);
		};

		class TINY_TOOLKIT_API IUDPSession : public ISession
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
			~IUDPSession() override = default;

			/**
			 *
			 * 连接监听
			 *
			 * @param host 目标地址
			 * @param port 目标端口
			 *
			 * @return 是否连接成功
			 *
			 */
			bool Connect(std::string host, uint16_t port);

		protected:
			/**
			 *
			 * 事件错误
			 *
			 */
			virtual void OnError();

			/**
			 *
			 * 会话连接
			 *
			 */
			virtual void OnConnect();

			/**
			 *
			 * 断开连接
			 *
			 */
			virtual void OnDisconnect();

			/**
			 *
			 * 发送数据
			 *
			 */
			virtual void OnSend();

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
			virtual std::size_t OnReceive(const char * buffer, std::size_t length);
		};
	}
}


#endif // __TINY_TOOLKIT__NET__SESSION__H__
