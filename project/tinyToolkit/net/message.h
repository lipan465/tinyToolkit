#ifndef __TINY_TOOLKIT__NET__MESSAGE__H__
#define __TINY_TOOLKIT__NET__MESSAGE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 消息
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetMessage
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param buf 待发送数据缓冲区指针
		 * @param len 待发送数据缓冲区长度
		 *
		 */
		NetMessage(const void * buf, std::size_t len);

		/**
		 *
		 * 析构函数
		 *
		 */
		~NetMessage();

	public:
		char * buffer{ nullptr };

		std::size_t offset{ 0 };
		std::size_t length{ 0 };
	};
}


#endif // __TINY_TOOLKIT__NET__MESSAGE__H__
