#ifndef __TINY_TOOLKIT__UTILITIES__BYTE__H__
#define __TINY_TOOLKIT__UTILITIES__BYTE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 字节处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Bytes
	{
	public:
		/**
		 *
		 * 字符对应的16进制
		 *
		 * @param value 待转换字符
		 *
		 * @return 16进制字符
		 *
		 */
		static uint8_t AsHex(uint8_t value)
		{
			return static_cast<uint8_t>(value > 9 ? value - 10 + 'A': value + '0');
		}

		/**
		 *
		 * 16进制对应的字符
		 *
		 * @param value 待转换字符
		 *
		 * @return 字符
		 *
		 */
		static uint8_t AsChar(uint8_t value)
		{
			return static_cast<uint8_t>(::isdigit(value) == 0 ? value + 10 - 'A' : value - '0');
		}
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__BYTE__H__
