#ifndef __TINY_TOOLKIT__UTILITIES__OPERATOR__H__
#define __TINY_TOOLKIT__UTILITIES__OPERATOR__H__


/**
 *
 *  作者: hm
 *
 *  说明: 操作
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Operator
	{
	public:
		/**
		 *
		 * 清理
		 *
		 * @tparam TypeT [all types]
		 *
		 * @param value 待清理内容
		 *
		 */
		template <typename TypeT>
		static void Clear(TypeT & value)
		{
			TypeT().swap(value);
		}
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__OPERATOR__H__
