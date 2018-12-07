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
#if TINY_TOOLKIT_PLATFORM != TINY_TOOLKIT_PLATFORM_WINDOWS

		/**
		 *
		 * 复制进程
		 *
		 * @param isCloseIO 是否关闭输出
		 *
		 * @return 复制结果
		 *
		 */
		static bool Fork(bool isCloseIO = true)
		{
			/**
			 *
			 * fork函数返回两个值
			 *
			 * 对于子进程, 返回0
			 *
			 * 对于父进程, 返回子进程ID
			 *
			 */
			pid_t pid = ::fork();

			if (pid < 0)
			{
			return false;
			}

			if (pid != 0)  /// 父进程
			{
				exit(0);
			}

			/**
			 *
			 * 当进程是会话的领头进程时setsid()调用失败并返回(-1)
			 *
			 * setsid()调用成功后, 返回新的会话的ID, 调用setsid函数的进程成为新的会话的领头进程, 并与其父进程的会话组和进程组脱离
			 *
			 * 由于会话对控制终端的独占性, 进程同时与控制终端脱离
			 *
			 *
			 * 之前parent和child运行在同一个session里, parent是会话(session)的领头进程
			 *
			 * 所以作为session头的parent如果exit结束执行的话,那么会话 session组中的所有进程将都被杀死
			 *
			 * 执行setsid()之后, child将重新获得一个新的会话(session)id
			 *
			 * 这时parent退出之后, 将不会影响到child了
			 *
			 */
			pid = setsid();

			if (pid < -1)
			{
				return false;
			}

			if (isCloseIO)
			{
				int32_t fd = ::open("/dev/null", O_RDWR, 0);

				if (fd != -1)
				{
					::dup2(fd, STDIN_FILENO);   /// 关闭标准输入
					::dup2(fd, STDOUT_FILENO);  /// 关闭标准输出
					::dup2(fd, STDERR_FILENO);  /// 关闭标准错误输出

					::close(fd);
				}
			}

			return true;
		}

		/**
		 *
		 * 进程是否存在
		 *
		 * @return 进程状态
		 *
		 */
		static bool Exist()
		{
			static std::pair<LockFile, bool> value({ }, true);

			if (value.second)
			{
				if (value.first.Open(String::Format("/var/run/{}.pid", Name()), true))
				{
					value.first << std::to_string(OS::ProcessID());
				}

				value.second = false;
			}

			return !value.first.IsOpen();
		}

#endif

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

				value.first = Time::FromTm(date);

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
				value.first = Filesystem::FileName(Path());

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
				char str[TINY_TOOLKIT_PATH_MAX] = { 0 };

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
				value.first = Filesystem::FileSteam(Path());

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
				value.first = Filesystem::FileExtension(Path());

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
				value.first = Filesystem::FileDirectory(Path());

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
				value.first = Time::FormatTimeString(CompileTime());

				value.second = false;
			}

			return value.first;
		}
	};
}


#endif // __TINY_TOOLKIT__SYSTEM__APPLICATION__H__
