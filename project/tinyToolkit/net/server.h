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
		friend class NetManager;
		friend class TCPServerPipe;

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
		 * @param cacheSize 缓存大小
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(std::string localHost, uint16_t localPort, std::size_t cacheSize);

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
}


#endif // __TINY_TOOLKIT__NET__SERVER__H__
