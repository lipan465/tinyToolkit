#ifndef __TINY_TOOLKIT__NET__SERVER__H__
#define __TINY_TOOLKIT__NET__SERVER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 服务器
 *
 */


#include "session.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ITCPServer
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~ITCPServer() = default;

		/**
		 *
		 * 新连接触发回调函数
		 *
		 * @param host 主机地址
		 * @param port 主机端口
		 *
		 * @return 会话
		 *
		 */
		virtual tinyToolkit::ITCPSession * OnNewConnect(const std::string & host, uint16_t port) = 0;

		/**
		 *
		 * 会话错误触发回调函数
		 *
		 */
		virtual void OnSessionError(tinyToolkit::ITCPSession * session) = 0;

		/**
		 *
		 * 错误触发回调函数
		 *
		 */
		virtual void OnError() = 0;

		/**
		 *
		 * 断开连接触发回调函数
		 *
		 */
		virtual void OnRelease() = 0;

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

	public:
		uint16_t _port{ 0 };

		std::string _host{ 0 };

		ITCPPipe * _pipe{ nullptr };
	};

	class TINY_TOOLKIT_API IUDPServer
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~IUDPServer() = default;

		/**
		 *
		 * 接收数据触发回调函数
		 *
		 * @param host 接收的主机地址
		 * @param port 接收的主机端口
		 * @param data 接收的数据缓冲区
		 * @param size 接收的数据缓冲区长度
		 *
		 */
		virtual void OnReceive(const char * host, uint16_t port, const char * data, std::size_t size) = 0;

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
		 * @param host 待发送主机地址
		 * @param port 待发送主机端口
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 *
		 */
		void Send(const char * host, uint16_t port, const void * value, std::size_t size);

	public:
		uint16_t _port{ 0 };

		std::string _host{ 0 };

		std::shared_ptr<IUDPPipe> _pipe{ };
	};
}


#endif // __TINY_TOOLKIT__NET__SERVER__H__
