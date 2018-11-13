#ifndef __TINY_TOOLKIT__UTILITIES__CONTAINER__H__
#define __TINY_TOOLKIT__UTILITIES__CONTAINER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 容器处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Container
	{
	public:
		/**
		 *
		 * 清理容器
		 *
		 * @tparam TypeT [all container types]
		 *
		 * @param container 待清理容器
		 *
		 */
		template <class TypeT>
		static void Clear(TypeT & container)
		{
			TypeT().swap(container);
		}
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__CONTAINER__H__
