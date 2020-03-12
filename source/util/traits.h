#ifndef __TINY_TOOLKIT__UTIL__TRAITS__H__
#define __TINY_TOOLKIT__UTIL__TRAITS__H__


/**
 *
 *  作者: hm
 *
 *  说明: 萃取处理
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <type_traits>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <type_traits>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <type_traits>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		template <typename T, typename... Args>
		struct is_with_type : std::false_type
		{

		};

		template <typename T, typename U, typename... Args>
		struct is_with_type<T, U, Args...> : std::conditional_t<std::is_same<T, U>::value, std::true_type, is_with_type<T, Args...>>
		{

		};
	}
}


#endif // __TINY_TOOLKIT__UTIL__TRAITS__H__
