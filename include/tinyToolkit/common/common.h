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

/// std
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
#include <variant>
#include <optional>
#include <iostream>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <functional>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>
#include <condition_variable>

/// common
#include "macro.h"
#include "version.h"
#include "support.h"
#include "platform.h"
#include "compiler.h"
#include "function.h"


/// 支持其余c++17
#if (TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS && defined(_HAS_CXX17)) || \
	(TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX && TINY_TOOLKIT_COMPILER_VERSION >= 80100)

#include <filesystem>

#define TINY_TOOLKIT_SUPPORT_OTHER_CXX_17

#endif


/// 各平台头文件
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

#include <windows.h>

#pragma comment(lib,"ws2_32.lib")

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

#include <unistd.h>
#include <execinfo.h>
#include <arpa/inet.h>
#include <sys/syscall.h>

#endif


using ByteSet = std::set<uint8_t>;
using ByteVector = std::vector<uint8_t>;
using ByteUnorderedSet = std::unordered_set<uint8_t>;

using StringSet = std::set<std::string>;
using StringVector = std::vector<std::string>;
using StringUnorderedSet = std::unordered_set<std::string>;


#endif // __TINY_TOOLKIT__COMMON__COMMON__H__
