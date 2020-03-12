#ifndef __TINY_TOOLKIT__COMMON__PLATFORM__H__
#define __TINY_TOOLKIT__COMMON__PLATFORM__H__


/**
 *
 *  作者: hm
 *
 *  说明: 平台信息
 *
 */


#define TINY_TOOLKIT_PLATFORM_LINUX		1
#define TINY_TOOLKIT_PLATFORM_APPLE		2
#define TINY_TOOLKIT_PLATFORM_WINDOWS	3


#if defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32 ) || defined( _WIN32_ ) || defined( __WIN32__ ) || \
	defined( WIN64 ) || defined( _WIN64 ) || defined( __WIN64 ) || defined( _WIN64_ ) || defined( __WIN64__ )
#
#  define TINY_TOOLKIT_PLATFORM TINY_TOOLKIT_PLATFORM_WINDOWS
#
#elif defined( APPLE ) || defined( __APPLE ) || defined( __APPLE__ )
#
#  define TINY_TOOLKIT_PLATFORM TINY_TOOLKIT_PLATFORM_APPLE
#
#elif defined( __linux__ )
#
#  define TINY_TOOLKIT_PLATFORM TINY_TOOLKIT_PLATFORM_LINUX
#
#else
#
#  error "platform not supported"
#
#endif


#endif // __TINY_TOOLKIT__COMMON__PLATFORM__H__
