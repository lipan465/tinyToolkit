#ifndef __TINY_TOOLKIT__NET__COMPLETER__H__
#define __TINY_TOOLKIT__NET__COMPLETER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 完成者
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetContext;

	class TINY_TOOLKIT_API INetCompleter
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~INetCompleter() = default;

		/**
		 *
		 * 逻辑处理
		 *
		 * @param netContext 网络上下文
		 * @param sysContext 系统上下文
		 *
		 */
		virtual void Logic(NetContext * netContext, void * sysContext) = 0;
	};
}


#endif // __TINY_TOOLKIT__NET__COMPLETER__H__
