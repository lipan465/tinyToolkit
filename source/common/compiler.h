#ifndef __TINY_TOOLKIT__COMMON__COMPILER__H__
#define __TINY_TOOLKIT__COMMON__COMPILER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 编译器信息
 *
 */


#define TINY_TOOLKIT_COMPILER_VC 			1
#define TINY_TOOLKIT_COMPILER_GNU       	2


#if defined( _MSC_VER )
#
#  define TINY_TOOLKIT_COMPILER TINY_TOOLKIT_COMPILER_VC
#
#elif defined( __GNUC__ )
#
#  define TINY_TOOLKIT_COMPILER TINY_TOOLKIT_COMPILER_GNU
#
#else
#
#  error "compiler not supported"
#
#endif


#endif // __TINY_TOOLKIT__COMMON__COMPILER__H__
