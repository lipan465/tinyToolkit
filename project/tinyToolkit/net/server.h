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
	class TINY_TOOLKIT_API INetServer
	{
		friend class TCPServerPipe;
		friend class NetEventMonitor;

	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~INetServer() = default;

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
		 * 监听连接触发回调函数
		 *
		 */
		virtual void OnListen();

		/**
		 *
		 * 监听失败触发回调函数
		 *
		 */
		virtual void OnListenFailed();

		/**
		 *
		 * 连接失败触发回调函数
		 *
		 */
		virtual void OnAcceptFailed();

		/**
		 *
		 * 会话连接触发回调函数
		 *
		 * @return 会话
		 *
		 */
		virtual tinyToolkit::ITCPSession * OnAccept();

		/**
		 *
		 * 关闭会话
		 *
		 */
		void Close();

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

	class TINY_TOOLKIT_API ITCPServer : public INetServer
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
		 * @param localHost 本地地址
		 * @param localPort 本地端口
		 * @param cacheSize 缓存大小
		 * @param eventMonitor 事件监控
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(std::string localHost, uint16_t localPort, std::size_t cacheSize, NetEventMonitor * eventMonitor = nullptr);
	};
}


#endif // __TINY_TOOLKIT__NET__SERVER__H__
