#ifndef __TINY_TOOLKIT__SYSTEM__SIGNAL__H__
#define __TINY_TOOLKIT__SYSTEM__SIGNAL__H__


/**
 *
 *  作者: hm
 *
 *  说明: 信号处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Signal
	{
	public:
		/**
		 *
		 * 向自身发送信号
		 *
		 * @param signalNo 待发送信号
		 *
		 * @return 发送结果
		 *
		 */
		static int32_t Raise(int32_t signalNo);

		/**
		 *
		 * 注册忽略信号
		 *
		 */
		static void RegisterIgnore();

		/**
		 *
		 * 注册堆栈跟踪信号
		 *
		 * @param handler 信号触发时调用的函数
		 *
		 */
		static void RegisterStackTrace(void(* handler)(int32_t));

		/**
		 *
		 * 注册结束信号
		 *
		 * @param handler 信号触发时调用的函数
		 *
		 */
		static void RegisterTerminate(void(* handler)(int32_t));

		/**
		 *
		 * 注册信号动作
		 *
		 * @param signalNo 待注册信号
		 * @param handler 信号触发时调用的函数
		 *
		 */
		static void RegisterAction(int32_t signalNo, void(* handler)(int32_t));
	};
}


#endif // __TINY_TOOLKIT__SYSTEM__SIGNAL__H__
