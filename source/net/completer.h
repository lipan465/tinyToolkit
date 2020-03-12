#ifndef __TINY_TOOLKIT__NET__COMPLETER__H__
#define __TINY_TOOLKIT__NET__COMPLETER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 完成者
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API Context;

		class TINY_TOOLKIT_API ICompleter
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~ICompleter() = default;

			/**
			 *
			 * 处理流程
			 *
			 * @param netContext 网络上下文
			 * @param sysContext 系统上下文
			 *
			 */
			virtual void Process(Context * netContext, void * sysContext) = 0;
		};
	}
}


#endif // __TINY_TOOLKIT__NET__COMPLETER__H__
