#ifndef __TINY_TOOLKIT__SYSTEM__OS__H__
#define __TINY_TOOLKIT__SYSTEM__OS__H__


/**
 *
 *  作者: hm
 *
 *  说明: os信息
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
		static int64_t ThreadID()
		{
			int64_t tid = 0;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			tid = ::GetCurrentThreadId();

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			tid = ::syscall(SYS_gettid);

#else

			tid = static_cast<int64_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));  /// std::this_thread::get_id()慢得多(特别是在VS 2013下)

#endif

			return tid;
		}

		/**
		 *
		 * 获取当前进程id
		 *
		 * @return pid
		 *
		 */
		static int64_t ProcessID()
		{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			return ::GetCurrentProcessId();

#else

			return ::getpid();

#endif
		}
	};
}


#endif // __TINY_TOOLKIT__SYSTEM__OS__H__
