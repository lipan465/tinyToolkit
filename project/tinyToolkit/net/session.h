#ifndef __TINY_TOOLKIT__NET__SESSION__H__
#define __TINY_TOOLKIT__NET__SESSION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯会话
 *
 */


#include "pipe.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ITCPSession
	{
		friend class NetManager;
		friend class TCPServerPipe;
		friend class TCPSessionPipe;

	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~ITCPSession() = default;

		/**
		 *
		 * 连接成功触发回调函数
		 *
		 */
		virtual void OnConnect() = 0;

		/**
		 *
		 * 断开连接触发回调函数
		 *
		 */
		virtual void OnDisconnect() = 0;

		/**
		 *
		 * 连接失败触发回调函数
		 *
		 */
		virtual void OnConnectFailed() = 0;

		/**
		 *
		 * 接收数据触发回调函数
		 *
		 * @param data 接收的数据缓冲区指针
		 * @param size 接收的数据缓冲区长度
		 *
		 * @return 使用的字节数
		 *
		 */
		virtual std::size_t OnReceive(const char * data, std::size_t size) = 0;

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
		 * @param data 待发送数据指针
		 * @param size 待发送数据长度
		 *
		 */
		void Send(const void * data, std::size_t size);

		/**
		 *
		 * 启动
		 *
		 * @param remoteHost 远端地址
		 * @param remotePort 远端端口
		 * @param cacheSize 缓存大小
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(std::string remoteHost, uint16_t remotePort, std::size_t cacheSize);

		/**
		 *
		 * 主机端口
		 *
		 * @return 主机端口
		 *
		 */
		uint16_t LocalPort() const;

		/**
		 *
		 * 远端端口
		 *
		 * @return 远端端口
		 *
		 */
		uint16_t RemotePort() const;

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
		 * 主机地址
		 *
		 * @return 主机地址
		 *
		 */
		const std::string & LocalHost() const;

		/**
		 *
		 * 远端地址
		 *
		 * @return 远端地址
		 *
		 */
		const std::string & RemoteHost() const;

	private:
		uint16_t _localPort{ 0 };
		uint16_t _remotePort{ 0 };

		std::string _localHost{ };
		std::string _remoteHost{ };

		std::size_t _cacheSize{ 0 };

		std::shared_ptr<ITCPPipe> _pipe{ };
	};

	class TINY_TOOLKIT_API IUDPSession
	{
		friend class NetManager;
		friend class UDPSessionPipe;

	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~IUDPSession() = default;

		/**
		 *
		 * 连接成功触发回调函数
		 *
		 */
		virtual void OnConnect() = 0;

		/**
		 *
		 * 断开连接触发回调函数
		 *
		 */
		virtual void OnDisconnect() = 0;

		/**
		 *
		 * 连接失败触发回调函数
		 *
		 */
		virtual void OnConnectFailed() = 0;

		/**
		 *
		 * 接收数据触发回调函数
		 *
		 * @param data 接收的数据缓冲区指针
		 * @param size 接收的数据缓冲区长度
		 *
		 * @return 使用的字节数
		 *
		 */
		virtual std::size_t OnReceive(const char * data, std::size_t size) = 0;

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
		 * @param data 待发送数据指针
		 * @param size 待发送数据长度
		 *
		 */
		void Send(const void * data, std::size_t size);

		/**
		 *
		 * 启动
		 *
		 * @param localHost 主机地址
		 * @param localPort 主机端口
		 * @param remoteHost 远端地址
		 * @param remotePort 远端端口
		 * @param cacheSize 缓存大小
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(std::string localHost, uint16_t localPort, std::string remoteHost, uint16_t remotePort, std::size_t cacheSize);

		/**
		 *
		 * 主机端口
		 *
		 * @return 主机端口
		 *
		 */
		uint16_t LocalPort() const;

		/**
		 *
		 * 远端端口
		 *
		 * @return 远端端口
		 *
		 */
		uint16_t RemotePort() const;

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
		 * 主机地址
		 *
		 * @return 主机地址
		 *
		 */
		const std::string & LocalHost() const;

		/**
		 *
		 * 远端地址
		 *
		 * @return 远端地址
		 *
		 */
		const std::string & RemoteHost() const;

	private:
		uint16_t _localPort{ 0 };
		uint16_t _remotePort{ 0 };

		std::string _localHost{ };
		std::string _remoteHost{ };

		std::size_t _cacheSize{ 0 };

		std::shared_ptr<IUDPPipe> _pipe{ };
	};

	class TINY_TOOLKIT_API IRUDPSession
	{
		friend class NetManager;
		friend class RUDPSessionPipe;

	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~IRUDPSession() = default;

		/**
		 *
		 * 连接成功触发回调函数
		 *
		 */
		virtual void OnConnect() = 0;

		/**
		 *
		 * 断开连接触发回调函数
		 *
		 */
		virtual void OnDisconnect() = 0;

		/**
		 *
		 * 连接失败触发回调函数
		 *
		 */
		virtual void OnConnectFailed() = 0;

		/**
		 *
		 * 接收数据触发回调函数
		 *
		 * @param data 接收的数据缓冲区指针
		 * @param size 接收的数据缓冲区长度
		 *
		 * @return 使用的字节数
		 *
		 */
		virtual std::size_t OnReceive(const char * data, std::size_t size) = 0;

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
		 * @param data 待发送数据指针
		 * @param size 待发送数据长度
		 *
		 */
		void Send(const void * data, std::size_t size);

		/**
		 *
		 * 启动
		 *
		 * @param localHost 主机地址
		 * @param localPort 主机端口
		 * @param remoteHost 远端地址
		 * @param remotePort 远端端口
		 * @param cacheSize 缓存大小
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(std::string localHost, uint16_t localPort, std::string remoteHost, uint16_t remotePort, std::size_t cacheSize);

		/**
		 *
		 * 主机端口
		 *
		 * @return 主机端口
		 *
		 */
		uint16_t LocalPort() const;

		/**
		 *
		 * 远端端口
		 *
		 * @return 远端端口
		 *
		 */
		uint16_t RemotePort() const;

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
		 * 主机地址
		 *
		 * @return 主机地址
		 *
		 */
		const std::string & LocalHost() const;

		/**
		 *
		 * 远端地址
		 *
		 * @return 远端地址
		 *
		 */
		const std::string & RemoteHost() const;

	private:
		uint16_t _localPort{ 0 };
		uint16_t _remotePort{ 0 };

		std::string _localHost{ };
		std::string _remoteHost{ };

		std::size_t _cacheSize{ 0 };

		std::shared_ptr<IRUDPPipe> _pipe{ };
	};
}


#endif // __TINY_TOOLKIT__NET__SESSION__H__
