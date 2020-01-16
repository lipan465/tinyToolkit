/**
 *
 *  作者: hm
 *
 *  说明: 消息
 *
 */


#include "message.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param buf 待发送数据缓冲区指针
	 * @param len 待发送数据缓冲区长度
	 *
	 */
	NetMessage::NetMessage(const void * buf, std::size_t len) : length(len)
	{
		buffer = new char[length + 1];

		memcpy(buffer, buf, length);

		buffer[length] = '\0';
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	NetMessage::~NetMessage()
	{
		if (buffer)
		{
			delete[] buffer;

			buffer = nullptr;
		}
	}
}
