#ifndef __TINY_TOOLKIT__OPTION__TRAITS__H__
#define __TINY_TOOLKIT__OPTION__TRAITS__H__


/**
 *
 *  作者: hm
 *
 *  说明: 萃取
 *
 */


#include "type.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <string>
#
#endif


namespace tinyToolkit
{
	namespace option
	{
		template<typename TypeT>
		struct TINY_TOOLKIT_API Traits
		{
			const static OPTION_TYPE type = OPTION_TYPE::UNKNOWN;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<bool>
		{
			const static OPTION_TYPE type = OPTION_TYPE::BOOL;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<float>
		{
			const static OPTION_TYPE type = OPTION_TYPE::FLOAT;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<double>
		{
			const static OPTION_TYPE type = OPTION_TYPE::DOUBLE;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<long double>
		{
			const static OPTION_TYPE type = OPTION_TYPE::LONG_DOUBLE;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<int8_t>
		{
			const static OPTION_TYPE type = OPTION_TYPE::INT8;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<uint8_t>
		{
			const static OPTION_TYPE type = OPTION_TYPE::UINT8;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<int16_t>
		{
			const static OPTION_TYPE type = OPTION_TYPE::INT16;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<uint16_t>
		{
			const static OPTION_TYPE type = OPTION_TYPE::UINT16;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<int32_t>
		{
			const static OPTION_TYPE type = OPTION_TYPE::INT32;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<uint32_t>
		{
			const static OPTION_TYPE type = OPTION_TYPE::UINT32;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<int64_t>
		{
			const static OPTION_TYPE type = OPTION_TYPE::INT64;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<uint64_t>
		{
			const static OPTION_TYPE type = OPTION_TYPE::UINT64;
		};

		template<>
		struct TINY_TOOLKIT_API Traits<std::string>
		{
			const static OPTION_TYPE type = OPTION_TYPE::STRING;
		};
	}
}


#endif // __TINY_TOOLKIT__OPTION__TRAITS__H__
