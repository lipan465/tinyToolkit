#ifndef __TINY_TOOLKIT__NET__COMPLETER__H__
#define __TINY_TOOLKIT__NET__COMPLETER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯完成者
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetEvent;

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
		 * 回调函数
		 *
		 * @param netEvent 网络事件
		 * @param sysEvent 系统事件
		 *
		 */
		virtual void OnCallback(NetEvent * netEvent, void * sysEvent) = 0;
	};
}


#endif // __TINY_TOOLKIT__NET__COMPLETER__H__
