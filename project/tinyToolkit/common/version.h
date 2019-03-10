#ifndef __TINY_TOOLKIT__COMMON__VERSION__H__
#define __TINY_TOOLKIT__COMMON__VERSION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 版本信息
 *
 */


#ifdef DEBUG
#
#  define TINY_TOOLKIT_FLAG "DEBUG"
#
#else
#
#  define TINY_TOOLKIT_FLAG "RELEASE"
#
#endif


#define TINY_TOOLKIT_MAJOR				"0"
#define TINY_TOOLKIT_MINOR				"0"
#define TINY_TOOLKIT_REVISION			"1"
#define TINY_TOOLKIT_VERSION			TINY_TOOLKIT_FLAG "_" TINY_TOOLKIT_MAJOR "." TINY_TOOLKIT_MINOR "." TINY_TOOLKIT_REVISION
#define TINY_TOOLKIT_VERSION_STRING		"tinyToolkit/" TINY_TOOLKIT_VERSION


#endif // __TINY_TOOLKIT__COMMON__VERSION__H__
