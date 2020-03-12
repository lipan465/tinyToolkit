/**
 *
 *  作者: hm
 *
 *  说明: 应用处理
 *
 */


#include "time.h"
#include "string.h"
#include "filesystem.h"
#include "application.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <TlHelp32.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <fcntl.h>
#  include <unistd.h>
#
#  include <sys/stat.h>
#
#  include <mach-o/dyld.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <climits>
#
#  include <fcntl.h>
#  include <unistd.h>
#
#  include <sys/stat.h>
#  include <sys/syscall.h>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		/**
		 *
		 * 是否存在
		 *
		 * @return 是否存在
		 *
		 */
		bool Application::Exist()
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

			if (hProcessSnap == INVALID_HANDLE_VALUE)
			{
				return true;
			}

			bool find = false;

			PROCESSENTRY32 process{ };

			process.dwSize = sizeof(PROCESSENTRY32);

			if (::Process32First(hProcessSnap, &process))
			{
				do
				{
					if (strcmp(process.szExeFile, Name().c_str()) == 0)
					{
						find = true;

						break;
					}
				}
				while (::Process32Next(hProcessSnap, &process));
			}

			::CloseHandle(hProcessSnap);

			return find;

		#else

			{
				FILE * fp = ::popen(String::Format("ps -ef | grep {0} | grep -v grep | grep -v /{0}/ | wc -l", Name()).c_str(), "r");

				if (fp == nullptr)
				{
					return true;
				}

				char buffer[1024]{ 0 };

				if (::fgets(buffer, 1024, fp))
				{
					if (::strtol(buffer, nullptr, 10) > 1)
					{
						::pclose(fp);

						return true;
					}
				}

				::pclose(fp);
			}

			{
			#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

				Filesystem::CreateDirectories("/usr/local/var/run");

				int32_t fd = ::open(String::Format("/usr/local/var/run/{}.pid", Name()).c_str(), O_WRONLY | O_CREAT, 0644);

			#else

				Filesystem::CreateDirectories("/var/run");

				int32_t fd = ::open(String::Format("/var/run/{}.pid", Name()).c_str(), O_WRONLY | O_CREAT, 0644);

			#endif

				if (fd < 0)
				{
					return true;
				}

				struct flock lock
				{
					.l_type = F_WRLCK,  /// 加锁的类型: 只读锁(F_RDLCK), 读写锁(F_WRLCK), 或是解锁(F_UNLCK)
					.l_whence = SEEK_SET,  /// 加锁部分的开始位置
					.l_start = 0,  /// 加锁部分的开始位置
					.l_len = 0,  /// 加锁的长度
					.l_pid = 0,  /// pid
				};

				if (::fcntl(fd, F_SETLK, &lock) == -1)  /// 尝试在整个文件上设置锁定
				{
					::close(fd);

					return true;
				}

				if (::ftruncate(fd, 0) == -1)  /// 清空文件
				{
					::close(fd);

					return false;
				}

				auto pidStr = std::to_string(ProcessID());

				if (::write(fd, pidStr.data(), pidStr.size()) != static_cast<ssize_t>(pidStr.size()))
				{
					::close(fd);

					return false;
				}

				int32_t val = fcntl(fd, F_GETFD, 0);  /// 获得文件描述符标记

				if (val == -1)
				{
					::close(fd);

					return false;
				}

				/**
				 *
				 * 对描述符设置了FD_CLOEXEC
				 *
				 * 使用execl执行的程序里, 此描述符被关闭, 不能再使用它
				 * 使用fork调用的子进程中, 此描述符并不关闭, 仍可使用
				 *
				 */
				val |= FD_CLOEXEC;

				if (::fcntl(fd, F_SETFD, val) == -1)  /// 设置文件描述符标记
				{
					::close(fd);

					return false;
				}

				return false;
			}

		#endif
		}

		/**
		 *
		 * 开启守护进程
		 *
		 * @return 是否开启成功
		 *
		 */
		bool Application::Deamon()
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			return false;

		#else

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
			pid = ::setsid();

			if (pid < -1)
			{
				return false;
			}

			::umask(0);  /// 修改掩码

			::chdir("/");  /// 修改工作目录

			int32_t fd = ::open("/dev/null", O_RDWR, 0);

			if (fd != -1)
			{
				::dup2(fd, STDIN_FILENO);   /// 关闭标准输入
				::dup2(fd, STDOUT_FILENO);  /// 关闭标准输出
				::dup2(fd, STDERR_FILENO);  /// 关闭标准错误输出

				::close(fd);
			}

			return true;

		#endif
		}



		/**
		 *
		 * 线程序列号
		 *
		 * @return 线程序列号
		 *
		 */
		uint64_t Application::ThreadID()
		{
			uint64_t tid = 0;

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			tid = ::GetCurrentThreadId();

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

			tid = static_cast<uint64_t>(::syscall(SYS_gettid));

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			::pthread_threadid_np(nullptr, &tid);

		#else

			tid = static_cast<uint64_t>
			(
				std::hash<std::thread::id>()
				(
					std::this_thread::get_id()
				)
			);

		#endif

			return tid;
		}

		/**
		 *
		 * 进程序列号
		 *
		 * @return 进程序列号
		 *
		 */
		uint64_t Application::ProcessID()
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			return ::GetCurrentProcessId();

		#else

			return static_cast<uint64_t>(::getpid());

		#endif
		}

		/**
		 *
		 * 编译时间
		 *
		 * @param date 日期
		 * @param time 时间
		 *
		 * @return 编译时间
		 *
		 */
		std::time_t Application::CompileTime(const char * date, const char * time)
		{
			return Time::FromString(CompileTimeString(date, time).c_str());
		}

		/**
		 *
		 * 路径
		 *
		 * @return 路径
		 *
		 */
		std::string Application::Path()
		{
			char path[TINY_TOOLKIT_PATH_MAX + 1]{ 0 };

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			::GetModuleFileName(nullptr, path, TINY_TOOLKIT_PATH_MAX);

		#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

			uint32_t size = TINY_TOOLKIT_PATH_MAX;

			::_NSGetExecutablePath(path, &size);

			path[size] = '\0';

		#else

			::readlink("/proc/self/exe", path, sizeof(path));

		#endif

			return path;
		}

		/**
		 *
		 * 名称
		 *
		 * @return 名称
		 *
		 */
		std::string Application::Name()
		{
			return Filesystem::Name(Path());
		}

		/**
		 *
		 * 前缀
		 *
		 * @return 前缀
		 *
		 */
		std::string Application::Steam()
		{
			return Filesystem::Steam(Name());
		}

		/**
		 *
		 * 扩展名
		 *
		 * @return 扩展名
		 *
		 */
		std::string Application::Extension()
		{
			return Filesystem::Extension(Name());
		}

		/**
		 *
		 * 父级目录
		 *
		 * @return 父级目录
		 *
		 */
		std::string Application::ParentDirectory()
		{
			return Filesystem::ParentDirectory(Path());
		}

		/**
		 *
		 * 编译时间字符串
		 *
		 * @param date 日期
		 * @param time 时间
		 *
		 * @return 编译时间
		 *
		 */
		std::string Application::CompileTimeString(const char * date, const char * time)
		{
			char buffer[1024]{ 0 };

			snprintf
			(
				buffer,
				sizeof(buffer),
				"%04d-%02d-%02d %s",
				(((date[7] - '0') * 10 + (date[8] - '0')) * 10 + (date[9] - '0')) * 10 + (date[10] - '0'),

				date[2] == 'n' ? 1 :
				date[2] == 'b' ? 2 :
				date[2] == 'r' ? (date[0] == 'M' ? 3 : 4) :
				date[2] == 'y' ? 5 :
				date[2] == 'n' ? 6 :
				date[2] == 'l' ? 7 :
				date[2] == 'g' ? 8 :
				date[2] == 'p' ? 9 :
				date[2] == 't' ? 10 :
				date[2] == 'v' ? 11 : 12,

				(date[4] == ' ' ? 0 : date[4] - '0') * 10 + (date[5] - '0'),

				time
			);

			return buffer;
		}
	}
}
