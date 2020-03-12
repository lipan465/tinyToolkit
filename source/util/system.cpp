/**
 *
 *  作者: hm
 *
 *  说明: 系统处理
 *
 */


#include "system.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <climits>
#
#  include <pwd.h>
#  include <unistd.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <climits>
#
#  include <pwd.h>
#  include <unistd.h>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		/**
		 *
		 * 是否是大端字节序
		 *
		 * @return 是否是大端字节序
		 *
		 */
		bool System::IsBigEndian()
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
		bool System::IsLittleEndian()
		{
			static uint32_t value = 0x0001;

			return *((uint8_t *)&value) == 1;
		}

		/**
		 *
		 * 处理器个数
		 *
		 * @return 处理器个数
		 *
		 */
		uint32_t System::ProcessorCount()
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			SYSTEM_INFO info{ };

			::GetSystemInfo(&info);

			return info.dwNumberOfProcessors;

		#else

			return static_cast<uint32_t>(::sysconf(_SC_NPROCESSORS_CONF));

		#endif
		}

		/**
		 *
		 * 用户名
		 *
		 * @return 用户名
		 *
		 */
		std::string System::UserName()
		{
			char name[TINY_TOOLKIT_NAME_MAX]{ 0 };

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			DWORD size = sizeof(name);

			if (::GetUserName(name, &size))
			{
				return name;
			}

		#else

			struct passwd   pwd{ };
			struct passwd * result = nullptr;

			::getpwuid_r(getuid(), &pwd, name, sizeof(name), &result);

			if (result)
			{
				return name;
			}

		#endif

			return { };
		}

		/**
		 *
		 * 计算机名称
		 *
		 * @return 计算机名称
		 *
		 */
		std::string System::ComputerName()
		{
			char name[TINY_TOOLKIT_PATH_MAX]{ 0 };

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			DWORD size = sizeof(name);

			if (::GetComputerName(name, &size))
			{
				return name;
			}

		#else

			if (::gethostname(name, sizeof(name)) == 0)
			{
				return name;
			}

		#endif

			return { };
		}
	}
}
