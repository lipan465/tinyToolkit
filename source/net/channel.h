#ifndef __TINY_TOOLKIT__NET__CHANNEL__H__
#define __TINY_TOOLKIT__NET__CHANNEL__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通道
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <cstring>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <cstring>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstdint>
#  include <cstring>
#
#endif


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API IChannel
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~IChannel() = default;

			/**
			 *
			 * 关闭会话
			 *
			 */
			virtual void Close();

			/**
			 *
			 * 接受连接
			 *
			 * @return 是否连接成功
			 *
			 */
			virtual bool Accept();

			/**
			 *
			 * 接收数据
			 *
			 * @return 是否接收成功
			 *
			 */
			virtual bool Receive();

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
			virtual bool Send(const void * buffer, std::size_t length);

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
			virtual bool Append(const void * buffer, std::size_t length);

			/**
			 *
			 * 缓存大小
			 *
			 * @return 缓存大小
			 *
			 */
			virtual std::size_t CacheSize() const;

			/**
			 *
			 * 剩余消息个数
			 *
			 * @return 剩余消息个数
			 *
			 */
			virtual std::size_t RemainMessageCount() const;

			/**
			 *
			 * 套接字
			 *
			 * @return 套接字
			 *
			 */
			virtual TINY_TOOLKIT_SOCKET_TYPE Socket() const;
		};
	}
}


#endif // __TINY_TOOLKIT__NET__CHANNEL__H__
