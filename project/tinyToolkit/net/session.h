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
#include "address.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API INetSession
	{
		friend class TCPServerPipe;
		friend class TCPSessionPipe;
		friend class NetEventMonitor;

	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~INetSession() = default;

		/**
		 *
		 * 缓存堵塞触发回调函数
		 *
		 * @return 是否关闭连接
		 *
		 */
		virtual bool OnCacheFull();

		/**
		 *
		 * 事件错误触发回调函数
		 *
		 */
		virtual void OnEventError();

		/**
		 *
		 * 断开连接触发回调函数
		 *
		 */
		virtual void OnDisconnect();

		/**
		 *
		 * 发送成功触发回调函数
		 *
		 */
		virtual void OnSend();

		/**
		 *
		 * 发送失败触发回调函数
		 *
		 */
		virtual void OnSendFailed();

		/**
		 *
		 * 监听连接触发回调函数
		 *
		 */
		virtual void OnBind();

		/**
		 *
		 * 监听失败触发回调函数
		 *
		 */
		virtual void OnBindFailed();

		/**
		 *
		 * 套接字生成触发回调函数
		 *
		 */
		virtual void OnSocket();

		/**
		 *
		 * 套接字错误触发回调函数
		 *
		 */
		virtual void OnSocketFailed();

		/**
		 *
		 * 连接成功触发回调函数
		 *
		 */
		virtual void OnConnect();

		/**
		 *
		 * 连接失败触发回调函数
		 *
		 */
		virtual void OnConnectFailed();

		/**
		 *
		 * 接收失败触发回调函数
		 *
		 */
		virtual void OnReceiveFailed();

		/**
		 *
		 * 接收数据触发回调函数
		 *
		 * @param buffer 数据缓冲区指针
		 * @param length 数据缓冲区长度
		 *
		 * @return 使用的字节数
		 *
		 */
		virtual std::size_t OnReceive(const char * buffer, std::size_t length);

		/**
		 *
		 * 关闭会话
		 *
		 */
		void Close();

		/**
		 *
		 * 发送数据
		 *
		 * @param buffer 待发送数据缓冲区指针
		 * @param length 待发送数据缓冲区长度
		 *
		 * @return 是否发送成功
		 *
		 */
		bool Send(const void * buffer, std::size_t length);

		/**
		 *
		 * 剩余消息个数
		 *
		 * @return 剩余消息个数
		 *
		 */
		std::size_t RemainMessageCount();

		/**
		 *
		 * 目标地址
		 *
		 * @return 目标地址
		 *
		 */
		const NetAddress & PeerAddress();

		/**
		 *
		 * 本地地址
		 *
		 * @return 本地地址
		 *
		 */
		const NetAddress & LocalAddress();

	protected:
		NetAddress _peerAddress{ };
		NetAddress _localAddress{ };

		std::size_t _cacheSize{ 0 };

		std::shared_ptr<INetPipe> _pipe{ };
	};

	class TINY_TOOLKIT_API ITCPSession : public INetSession
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
		 * @param peerHost 目标地址
		 * @param peerPort 目标端口
		 * @param cacheSize 缓存大小
		 * @param eventMonitor 事件监控
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(std::string peerHost, uint16_t peerPort, std::size_t cacheSize, NetEventMonitor * eventMonitor = nullptr);
	};
}


#endif // __TINY_TOOLKIT__NET__SESSION__H__
