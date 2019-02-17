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


/**
 *
 * pi
 *
 */
#ifndef TINY_TOOLKIT_PI
#
#  define TINY_TOOLKIT_PI								(3.1415926535898f)
#
#endif

/**
 *
 * second
 *
 */
#ifndef TINY_TOOLKIT_SECOND
#
#  define TINY_TOOLKIT_SECOND							(1)
#
#endif

/**
 *
 * minute
 *
 */
#ifndef TINY_TOOLKIT_MINUTE
#
#  define TINY_TOOLKIT_MINUTE							(60 * TINY_TOOLKIT_SECOND)
#
#endif

/**
 *
 * hour
 *
 */
#ifndef TINY_TOOLKIT_HOUR
#
#  define TINY_TOOLKIT_HOUR								(60 * TINY_TOOLKIT_MINUTE)
#
#endif

/**
 *
 * day
 *
 */
#ifndef TINY_TOOLKIT_DAY
#
#  define TINY_TOOLKIT_DAY								(24 * TINY_TOOLKIT_HOUR)
#
#endif

/**
 *
 * week
 *
 */
#ifndef TINY_TOOLKIT_WEEK
#
#  define TINY_TOOLKIT_WEEK								(7 * TINY_TOOLKIT_DAY)
#
#endif

/**
 *
 * year
 *
 */
#ifndef TINY_TOOLKIT_YEAR
#
#  define TINY_TOOLKIT_YEAR								(12 * TINY_TOOLKIT_WEEK)
#
#endif

/**
 *
 * Byte
 *
 */
#ifndef TINY_TOOLKIT_BYTE
#
#  define TINY_TOOLKIT_BYTE								(1)
#
#endif

/**
 *
 * Kilobyte
 *
 */
#ifndef TINY_TOOLKIT_KB
#
#  define TINY_TOOLKIT_KB								(1024 * TINY_TOOLKIT_BYTE)
#
#endif

/**
 *
 * Mega byte
 *
 */
#ifndef TINY_TOOLKIT_MB
#
#  define TINY_TOOLKIT_MB								(1024 * TINY_TOOLKIT_KB)
#
#endif

/**
 *
 * Giga byte
 *
 */
#ifndef TINY_TOOLKIT_GB
#
#  define TINY_TOOLKIT_GB								(1024 * TINY_TOOLKIT_MB)
#
#endif

/**
 *
 * Tera byte
 *
 */
#ifndef TINY_TOOLKIT_TB
#
#  define TINY_TOOLKIT_TB								(1024 * TINY_TOOLKIT_GB)
#
#endif

/**
 *
 * Peta byte
 *
 */
#ifndef TINY_TOOLKIT_PB
#
#  define TINY_TOOLKIT_PB								(1024 * TINY_TOOLKIT_TB)
#
#endif

/**
 *
 * Exa byte
 *
 */
#ifndef TINY_TOOLKIT_EB
#
#  define TINY_TOOLKIT_EB								(1024 * TINY_TOOLKIT_PB)
#
#endif

/**
 *
 * Zetta byte
 *
 */
#ifndef TINY_TOOLKIT_ZB
#
#  define TINY_TOOLKIT_ZB								(1024 * TINY_TOOLKIT_EB)
#
#endif

/**
 *
 * Yotta byte
 *
 */
#ifndef TINY_TOOLKIT_YB
#
#  define TINY_TOOLKIT_YB								(1024 * TINY_TOOLKIT_ZB)
#
#endif

/**
 *
 * Bronto byte
 *
 */
#ifndef TINY_TOOLKIT_BB
#
#  define TINY_TOOLKIT_BB								(1024 * TINY_TOOLKIT_YB)
#
#endif

/**
 *
 * Nona byte
 *
 */
#ifndef TINY_TOOLKIT_NB
#
#  define TINY_TOOLKIT_NB								(1024 * TINY_TOOLKIT_BB)
#
#endif

/**
 *
 * Dogga byte
 *
 */
#ifndef TINY_TOOLKIT_DB
#
#  define TINY_TOOLKIT_DB								(1024 * TINY_TOOLKIT_NB)
#
#endif

/**
 *
 * 网络句柄个数
 *
 */
#ifndef TINY_TOOLKIT_NET_COUNT
#define TINY_TOOLKIT_NET_COUNT							10240
#endif


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  ifndef TINY_TOOLKIT_EOL
#  define TINY_TOOLKIT_EOL								"\r\n"
#  endif
#
#  ifndef TINY_TOOLKIT_FILE
#  define TINY_TOOLKIT_FILE								__FILE__
#  endif
#
#  ifndef TINY_TOOLKIT_LINE
#  define TINY_TOOLKIT_LINE								__LINE__
#  endif
#
#  ifndef TINY_TOOLKIT_FUNC
#  define TINY_TOOLKIT_FUNC								__FUNCTION__
#  endif
#
#  ifndef TINY_TOOLKIT_FOLDER_SEP
#  define TINY_TOOLKIT_FOLDER_SEP						"\\"
#  endif
#
#  ifndef TINY_TOOLKIT_FOLDER_EOL
#  define TINY_TOOLKIT_FOLDER_EOL						".\\"
#  endif
#
#else
#
#  ifndef TINY_TOOLKIT_EOL
#  define TINY_TOOLKIT_EOL								"\n"
#  endif
#
#  ifndef TINY_TOOLKIT_FILE
#  define TINY_TOOLKIT_FILE								__FILE__
#  endif
#
#  ifndef TINY_TOOLKIT_LINE
#  define TINY_TOOLKIT_LINE								__LINE__
#  endif
#
#  ifndef TINY_TOOLKIT_FUNC
#  define TINY_TOOLKIT_FUNC								__PRETTY_FUNCTION__
#  endif
#
#  ifndef TINY_TOOLKIT_FOLDER_SEP
#  define TINY_TOOLKIT_FOLDER_SEP						"/"
#  endif
#
#  ifndef TINY_TOOLKIT_FOLDER_EOL
#  define TINY_TOOLKIT_FOLDER_EOL						"./"
#  endif
#
#endif


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  ifndef TINY_TOOLKIT_NAME_MAX
#  define TINY_TOOLKIT_NAME_MAX							_MAX_NAME
#  endif
#
#  ifndef TINY_TOOLKIT_PATH_MAX
#  define TINY_TOOLKIT_PATH_MAX							_MAX_PATH
#  endif
#
#else
#
#  include <limits.h>
#
#  ifndef TINY_TOOLKIT_NAME_MAX
#  define TINY_TOOLKIT_NAME_MAX							NAME_MAX
#  endif
#
#  ifndef TINY_TOOLKIT_PATH_MAX
#  define TINY_TOOLKIT_PATH_MAX							PATH_MAX
#  endif
#
#endif


#endif // __TINY_TOOLKIT__COMMON__MACRO__H__
