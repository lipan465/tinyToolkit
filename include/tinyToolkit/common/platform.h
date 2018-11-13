#ifndef __TINY_TOOLKIT__COMMON__PLATFORM__H__
#define __TINY_TOOLKIT__COMMON__PLATFORM__H__


/**
 *
 *  作者: hm
 *
 *  说明: 平台信息
 *
 */


#define TINY_TOOLKIT_PLATFORM_UNIX		1
#define TINY_TOOLKIT_PLATFORM_LINUX		2
#define TINY_TOOLKIT_PLATFORM_APPLE		3
#define TINY_TOOLKIT_PLATFORM_INTEL		4
#define TINY_TOOLKIT_PLATFORM_SOLARIS	5
#define TINY_TOOLKIT_PLATFORM_WINDOWS	6
#define TINY_TOOLKIT_PLATFORM_FREE_BSD	7


#if defined( __FreeBSD__ )
#
#  define TINY_TOOLKIT_PLATFORM TINY_TOOLKIT_PLATFORM_FREE_BSD
#
#elif defined( __sun )
#
#  define TINY_TOOLKIT_PLATFORM TINY_TOOLKIT_PLATFORM_SOLARIS
#
#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32 ) || defined( _WIN32_ ) || defined( __WIN32__ ) || \
	  defined( WIN64 ) || defined( _WIN64 ) || defined( __WIN64 ) || defined( _WIN64_ ) || defined( __WIN64__ )
#
#  define TINY_TOOLKIT_PLATFORM TINY_TOOLKIT_PLATFORM_WINDOWS
#
#elif defined( APPLE ) || defined( __APPLE ) || defined( __APPLE__ )
#
#  define TINY_TOOLKIT_PLATFORM TINY_TOOLKIT_PLATFORM_APPLE
#
#elif defined( INTEL_COMPILER ) || defined( __INTEL_COMPILER ) || defined( __INTEL_COMPILER__ )
#
#  define TINY_TOOLKIT_PLATFORM TINY_TOOLKIT_PLATFORM_INTEL
#
#elif defined( __linux__ )
#
#  define TINY_TOOLKIT_PLATFORM TINY_TOOLKIT_PLATFORM_LINUX
#
#else
#
#  define TINY_TOOLKIT_PLATFORM TINY_TOOLKIT_PLATFORM_UNIX
#
#endif


#endif // __TINY_TOOLKIT__COMMON__PLATFORM__H__
