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


#ifndef TINY_TOOLKIT_PI
#define TINY_TOOLKIT_PI									3.1415926535898f
#endif

#ifndef TINY_TOOLKIT_SECOND
#define TINY_TOOLKIT_SECOND								1
#endif

#ifndef TINY_TOOLKIT_MINUTE
#define TINY_TOOLKIT_MINUTE								(60 * TINY_TOOLKIT_SECOND)
#endif

#ifndef TINY_TOOLKIT_HOUR
#define TINY_TOOLKIT_HOUR								(60 * TINY_TOOLKIT_MINUTE)
#endif

#ifndef TINY_TOOLKIT_DAY
#define TINY_TOOLKIT_DAY								(24 * TINY_TOOLKIT_HOUR)
#endif

#ifndef TINY_TOOLKIT_WEEK
#define TINY_TOOLKIT_WEEK								(7 * TINY_TOOLKIT_DAY)
#endif

#ifndef TINY_TOOLKIT_YEAR
#define TINY_TOOLKIT_YEAR								(12 * TINY_TOOLKIT_WEEK)
#endif

#ifndef TINY_TOOLKIT_BYTE
#define TINY_TOOLKIT_BYTE								1
#endif

#ifndef TINY_TOOLKIT_KB
#define TINY_TOOLKIT_KB									(1024 * TINY_TOOLKIT_BYTE)
#endif

#ifndef TINY_TOOLKIT_MB
#define TINY_TOOLKIT_MB									(1024 * TINY_TOOLKIT_KB)
#endif

#ifndef TINY_TOOLKIT_GB
#define TINY_TOOLKIT_GB									(1024 * TINY_TOOLKIT_MB)
#endif

#ifndef TINY_TOOLKIT_TB
#define TINY_TOOLKIT_TB									(1024 * TINY_TOOLKIT_GB)
#endif

#ifndef TINY_TOOLKIT_PB
#define TINY_TOOLKIT_PB									(1024 * TINY_TOOLKIT_TB)
#endif

#ifndef TINY_TOOLKIT_EB
#define TINY_TOOLKIT_EB									(1024 * TINY_TOOLKIT_PB)
#endif

#ifndef TINY_TOOLKIT_ZB
#define TINY_TOOLKIT_ZB									(1024 * TINY_TOOLKIT_EB)
#endif

#ifndef TINY_TOOLKIT_YB
#define TINY_TOOLKIT_YB									(1024 * TINY_TOOLKIT_ZB)
#endif

#ifndef TINY_TOOLKIT_BB
#define TINY_TOOLKIT_BB									(1024 * TINY_TOOLKIT_YB)
#endif

#ifndef TINY_TOOLKIT_NB
#define TINY_TOOLKIT_NB									(1024 * TINY_TOOLKIT_BB)
#endif

#ifndef TINY_TOOLKIT_DB
#define TINY_TOOLKIT_DB									(1024 * TINY_TOOLKIT_NB)
#endif

#ifndef TINY_TOOLKIT_NET_COUNT
#define TINY_TOOLKIT_NET_COUNT							10240
#endif

#ifndef HMAC_PAD_64_SIZE
#define HMAC_PAD_64_SIZE								64
#endif

#ifndef HMAC_PAD_128_SIZE
#define HMAC_PAD_128_SIZE								128
#endif

#ifndef MD5_BLOCK_SIZE
#define MD5_BLOCK_SIZE									32
#endif

#ifndef MD5_DIGEST_SIZE
#define MD5_DIGEST_SIZE									16
#endif

#ifndef SHA1_BLOCK_SIZE
#define SHA1_BLOCK_SIZE									32
#endif

#ifndef SHA1_DIGEST_SIZE
#define SHA1_DIGEST_SIZE								20
#endif

#ifndef SHA224_BLOCK_SIZE
#define SHA224_BLOCK_SIZE								64
#endif

#ifndef SHA224_DIGEST_SIZE
#define SHA224_DIGEST_SIZE								28
#endif

#ifndef SHA256_BLOCK_SIZE
#define SHA256_BLOCK_SIZE								64
#endif

#ifndef SHA256_DIGEST_SIZE
#define SHA256_DIGEST_SIZE								32
#endif

#ifndef SHA384_BLOCK_SIZE
#define SHA384_BLOCK_SIZE								128
#endif

#ifndef SHA384_DIGEST_SIZE
#define SHA384_DIGEST_SIZE								48
#endif

#ifndef SHA512_BLOCK_SIZE
#define SHA512_BLOCK_SIZE								128
#endif

#ifndef SHA512_DIGEST_SIZE
#define SHA512_DIGEST_SIZE								64
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
#  define TINY_TOOLKIT_NAME_MAX							_MAX_FNAME
#  endif
#
#  ifndef TINY_TOOLKIT_PATH_MAX
#  define TINY_TOOLKIT_PATH_MAX							_MAX_PATH
#  endif
#
#else
#
#  include <climits>
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


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  ifndef TINY_TOOLKIT_SOCKET_TYPE
#  define TINY_TOOLKIT_SOCKET_TYPE						SOCKET
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_ERROR
#  define TINY_TOOLKIT_SOCKET_ERROR						SOCKET_ERROR
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_HANDLE
#  define TINY_TOOLKIT_SOCKET_HANDLE					HANDLE
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_INVALID
#  define TINY_TOOLKIT_SOCKET_INVALID					INVALID_SOCKET
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_TEMP_SIZE
#  define TINY_TOOLKIT_SOCKET_TEMP_SIZE					1460
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_LISTEN_COUNT
#  define TINY_TOOLKIT_SOCKET_LISTEN_COUNT				2048
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_HANDLE_INVALID
#  define TINY_TOOLKIT_SOCKET_HANDLE_INVALID			nullptr
#  endif
#
#else
#
#  ifndef TINY_TOOLKIT_SOCKET_TYPE
#  define TINY_TOOLKIT_SOCKET_TYPE						int32_t
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_ERROR
#  define TINY_TOOLKIT_SOCKET_ERROR						-1
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_HANDLE
#  define TINY_TOOLKIT_SOCKET_HANDLE					int32_t
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_INVALID
#  define TINY_TOOLKIT_SOCKET_INVALID					-1
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_TEMP_SIZE
#  define TINY_TOOLKIT_SOCKET_TEMP_SIZE					1460
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_LISTEN_COUNT
#  define TINY_TOOLKIT_SOCKET_LISTEN_COUNT				2048
#  endif
#
#  ifndef TINY_TOOLKIT_SOCKET_HANDLE_INVALID
#  define TINY_TOOLKIT_SOCKET_HANDLE_INVALID			-1
#  endif
#
#endif


#endif // __TINY_TOOLKIT__COMMON__MACRO__H__
