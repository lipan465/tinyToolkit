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
		 * 会话错误触发回调函数
		 *
		 */
		virtual void OnSessionError(tinyToolkit::ITCPSession * session) = 0;

		/**
		 *
		 * 会话连接触发回调函数
		 *
		 * @return 会话
		 *
		 */
		virtual tinyToolkit::ITCPSession * OnSessionConnect() = 0;

		/**
		 *
		 * 启动
		 *
		 * @param localHost 主机地址
		 * @param localPort 主机端口
		 * @param cacheSize 缓存大小
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(std::string localHost, uint16_t localPort, std::size_t cacheSize);

		/**
		 *
		 * 关闭会话
		 *
		 */
		void Close();

	public:
		uint16_t _localPort{ 0 };
		uint16_t _remotePort{ 0 };

		std::string _localHost{ };
		std::string _remoteHost{ };

		std::size_t _cacheSize{ 0 };

		std::shared_ptr<ITCPPipe> _pipe{ };
	};
}


#endif // __TINY_TOOLKIT__NET__SERVER__H__
