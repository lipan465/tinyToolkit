#ifndef __TINY_TOOLKIT__DEBUG__DUMP__H__
#define __TINY_TOOLKIT__DEBUG__DUMP__H__


/**
 *
 *  作者: hm
 *
 *  说明: 转储
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


namespace tinyToolkit
{
	namespace debug
	{
		class TINY_TOOLKIT_API Dump
		{
		public:
			/**
			 *
			 * 注册
			 *
			 */
			static void Register();

			/**
			 *
			 * 注销
			 *
			 */
			static void UnRegister();
		};
	}
}


#endif // __TINY_TOOLKIT__DEBUG__DUMP__H__
