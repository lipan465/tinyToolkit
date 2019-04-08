#ifndef __TINY_TOOLKIT__NET__SERVER__H__
#define __TINY_TOOLKIT__NET__SERVER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯服务器
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
		 * @param sSize 发送缓冲区大小
		 * @param rSize 接受缓冲区大小
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(const char * host, uint16_t port, std::size_t sSize, std::size_t rSize);

		/**
		 *
		 * 关闭会话
		 *
		 */
		void Close();

	public:
		uint16_t _port{ 0 };

		std::string _host{ };

		std::size_t _sSize{ 0 };
		std::size_t _rSize{ 0 };

		std::shared_ptr<ITCPPipe> _pipe{ };
	};
}


#endif // __TINY_TOOLKIT__NET__SERVER__H__
