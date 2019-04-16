#ifndef __TINY_TOOLKIT__COMMON__COMMON__H__
#define __TINY_TOOLKIT__COMMON__COMMON__H__


/**
 *
 *  作者: hm
 *
 *  说明: 公共信息
 *
 */


/// c
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <cstdint>
#include <cassert>
#include <fcntl.h>

/// common
#include "macro.h"
#include "version.h"
#include "support.h"
#include "platform.h"
#include "compiler.h"
#include "function.h"

/// c++
#include <set>
#include <map>
#include <list>
#include <array>
#include <queue>
#include <stack>
#include <mutex>
#include <regex>
#include <chrono>
#include <atomic>
#include <vector>
#include <string>
#include <memory>
#include <future>
#include <thread>
#include <random>
#include <iomanip>
#include <utility>
#include <numeric>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <functional>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>
#include <condition_variable>


/// 各平台头文件
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <io.h>
#  include <direct.h>
#  include <Ws2tcpip.h>
#  include <MSWSock.h>
#  include <windows.h>
#  include <WinSock2.h>
#  include <tlhelp32.h>
#
#  define strcasecmp _stricmp
#
#  pragma comment(lib,"ws2_32.lib")  /// socket编程需用的动态链接库
#  pragma comment(lib, "kernel32.lib")  /// IOCP需要用到的动态链接库
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
#  include <sys/types.h>
#  include <sys/ioctl.h>
#  include <sys/socket.h>
#  include <sys/syscall.h>
#  include <sys/resource.h>
#
#  include <arpa/inet.h>
#
#  include <netinet/ip.h>
#  include <netinet/tcp.h>
#  include <netinet/udp.h>
#  include <netinet/if_ether.h>
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
	using ByteSet = std::set<uint8_t>;
	using ByteVector = std::vector<uint8_t>;
	using ByteUnorderedSet = std::unordered_set<uint8_t>;

	using StringSet = std::set<std::string>;
	using StringVector = std::vector<std::string>;
	using StringUnorderedSet = std::unordered_set<std::string>;

	using ClockType = std::chrono::system_clock;
	using ClockDuration = std::chrono::system_clock::duration;
	using ClockTimePoint = std::chrono::system_clock::time_point;
}


#endif // __TINY_TOOLKIT__COMMON__COMMON__H__
