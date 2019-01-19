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
		virtual void OnSessionError(const tinyToolkit::ITCPSession * session) = 0;

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
		void Close()
		{
			if (_pipe)
			{
				_pipe->Close();

				delete _pipe;

				_pipe = nullptr;
			}
		}

	public:
		uint16_t _port{ 0 };

		std::string _host{ 0 };

		ITCPPipe * _pipe{ nullptr };
	};
}


#endif // __TINY_TOOLKIT__NET__SERVER__H__
