#ifndef __TINY_TOOLKIT__DEBUG__BACKTRACE__H__
#define __TINY_TOOLKIT__DEBUG__BACKTRACE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 堆栈信息
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Backtrace
	{
	public:
		/**
		 *
		 * 打印堆栈信息
		 *
		 * @param signalNo 信号
		 *
		 */
		static void Print(int32_t signalNo = 0);
	};
}


#endif // __TINY_TOOLKIT__DEBUG__BACKTRACE__H__
