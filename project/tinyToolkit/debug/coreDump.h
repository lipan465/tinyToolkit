#ifndef __TINY_TOOLKIT__DEBUG__CORE_DUMP__H__
#define __TINY_TOOLKIT__DEBUG__CORE_DUMP__H__


/**
 *
 *  作者: hm
 *
 *  说明: 核心转储
 *
 */


#include "../utilities/singleton.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API CoreDump
	{
	public:
		/**
		 *
		 * 注册
		 *
		 */
		static void Register();

	protected:
		/**
		 *
		 * 构造函数
		 *
		 */
		CoreDump() = default;

		/**
		 *
		 * 析构函数
		 *
		 */
		~CoreDump();
	};
}


#endif // __TINY_TOOLKIT__DEBUG__CORE_DUMP__H__
