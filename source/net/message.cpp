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
	namespace net
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param buf 内容
		 * @param len 长度
		 *
		 */
		Message::Message(const void * buf, std::size_t len) : length(len)
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
		Message::~Message()
		{
			if (buffer == nullptr)
			{
				return;
			}

			delete[] buffer;

			buffer = nullptr;
		}
	}
}
