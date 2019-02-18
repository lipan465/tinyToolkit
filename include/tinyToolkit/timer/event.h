#ifndef __TINY_TOOLKIT__TIMER__EVENT__H__
#define __TINY_TOOLKIT__TIMER__EVENT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 定时器事件
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ITimerEvent
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~ITimerEvent() = default;

		/**
		 *
		 * 暂停事件调用回调函数
		 *
		 */
		virtual void OnPause() = 0;

		/**
		 *
		 * 恢复事件调用回调函数
		 *
		 */
		virtual void OnResume() = 0;

		/**
		 *
		 * 触发事件调用回调函数
		 *
		 */
		virtual void OnTrigger() = 0;

		/**
		 *
		 * 结束事件调用回调函数
		 *
		 * @param forced 强制性
		 *
		 */
		virtual void OnFinish(bool forced) = 0;
	};
}


#endif // __TINY_TOOLKIT__TIMER__EVENT__H__
