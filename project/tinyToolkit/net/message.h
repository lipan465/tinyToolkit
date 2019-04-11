#ifndef __TINY_TOOLKIT__NET__MESSAGE__H__
#define __TINY_TOOLKIT__NET__MESSAGE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯信息
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
		 * @param data 待发送数据指针
		 * @param size 待发送数据长度
		 *
		 */
		NetMessage(const void * data, std::size_t size);

		/**
		 *
		 * 拷贝构造函数
		 *
		 * @param rhs 右值对象
		 *
		 */
		NetMessage(NetMessage && rhs) noexcept;

		/**
		 *
		 * 析构函数
		 *
		 */
		~NetMessage();

	public:
		char * _data{ nullptr };

		std::size_t _size{ 0 };
	};
}


#endif // __TINY_TOOLKIT__NET__MESSAGE__H__
