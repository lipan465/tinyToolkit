#ifndef __TINY_TOOLKIT__LOG__CONTEXT__H__
#define __TINY_TOOLKIT__LOG__CONTEXT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 上下文
 *
 */


#include "type.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <chrono>
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <chrono>
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <chrono>
#  include <string>
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		struct TINY_TOOLKIT_API Context
		{
			std::tm tm{ };

			uint64_t threadID{ 0 };
			uint64_t processID{ 0 };

			std::string name{ };
			std::string content{ };

			LOG_OPTION_TYPE option{ LOG_OPTION_TYPE::WRITE };
			LOG_PRIORITY_TYPE priority{ LOG_PRIORITY_TYPE::INFO };

			std::chrono::system_clock::time_point time{ };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__CONTEXT__H__
