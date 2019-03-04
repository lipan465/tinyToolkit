#ifndef __TINY_TOOLKIT__COMMON__SUPPORT__H__
#define __TINY_TOOLKIT__COMMON__SUPPORT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 支持信息
 *
 */


#include "platform.h"
#include "compiler.h"


#ifndef NDEBUG
#
#	ifndef _DEBUG
#
#		define _DEBUG
#
#	endif
#
#	ifndef DEBUG
#
#		define DEBUG
#
#	endif
#
#   ifndef _GLIBCXX_DEBUG
#
#       define _GLIBCXX_DEBUG
#
#   endif
#
#endif


#ifdef DEBUG
#
#  define TINY_TOOLKIT_INLINE
#
#else
#
#  define TINY_TOOLKIT_INLINE inline
#
#endif


/**
 *
 * DECLSPEC_NORETURN 告诉编译器, 这个函数不会返回, 让编译器知道调用约定为__declspec(noreturn)的函数之后的代码不可到达
 *
 * DECLSPEC_DEPRECATED 说明一个函数, 类型, 或别的标识符在新的版本或未来版本中不再支持, 你不应该用这个函数或类型
 *
 */
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  if !defined(TINY_TOOLKIT_DECLSPEC_NORETURN)
#
#    define TINY_TOOLKIT_DECLSPEC_NORETURN __declspec(noreturn)
#
#  endif
#
#  if !defined(TINY_TOOLKIT_DECLSPEC_DEPRECATED)
#
#    define TINY_TOOLKIT_DECLSPEC_DEPRECATED __declspec(deprecated)
#
#  endif
#
#else
#
#  define TINY_TOOLKIT_DECLSPEC_NORETURN
#  define TINY_TOOLKIT_DECLSPEC_DEPRECATED
#
#endif


/**
 *
 * noreturn
 *
 * 该属性通知编译器函数从不返回值
 *
 *
 * deprecated
 *
 * 该属性标识预期在程序的新的版本或未来版本中不再支持
 *
 *
 * format (archetype, string-index, first-to-check)
 *
 * archetype      : 指定是哪种风格 (printf scanf strftime strfmon)
 * string-index   : 指定传入函数的第几个参数是格式化字符串
 * first-to-check : 指定从函数的第几个参数开始按上述规则进行检查
 *
 * 该属性可以使编译器检查函数声明和函数实际调用参数之间的格式化字符串是否匹配
 *
 */
#if TINY_TOOLKIT_COMPILER == TINY_TOOLKIT_COMPILER_GNU
#
#  define TINY_TOOLKIT_ATTR_NORETURN __attribute__((noreturn))
#  define TINY_TOOLKIT_ATTR_DEPRECATED __attribute__((deprecated))
#  define TINY_TOOLKIT_ATTR_PRINTF(index, check) __attribute__((format(printf, index, check)))
#
#else
#
#  define TINY_TOOLKIT_ATTR_NORETURN
#  define TINY_TOOLKIT_ATTR_DEPRECATED
#  define TINY_TOOLKIT_ATTR_PRINTF(index, check)
#
#endif


#if TINY_TOOLKIT_COMPILER == TINY_TOOLKIT_COMPILER_VC
#
#  define TINY_TOOLKIT_API_EXPORT __declspec(dllexport)
#  define TINY_TOOLKIT_API_IMPORT __declspec(dllimport)
#
#elif TINY_TOOLKIT_COMPILER == TINY_TOOLKIT_COMPILER_GNU
#
#  define TINY_TOOLKIT_API_EXPORT __attribute__((visibility("default")))
#  define TINY_TOOLKIT_API_IMPORT
#
#else
#
#  error "compiler not supported!"
#
#endif


#ifdef TINY_TOOLKIT_API_NOT_COMMON
#
#  define TINY_TOOLKIT_API
#
#elif defined(TINY_TOOLKIT_API_IMPORT_COMMON)
#
#  define TINY_TOOLKIT_API TINY_TOOLKIT_API_IMPORT
#
#else
#
#  define TINY_TOOLKIT_API TINY_TOOLKIT_API_EXPORT
#
#endif


#endif // __TINY_TOOLKIT__COMMON__SUPPORT__H__
