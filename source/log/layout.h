#ifndef __TINY_TOOLKIT__LOG__LAYOUT__H__
#define __TINY_TOOLKIT__LOG__LAYOUT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 布局
 *
 */


#include "context.h"


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API ILayout
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~ILayout() = default;

			/**
			 *
			 * 格式化
			 *
			 * @param context 上下文
			 *
			 * @return 格式化后内容
			 *
			 */
			virtual std::string Format(const Context & context) = 0;
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__LAYOUT__H__
