#ifndef __TINY_TOOLKIT__NET__PIPE__H__
#define __TINY_TOOLKIT__NET__PIPE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯管道
 *
 */


#include "../common/common.h"


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
	};

	class TINY_TOOLKIT_API ITCPPipe : public INetPipe
	{
	public:
		/**
		 *
		 * 发送数据
		 *
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 * @param cache 缓冲发送
		 *
		 */
		virtual void Send(const void * value, std::size_t size, bool cache) = 0;

		/**
		 *
		 * 异步发送
		 *
		 * @return 是否处理成功
		 *
		 */
		virtual bool AsyncSend() = 0;

		/**
		 *
		 * 异步连接
		 *
		 * @return 是否处理成功
		 *
		 */
		virtual bool AsyncAccept() = 0;

		/**
		 *
		 * 异步接收
		 *
		 * @return 是否处理成功
		 *
		 */
		virtual bool AsyncReceive() = 0;
	};

	class TINY_TOOLKIT_API IUDPPipe : public INetPipe
	{
	public:
		/**
		 *
		 * 发送数据
		 *
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 * @param cache 缓冲发送
		 *
		 */
		virtual void Send(const void * value, std::size_t size, bool cache) = 0;

		/**
		 *
		 * 异步发送
		 *
		 * @return 是否处理成功
		 *
		 */
		virtual bool AsyncSend() = 0;

		/**
		 *
		 * 异步连接
		 *
		 * @return 是否处理成功
		 *
		 */
		virtual bool AsyncAccept() = 0;

		/**
		 *
		 * 异步接收
		 *
		 * @return 是否处理成功
		 *
		 */
		virtual bool AsyncReceive() = 0;
	};
}


#endif // __TINY_TOOLKIT__NET__PIPE__H__
