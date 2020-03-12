#ifndef __TINY_TOOLKIT__LOG__CORRESPOND__H__
#define __TINY_TOOLKIT__LOG__CORRESPOND__H__


/**
 *
 *  作者: hm
 *
 *  说明: 对应关系
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
	namespace log
	{
		class TINY_TOOLKIT_API WeekCorrespond
		{
		public:
			/**
			 *
			 * 星期
			 *
			 * @param name 名称
			 *
			 * @return 星期
			 *
			 */
			static int32_t Number(const std::string & name);

			/**
			 *
			 * 名称
			 *
			 * @param week 星期
			 *
			 * @return 名称
			 *
			 */
			static const std::string & LongName(int32_t week);

			/**
			 *
			 * 名称
			 *
			 * @param week 星期
			 *
			 * @return 名称
			 *
			 */
			static const std::string & ShortName(int32_t week);
		};

		class TINY_TOOLKIT_API MonthCorrespond
		{
		public:
			/**
			 *
			 * 月份
			 *
			 * @param name 名称
			 *
			 * @return 月份
			 *
			 */
			static int32_t Number(const std::string & name);

			/**
			 *
			 * 名称
			 *
			 * @param month 月份
			 *
			 * @return 名称
			 *
			 */
			static const std::string & LongName(int32_t month);

			/**
			 *
			 * 名称
			 *
			 * @param month 月份
			 *
			 * @return 名称
			 *
			 */
			static const std::string & ShortName(int32_t month);
		};

		class TINY_TOOLKIT_API PriorityCorrespond
		{
		public:
			/**
			 *
			 * 优先级
			 *
			 * @param name 名称
			 *
			 * @return 优先级
			 *
			 */
			static LOG_PRIORITY_TYPE Priority(const std::string & name);

			/**
			 *
			 * 名称
			 *
			 * @param priority 优先级
			 *
			 * @return 名称
			 *
			 */
			static const std::string & Name(LOG_PRIORITY_TYPE priority);
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__CORRESPOND__H__
