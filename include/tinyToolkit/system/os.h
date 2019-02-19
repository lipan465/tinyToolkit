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
	};
}


#endif // __TINY_TOOLKIT__SYSTEM__OS__H__
