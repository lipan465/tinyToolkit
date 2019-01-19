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
	class TINY_TOOLKIT_API IPipe
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~IPipe() = default;

		/**
		 *
		 * 关闭会话
		 *
		 */
		virtual void Close() = 0;
	};

	class TINY_TOOLKIT_API ITCPPipe : public IPipe
	{
	public:
		/**
		 *
		 * 发送数据
		 *
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 *
		 */
		virtual void Send(const void * value, std::size_t size) = 0;
	};

	class TINY_TOOLKIT_API IUDPPipe : public IPipe
	{
	public:
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
		virtual void Send(const char * host, uint16_t port, const void * value, std::size_t size) = 0;
	};
}


#endif // __TINY_TOOLKIT__NET__PIPE__H__
