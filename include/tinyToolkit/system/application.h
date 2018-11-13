#ifndef __TINY_TOOLKIT__SYSTEM__APPLICATION__H__
#define __TINY_TOOLKIT__SYSTEM__APPLICATION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 应用处理
 *
 */


#include "os.h"

#include "../utilities/time.h"
#include "../utilities/file.h"
#include "../utilities/string.h"
#include "../utilities/filesystem.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Application
	{
	public:
		/**
		 *
		 * 进程是否存在
		 *
		 * @return 进程状态
		 *
		 */
		static bool Exist()
		{
			static std::pair<tinyToolkit::LockFile, bool> value({ }, true);

			if (value.second)
			{
				if (value.first.Open(tinyToolkit::String::Format("/var/run/{}.pid", Name()), true))
				{
					value.first << std::to_string(tinyToolkit::OS::ProcessID());
				}

				value.second = false;
			}

			return !value.first.IsOpen();
		}

		/**
		 *
		 * 编译时间
		 *
		 * @return 编译时间
		 *
		 */
		static std::time_t CompileTime()
		{
			static std::pair<std::time_t, bool> value(0, true);

			if (value.second)
			{
				static std::map<std::string, uint32_t> month
				{
					{ "Jan", 1  },
					{ "Feb", 2  },
					{ "Mar", 3  },
					{ "Apr", 4  },
					{ "May", 5  },
					{ "Jun", 6  },
					{ "Jul", 7  },
					{ "Aug", 8  },
					{ "Sep", 9  },
					{ "Oct", 10 },
					{ "Nov", 11 },
					{ "Dec", 12 },
				};

				std::tm date = { 0 };

				date.tm_year = static_cast<int32_t>(strtol(__DATE__ + 7, nullptr, 10)) - 1900;
				date.tm_mon  = month[std::string(__DATE__, __DATE__ + 3)] - 1;
				date.tm_mday = static_cast<int32_t>(strtol(__DATE__ + 4, nullptr, 10));
				date.tm_hour = static_cast<int32_t>(strtol(__TIME__ + 0, nullptr, 10));
				date.tm_min  = static_cast<int32_t>(strtol(__TIME__ + 3, nullptr, 10));
				date.tm_sec  = static_cast<int32_t>(strtol(__TIME__ + 6, nullptr, 10));

				value.first = tinyToolkit::Time::FromTm(date);

				value.second = false;
			}

			return value.first;
		}

		/**
		 *
		 * 程序名
		 *
		 * @return 程序名
		 *
		 */
		static const std::string & Name()
		{
			static std::pair<std::string, bool> value({}, true);

			if (value.second)
			{
				value.first = tinyToolkit::Filesystem::FileName(Path());

				value.second = false;
			}

			return value.first;
		}

		/**
		 *
		 * 程序完整路径
		 *
		 * @return 程序完整路径
		 *
		 */
		static const std::string & Path()
		{
			static std::pair<std::string, bool> value({}, true);

			if (value.second)
			{
				char str[TINY_TOOLKIT_KB] = { 0 };

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

				GetModuleFileName(nullptr, str, sizeof(str));

#else

				readlink("/proc/self/exe", str, sizeof(str));

#endif

				value.first = str;

				value.second = false;
			}

			return value.first;
		}

		/**
		 *
		 * 程序前缀
		 *
		 * @return 程序前缀
		 *
		 */
		static const std::string & Steam()
		{
			static std::pair<std::string, bool> value({}, true);

			if (value.second)
			{
				value.first = tinyToolkit::Filesystem::FileSteam(Path());

				value.second = false;
			}

			return value.first;
		}

		/**
		 *
		 * 程序扩展名
		 *
		 * @return 程序扩展名
		 *
		 */
		static const std::string & Extension()
		{
			static std::pair<std::string, bool> value({}, true);

			if (value.second)
			{
				value.first = tinyToolkit::Filesystem::FileExtension(Path());

				value.second = false;
			}

			return value.first;
		}

		/**
		 *
		 * 程序目录
		 *
		 * @return 程序目录
		 *
		 */
		static const std::string & Directory()
		{
			static std::pair<std::string, bool> value({}, true);

			if (value.second)
			{
				value.first = tinyToolkit::Filesystem::FileDirectory(Path());

				value.second = false;
			}

			return value.first;
		}

		/**
		 *
		 * 编译时间字符串
		 *
		 * @return 编译时间字符串
		 *
		 */
		static const std::string & CompileTimeString()
		{
			static std::pair<std::string, bool> value({}, true);

			if (value.second)
			{
				value.first = tinyToolkit::Time::FormatTimeString(CompileTime());

				value.second = false;
			}

			return value.first;
		}
	};
}


#endif // __TINY_TOOLKIT__SYSTEM__APPLICATION__H__
