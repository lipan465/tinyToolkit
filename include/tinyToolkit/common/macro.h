#ifndef __TINY_TOOLKIT__COMMON__MACRO__H__
#define __TINY_TOOLKIT__COMMON__MACRO__H__


/**
 *
 *  作者: hm
 *
 *  说明: 宏定义
 *
 */


#include "platform.h"


#define TINY_TOOLKIT_PI								3.1415926535898f


#define TINY_TOOLKIT_SECOND							(1)
#define TINY_TOOLKIT_MINUTE							(60 * TINY_TOOLKIT_SECOND)
#define TINY_TOOLKIT_HOUR							(60 * TINY_TOOLKIT_MINUTE)
#define TINY_TOOLKIT_DAY							(24 * TINY_TOOLKIT_HOUR)
#define TINY_TOOLKIT_WEEK							(7 * TINY_TOOLKIT_DAY)
#define TINY_TOOLKIT_YEAR							(12 * TINY_TOOLKIT_WEEK)


#define TINY_TOOLKIT_BYTE							(1)  /// Byte
#define TINY_TOOLKIT_KB								(1024 * TINY_TOOLKIT_BYTE)  /// Kilobyte
#define TINY_TOOLKIT_MB								(1024 * TINY_TOOLKIT_KB)  /// Mega byte
#define TINY_TOOLKIT_GB								(1024 * TINY_TOOLKIT_MB)  /// Giga byte
#define TINY_TOOLKIT_TB								(1024 * TINY_TOOLKIT_GB)  /// Tera byte
#define TINY_TOOLKIT_PB								(1024 * TINY_TOOLKIT_TB)  /// Peta byte
#define TINY_TOOLKIT_EB								(1024 * TINY_TOOLKIT_PB)  /// Exa byte
#define TINY_TOOLKIT_ZB								(1024 * TINY_TOOLKIT_EB)  /// Zetta byte
#define TINY_TOOLKIT_YB								(1024 * TINY_TOOLKIT_ZB)  /// Yotta byte
#define TINY_TOOLKIT_BB								(1024 * TINY_TOOLKIT_YB)  /// Bronto byte
#define TINY_TOOLKIT_NB								(1024 * TINY_TOOLKIT_BB)  /// Nona byte
#define TINY_TOOLKIT_DB								(1024 * TINY_TOOLKIT_NB)  /// Dogga byte


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  define TINY_TOOLKIT_EOL "\r\n"
#
#  define TINY_TOOLKIT_FILE __FILE__
#  define TINY_TOOLKIT_LINE __LINE__
#  define TINY_TOOLKIT_FUNC __FUNCTION__
#
#  define TINY_TOOLKIT_FOLDER_SEP "\\"
#  define TINY_TOOLKIT_FOLDER_EOL ".\\"
#
#else
#
#  define TINY_TOOLKIT_EOL "\n"
#
#  define TINY_TOOLKIT_FILE __FILE__
#  define TINY_TOOLKIT_LINE __LINE__
#  define TINY_TOOLKIT_FUNC __PRETTY_FUNCTION__
#
#  define TINY_TOOLKIT_FOLDER_SEP "/"
#  define TINY_TOOLKIT_FOLDER_EOL "./"
#
#endif


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  define TINY_TOOLKIT_PATH_MAX _MAX_PATH
#
#elif defined(PATH_MAX)
#
#  define TINY_TOOLKIT_PATH_MAX PATH_MAX
#
#elif defined(_XOPEN_PATH_MAX)
#
#  define TINY_TOOLKIT_PATH_MAX _XOPEN_PATH_MAX
#
#else
#
#  define TINY_TOOLKIT_PATH_MAX _POSIX_PATH_MAX
#
#endif


#endif // __TINY_TOOLKIT__COMMON__MACRO__H__
