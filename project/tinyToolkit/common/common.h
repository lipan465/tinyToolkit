#ifndef __TINY_TOOLKIT__COMMON__COMMON__H__
#define __TINY_TOOLKIT__COMMON__COMMON__H__


/**
 *
 *  作者: hm
 *
 *  说明: 公共信息
 *
 */


/// common
#include "macro.h"
#include "version.h"
#include "support.h"
#include "platform.h"
#include "compiler.h"
#include "function.h"

/// c
#include <csignal>
#include <cassert>
#include <fcntl.h>

/// c++
#include <map>
#include <set>
#include <queue>
#include <regex>
#include <future>
#include <random>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <functional>
#include <unordered_map>


/// 各平台头文件
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <io.h>
#  include <direct.h>
#  include <WS2tcpip.h>
#  include <MSWSock.h>
#  include <DbgHelp.h>
#  include <TlHelp32.h>
#
#  define strcasecmp _stricmp
#
#  pragma warning(disable:4251)
#  pragma warning(disable:4275)
#
#  pragma comment(lib,"ws2_32.lib")  /// socket编程需用的动态链接库
#  pragma comment( lib, "dbghelp.lib")  /// 调试需要用到的动态链接库
#
#else
#
#  include <pwd.h>
#  include <dlfcn.h>
#  include <netdb.h>
#  include <dirent.h>
#  include <cxxabi.h>
#  include <unistd.h>
#  include <syslog.h>
#  include <execinfo.h>
#
#  include <sys/stat.h>
#  include <sys/syscall.h>
#
#  include <arpa/inet.h>
#
#  include <netinet/tcp.h>
#
#  if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#    include <sys/event.h>
#    include <mach-o/dyld.h>
#
#  elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#    include <sys/epoll.h>
#
#  endif
#
#endif


namespace tinyToolkit
{
	using SystemClockType = std::chrono::system_clock;
	using SystemClockDuration = std::chrono::system_clock::duration;
	using SystemClockTimePoint = std::chrono::system_clock::time_point;

	using SteadyClockType = std::chrono::steady_clock;
	using SteadyClockDuration = std::chrono::steady_clock::duration;
	using SteadyClockTimePoint = std::chrono::steady_clock::time_point;
}


#endif // __TINY_TOOLKIT__COMMON__COMMON__H__
