#ifndef __TINY_TOOLKIT__CONTAINER__OPERATOR__H__
#define __TINY_TOOLKIT__CONTAINER__OPERATOR__H__


/**
 *
 *  作者: hm
 *
 *  说明: 容器操作
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ContainerOperator
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
		template <typename TypeT>
		static void Clear(TypeT & container)
		{
			TypeT().swap(container);
		}
	};
}


#endif // __TINY_TOOLKIT__CONTAINER__OPERATOR__H__
