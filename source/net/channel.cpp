/**
 *
 *  作者: hm
 *
 *  说明: 通道
 *
 */


#include "channel.h"


namespace tinyToolkit
{
	namespace net
	{
		/**
		 *
		 * 关闭会话
		 *
		 */
		void IChannel::Close()
		{

		}

		/**
		 *
		 * 接受连接
		 *
		 * @return 是否连接成功
		 *
		 */
		bool IChannel::Accept()
		{
			return false;
		}

		/**
		 *
		 * 接收数据
		 *
		 * @return 是否接收成功
		 *
		 */
		bool IChannel::Receive()
		{
			return false;
		}

		/**
		 *
		 * 发送数据
		 *
		 * @param buffer 内容
		 * @param length 长度
		 *
		 * @return 是否发送成功
		 *
		 */
		bool IChannel::Send(const void * buffer, std::size_t length)
		{
			(void)buffer;
			(void)length;

			return false;
		}

		/**
		 *
		 * 追加数据
		 *
		 * @param buffer 内容
		 * @param length 长度
		 *
		 * @return 是否追加成功
		 *
		 */
		bool IChannel::Append(const void * buffer, std::size_t length)
		{
			(void)buffer;
			(void)length;

			return false;
		}

		/**
		 *
		 * 套接字
		 *
		 * @return 套接字
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE IChannel::Socket() const
		{
			return TINY_TOOLKIT_SOCKET_INVALID;
		}
	}
}
