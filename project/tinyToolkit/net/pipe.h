#ifndef __TINY_TOOLKIT__NET__PIPE__H__
#define __TINY_TOOLKIT__NET__PIPE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 管道
 *
 */


#include "socket.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API INetPipe
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~INetPipe() = default;

		/**
		 *
		 * 关闭会话
		 *
		 */
		virtual void Close() = 0;

		/**
		 *
		 * 接受连接
		 *
		 * @return 是否连接成功
		 *
		 */
		virtual bool Accept() = 0;

		/**
		 *
		 * 接收数据
		 *
		 * @return 是否接收成功
		 *
		 */
		virtual bool Receive() = 0;

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
		virtual bool Send(const char * buffer, std::size_t length) = 0;

		/**
		 *
		 * 追加数据
		 *
		 * @param buffer 待追加数据缓冲区指针
		 * @param length 待追加数据缓冲区长度
		 *
		 * @return 是否追加成功
		 *
		 */
		virtual bool Append(const void * buffer, std::size_t length) = 0;

		/**
		 *
		 * 剩余消息个数
		 *
		 * @return 剩余消息个数
		 *
		 */
		virtual std::size_t RemainMessageCount() = 0;

		/**
		 *
		 * 句柄
		 *
		 * @return 句柄
		 *
		 */
		virtual TINY_TOOLKIT_SOCKET_TYPE SocketHandle() = 0;
	};
}


#endif // __TINY_TOOLKIT__NET__PIPE__H__
