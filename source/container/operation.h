#ifndef __TINY_TOOLKIT__CONTAINER__OPERATION__H__
#define __TINY_TOOLKIT__CONTAINER__OPERATION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 操作
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


namespace tinyToolkit
{
	namespace container
	{
		class TINY_TOOLKIT_API Operation
		{
		public:
			/**
			 *
			 * 交换
			 *
			 * @param container 容器对象
			 *
			 */
			template <typename ContainerTypeT>
			static void Swap(ContainerTypeT & container)
			{
				ContainerTypeT().swap(container);
			}
		};
	}
}


#endif // __TINY_TOOLKIT__CONTAINER__OPERATION__H__
