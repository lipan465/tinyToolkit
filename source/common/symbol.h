#ifndef __TINY_TOOLKIT__COMMON__SYMBOL__H__
#define __TINY_TOOLKIT__COMMON__SYMBOL__H__


/**
 *
 *  作者: hm
 *
 *  说明: 符号信息
 *
 */


#include "compiler.h"


#if TINY_TOOLKIT_COMPILER == TINY_TOOLKIT_COMPILER_VC
#
#  pragma warning(disable:4251)
#  pragma warning(disable:4275)
#
#  define TINY_TOOLKIT_SYMBOL_EXPORT __declspec(dllexport)
#  define TINY_TOOLKIT_SYMBOL_IMPORT __declspec(dllimport)
#
#elif TINY_TOOLKIT_COMPILER == TINY_TOOLKIT_COMPILER_GNU
#
#  define TINY_TOOLKIT_SYMBOL_EXPORT __attribute__((visibility("default")))
#  define TINY_TOOLKIT_SYMBOL_IMPORT
#
#else
#
#  define TINY_TOOLKIT_SYMBOL_EXPORT
#  define TINY_TOOLKIT_SYMBOL_IMPORT
#
#endif


#ifdef TINY_TOOLKIT_API_NULL
#
#  define TINY_TOOLKIT_API
#
#elif defined(TINY_TOOLKIT_API_IMPORT)
#
#  define TINY_TOOLKIT_API TINY_TOOLKIT_SYMBOL_IMPORT
#
#else
#
#  define TINY_TOOLKIT_API TINY_TOOLKIT_SYMBOL_EXPORT
#
#endif


#endif // __TINY_TOOLKIT__COMMON__SYMBOL__H__
