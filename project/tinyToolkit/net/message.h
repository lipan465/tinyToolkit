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
		 * @param ip 远端地址
		 * @param port 远端端口
		 * @param data 待发送数据
		 * @param size 待发送数据长度
		 *
		 */
		NetMessage(const char * ip, uint16_t port, const void * data, std::size_t size);

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

		uint32_t _ip{ 0 };

		uint16_t _port{ 0 };

		std::size_t _size{ 0 };
	};
}


#endif // __TINY_TOOLKIT__NET__MESSAGE__H__
