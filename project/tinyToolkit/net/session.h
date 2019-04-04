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
		 * @param data 接收的数据缓冲区
		 * @param size 接收的数据缓冲区长度
		 *
		 * @return 使用的字节数
		 *
		 */
		virtual std::size_t OnReceive(const char * data, std::size_t size) = 0;

		/**
		 *
		 * 启动
		 *
		 * @param host 主机地址
		 * @param port 主机端口
		 * @param sSize 发送缓冲区大小
		 * @param rSize 接受缓冲区大小
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(const std::string & host, uint16_t port, std::size_t sSize, std::size_t rSize);

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
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 * @param cache 缓冲发送
		 *
		 */
		void Send(const void * value, std::size_t size, bool cache = false);

	public:
		uint16_t _localPort{ 0 };
		uint16_t _remotePort{ 0 };

		std::size_t _sSize{ 0 };
		std::size_t _rSize{ 0 };

		std::string _localHost{ };
		std::string _remoteHost{ };

		std::shared_ptr<ITCPPipe> _pipe{ };
	};

	class TINY_TOOLKIT_API IUDPSession
	{
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
		 * @param ip 远端地址
		 * @param port 远端端口
		 * @param data 接收的数据缓冲区
		 * @param size 接收的数据缓冲区长度
		 *
		 * @return 使用的字节数
		 *
		 */
		virtual std::size_t OnReceive(const char * ip, uint16_t port, const char * data, std::size_t size) = 0;

		/**
		 *
		 * 启动
		 *
		 * @param host 主机地址
		 * @param port 主机端口
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(const std::string & host, uint16_t port);

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
		 * @param ip 远端地址
		 * @param port 远端端口
		 * @param data 待发送数据
		 * @param size 待发送数据长度
		 *
		 */
		void Send(const char * ip, uint16_t port, const void * data, std::size_t size);

	public:
		uint16_t _localPort{ 0 };
		uint16_t _remotePort{ 0 };

		std::size_t _sSize{ 0 };
		std::size_t _rSize{ 0 };

		std::string _localHost{ };
		std::string _remoteHost{ };

		std::shared_ptr<IUDPPipe> _pipe{ };
	};
}


#endif // __TINY_TOOLKIT__NET__SESSION__H__
