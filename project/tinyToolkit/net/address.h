#ifndef __TINY__TOOLKIT__NET__ADDRESS__H__
#define __TINY__TOOLKIT__NET__ADDRESS__H__


/**
 *
 *  作者: hm
 *
 *  说明: 地址
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetAddress
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		NetAddress() = default;

		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool IsValid();

	public:
		uint16_t port{ 0 };

		std::string host{ };
	};
}


#endif // __TINY__TOOLKIT__NET__ADDRESS__H__
