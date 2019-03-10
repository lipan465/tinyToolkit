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
		 * @param delay 延迟发送
		 *
		 */
		virtual void Send(const void * value, std::size_t size, bool delay) = 0;
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
		 * @param delay 延迟发送
		 *
		 */
		virtual void Send(const void * value, std::size_t size, bool delay) = 0;
	};
}


#endif // __TINY_TOOLKIT__NET__PIPE__H__
