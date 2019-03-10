#ifndef __TINY_TOOLKIT__SYSTEM__OS__H__
#define __TINY_TOOLKIT__SYSTEM__OS__H__


/**
 *
 *  作者: hm
 *
 *  说明: 进程信息
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API OS
	{
	public:
		/**
		 *
		 * 获取线程id
		 *
		 * @return 线程id
		 *
		 */
		static uint64_t ThreadID();

		/**
		 *
		 * 获取当前进程id
		 *
		 * @return pid
		 *
		 */
		static uint64_t ProcessID();

		/**
		 *
		 * 最后一个错误代码
		 *
		 * @return 错误代码
		 *
		 */
		static int32_t LastErrorCode();

		/**
		 *
		 * 最后一个错误信息
		 *
		 * @return 错误信息
		 *
		 */
		static const char * LastErrorMessage();
	};
}


#endif // __TINY_TOOLKIT__SYSTEM__OS__H__
