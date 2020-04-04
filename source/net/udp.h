#ifndef __TINY_TOOLKIT__NET__UDP__H__
#define __TINY_TOOLKIT__NET__UDP__H__


/**
 *
 *  作者: hm
 *
 *  说明: udp
 *
 */


#include "cache.h"
#include "poller.h"
#include "channel.h"
#include "message.h"
#include "adaptor.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <queue>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <queue>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <queue>
#
#endif


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API UDPSessionChannel : public IChannel, public ICompleter
		{
			friend class Poller;
			friend class UDPServerChannel;

		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param session 会话
			 * @param adaptor 适配器
			 *
			 */
			UDPSessionChannel(UDPSession * session, std::shared_ptr<UDPAdaptor> adaptor);

			/**
			 *
			 * 析构函数
			 *
			 */
			~UDPSessionChannel() override;

			/**
			 *
			 * 关闭会话
			 *
			 */
			void Close() override;

			/**
			 *
			 * 处理流程
			 *
			 * @param netContext 网络上下文
			 * @param sysContext 系统上下文
			 *
			 */
			void Process(Context * netContext, void * sysContext) override;

			/**
			 *
			 * 接收数据
			 *
			 * @return 是否接收成功
			 *
			 */
			bool Receive() override;

			/**
			 *
			 * 发送数据
			 *
			 * @param buffer 内容
			 * @param length 长度
			 *
			 * @return 是否发送成功
			 *
			 */
			bool Send(const void * buffer, std::size_t length) override;

			/**
			 *
			 * 追加数据
			 *
			 * @param buffer 内容
			 * @param length 长度
			 *
			 * @return 是否追加成功
			 *
			 */
			bool Append(const void * buffer, std::size_t length) override;

			/**
			 *
			 * 缓存大小
			 *
			 * @return 缓存大小
			 *
			 */
			std::size_t CacheSize() const override;

			/**
			 *
			 * 剩余消息个数
			 *
			 * @return 剩余消息个数
			 *
			 */
			std::size_t RemainMessageCount() const override;

			/**
			 *
			 * 套接字
			 *
			 * @return 套接字
			 *
			 */
			TINY_TOOLKIT_SOCKET_TYPE Socket() const override;

		private:
			/**
			 *
			 * 交互处理
			 *
			 * @param netContext 网络上下文
			 * @param sysContext 系统上下文
			 *
			 */
			void DoIO(Context * netContext, void * sysContext);

			/**
			 *
			 * 发送处理
			 *
			 * @param netContext 网络上下文
			 * @param sysContext 系统上下文
			 *
			 */
			void DoSend(Context * netContext, void * sysContext);

			/**
			 *
			 * 接收处理
			 *
			 * @param netContext 网络上下文
			 * @param sysContext 系统上下文
			 *
			 */
			void DoReceive(Context * netContext, void * sysContext);

			/**
			 *
			 * 连接处理
			 *
			 * @param netContext 网络上下文
			 * @param sysContext 系统上下文
			 *
			 */
			void DoConnect(Context * netContext, void * sysContext);

		private:
			bool _isSend{ false };
			bool _isConnect{ false };

			Cache _cache;

			std::mutex _mutex{ };

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			Context _sendContext{ };
			Context _receiveContext{ };

		#else

			Context _ioContext{ };

		#endif

			UDPSession * _session{ nullptr };

			std::shared_ptr<UDPAdaptor> _adaptor{ };

			std::queue<std::shared_ptr<Message>> _messageQueue{ };
		};

		class TINY_TOOLKIT_API UDPServerChannel : public IChannel, public ICompleter
		{
			friend class Poller;

		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param server 服务
			 * @param adaptor 适配器
			 *
			 */
			UDPServerChannel(UDPServer * server, std::shared_ptr<UDPAdaptor> adaptor);

			/**
			 *
			 * 析构函数
			 *
			 */
			~UDPServerChannel() override;

			/**
			 *
			 * 关闭会话
			 *
			 */
			void Close() override;

			/**
			 *
			 * 处理流程
			 *
			 * @param netContext 网络上下文
			 * @param sysContext 系统上下文
			 *
			 */
			void Process(Context * netContext, void * sysContext) override;

			/**
			 *
			 * 接受连接
			 *
			 * @return 是否连接成功
			 *
			 */
			bool Accept() override;

			/**
			 *
			 * 缓存大小
			 *
			 * @return 缓存大小
			 *
			 */
			std::size_t CacheSize() const override;

			/**
			 *
			 * 套接字
			 *
			 * @return 套接字
			 *
			 */
			TINY_TOOLKIT_SOCKET_TYPE Socket() const override;

		private:
			/**
			 *
			 * 连接处理
			 *
			 * @param netContext 网络上下文
			 * @param sysContext 系统上下文
			 *
			 */
			void DoAccept(Context * netContext, void * sysContext);

		private:
			bool _isListen{ true };

			Context _acceptContext{ };

			UDPServer * _server{ nullptr };

			std::shared_ptr<UDPAdaptor> _adaptor{ };
		};
	}
}


#endif // __TINY_TOOLKIT__NET__UDP__H__
