/**
 *
 *  作者: hm
 *
 *  说明: 进程信息
 *
 */


#include "os.h"


namespace tinyToolkit
{
	/**
	 *
	 * 是否是大端字节序
	 *
	 * @return 是否是大端字节序
	 *
	 */
	bool OS::IsBigEndian()
	{
		static uint32_t value = 0x0001;

		return *((uint8_t *)&value) == 0;
	}

	/**
	 *
	 * 是否是小端字节序
	 *
	 * @return 是否是小端字节序
	 *
	 */
	bool OS::IsLittleEndian()
	{
		static uint32_t value = 0x0001;

		return *((uint8_t *)&value) == 1;
	}

	/**
	 *
	 * 获取处理器个数
	 *
	 * @return 处理器个数
	 *
	 */
	int64_t OS::ProcessorCount()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		SYSTEM_INFO info{ };

		GetSystemInfo(&info);

		return info.dwNumberOfProcessors;

#else

		return sysconf(_SC_NPROCESSORS_CONF);

#endif
	}

	/**
	 *
	 * 获取线程id
	 *
	 * @return 线程id
	 *
	 */
	uint64_t OS::ThreadID()
	{
		uint64_t tid = 0;

	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		tid = ::GetCurrentThreadId();

	#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

		tid = static_cast<uint64_t>(::syscall(SYS_gettid));

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
	uint64_t OS::ProcessID()
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		return ::GetCurrentProcessId();

	#else

		return static_cast<uint64_t>(::getpid());

	#endif
	}

	/**
	 *
	 * 获取用户名
	 *
	 * @return 用户名
	 *
	 */
	std::string OS::UserName()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		char name[TINY_TOOLKIT_NAME_MAX]{ 0 };

		DWORD size = sizeof(name);

		return GetUserName(name, &size) ? name : "";

#else

		char name[TINY_TOOLKIT_NAME_MAX]{ 0 };

		struct passwd   pwd{ };
		struct passwd * result = nullptr;

		int err = getpwuid_r(getuid(), &pwd, name, sizeof(name), &result);

		return result == nullptr && err ? "" : name;

#endif
	}

	/**
	 *
	 * 获取计算机名称
	 *
	 * @return 计算机名称
	 *
	 */
	std::string OS::ComputerName()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		char name[TINY_TOOLKIT_PATH_MAX]{ 0 };

		DWORD size = sizeof(name);

		return GetComputerName(name, &size) ? name : "";

#else

		char name[TINY_TOOLKIT_PATH_MAX]{ 0 };

		return gethostname(name, sizeof(name)) == 0 ? name : "";

#endif
	}

	/**
	 *
	 * 获取最后一个错误信息
	 *
	 * @return 最后一个错误信息
	 *
	 */
	std::string OS::LastErrorMessage()
	{
		char message[TINY_TOOLKIT_KB]{ 0 };

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		strerror_s(message, sizeof(message), errno);

#else

		strerror_r(errno, message, sizeof(message));

#endif

		return message;
	}
}
