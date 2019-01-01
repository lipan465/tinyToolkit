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
		static uint64_t ThreadID()
		{
			uint64_t tid = 0;

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			tid = ::GetCurrentThreadId();

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			tid = static_cast<uint64_tt>(::syscall(SYS_gettid));

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			pthread_threadid_np(nullptr, &tid);

#else

			tid = static_cast<uint64_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));

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
		static uint64_t ProcessID()
		{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			return ::GetCurrentProcessId();

#else

			return static_cast<uint64_t>(::getpid());

#endif
		}
	};
}


#endif // __TINY_TOOLKIT__SYSTEM__OS__H__
