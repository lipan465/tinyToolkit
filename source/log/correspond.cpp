/**
 *
 *  作者: hm
 *
 *  说明: 对应关系
 *
 */


#include "correspond.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <map>
#  include <stdexcept>
#  include <unordered_map>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <map>
#  include <unordered_map>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <map>
#  include <unordered_map>
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 星期
		 *
		 * @param name 名称
		 *
		 * @return 星期
		 *
		 */
		int32_t WeekCorrespond::Number(const std::string & name)
		{
			static std::unordered_map<std::string, int32_t> WeekNumber
			{
				{ "Sun", 0 },
				{ "Sunday", 0 },
				{ "Mon", 1 },
				{ "Monday", 1 },
				{ "Tue", 2 },
				{ "Tuesday", 2 },
				{ "Wed", 3 },
				{ "Wednesday", 3 },
				{ "Thu", 4 },
				{ "Thursday", 4 },
				{ "Fri", 5 },
				{ "Friday", 5 },
				{ "Sat", 6 },
				{ "Saturday", 6 },
			};

			auto find = WeekNumber.find(name);

			if (find == WeekNumber.end())
			{
				throw std::logic_error("The week name cannot be recognized : " + name);
			}

			return find->second;
		}

		/**
		 *
		 * 名称
		 *
		 * @param week 星期
		 *
		 * @return 名称
		 *
		 */
		const std::string & WeekCorrespond::LongName(int32_t week)
		{
			static std::unordered_map<int32_t, std::string> WeekName
			{
				{ 0, "Sunday", },
				{ 1, "Monday", },
				{ 2, "Tuesday", },
				{ 3, "Wednesday", },
				{ 4, "Thursday", },
				{ 5, "Friday", },
				{ 6, "Saturday", },
			};

			auto find = WeekName.find(week);

			if (find == WeekName.end())
			{
				throw std::logic_error("The week number cannot be recognized : " + std::to_string(week));
			}

			return find->second;
		}

		/**
		 *
		 * 名称
		 *
		 * @param week 星期
		 *
		 * @return 名称
		 *
		 */
		const std::string & WeekCorrespond::ShortName(int32_t week)
		{
			static std::unordered_map<int32_t, std::string> WeekName
			{
				{ 0, "Sun", },
				{ 1, "Mon", },
				{ 2, "Tue", },
				{ 3, "Wed", },
				{ 4, "Thu", },
				{ 5, "Fri", },
				{ 6, "Sat", },
			};

			auto find = WeekName.find(week);

			if (find == WeekName.end())
			{
				throw std::logic_error("The week number cannot be recognized : " + std::to_string(week));
			}

			return find->second;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 月份
		 *
		 * @param name 名称
		 *
		 * @return 月份
		 *
		 */
		int32_t MonthCorrespond::Number(const std::string & name)
		{
			static std::unordered_map<std::string, int32_t> MonthNumber
			{
				{ "Jan", 0 },
				{ "January", 0 },
				{ "Feb", 1 },
				{ "February", 1 },
				{ "Mar", 2 },
				{ "March", 2 },
				{ "Apr", 3 },
				{ "April", 3 },
				{ "May", 4 },
				{ "May", 4 },
				{ "Jun", 5 },
				{ "June", 5 },
				{ "Jul", 6 },
				{ "July", 6 },
				{ "Aug", 7 },
				{ "August", 7 },
				{ "Sep", 8 },
				{ "September", 8 },
				{ "Oct", 9 },
				{ "October", 9 },
				{ "Nov", 10 },
				{ "November", 10 },
				{ "Dec", 11 },
				{ "December", 11 },
			};

			auto find = MonthNumber.find(name);

			if (find == MonthNumber.end())
			{
				throw std::logic_error("The month name cannot be recognized : " + name);
			}

			return find->second;
		}

		/**
		 *
		 * 名称
		 *
		 * @param month 月份
		 *
		 * @return 名称
		 *
		 */
		const std::string & MonthCorrespond::LongName(int32_t month)
		{
			static std::unordered_map<int32_t, std::string> MonthName
			{
				{  0, "January", },
				{  1, "February", },
				{  2, "March", },
				{  3, "April", },
				{  4, "May", },
				{  5, "June", },
				{  6, "July", },
				{  7, "August", },
				{  8, "September", },
				{  9, "October", },
				{ 10, "November", },
				{ 11, "December", },
			};

			auto find = MonthName.find(month);

			if (find == MonthName.end())
			{
				throw std::logic_error("The month number cannot be recognized : " + std::to_string(month));
			}

			return find->second;
		}

		/**
		 *
		 * 名称
		 *
		 * @param month 月份
		 *
		 * @return 名称
		 *
		 */
		const std::string & MonthCorrespond::ShortName(int32_t month)
		{
			static std::unordered_map<int32_t, std::string> MonthName
			{
				{  0, "Jan", },
				{  1, "Feb", },
				{  2, "Mar", },
				{  3, "Apr", },
				{  4, "May", },
				{  5, "Jun", },
				{  6, "Jul", },
				{  7, "Aug", },
				{  8, "Sep", },
				{  9, "Oct", },
				{ 10, "Nov", },
				{ 11, "Dec", },
			};

			auto find = MonthName.find(month);

			if (find == MonthName.end())
			{
				throw std::logic_error("The month number cannot be recognized : " + std::to_string(month));
			}

			return find->second;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 优先级
		 *
		 * @param name 名称
		 *
		 * @return 优先级
		 *
		 */
		LOG_PRIORITY_TYPE PriorityCorrespond::Priority(const std::string & name)
		{
			static std::unordered_map<std::string, LOG_PRIORITY_TYPE> PriorityNumber
			{
				{ "DEBUG", LOG_PRIORITY_TYPE::DEBUGS },
				{ "INFO", LOG_PRIORITY_TYPE::INFO },
				{ "NOTICE", LOG_PRIORITY_TYPE::NOTICE },
				{ "WARNING", LOG_PRIORITY_TYPE::WARNING },
				{ "ERROR", LOG_PRIORITY_TYPE::ERRORS },
				{ "CRITICAL", LOG_PRIORITY_TYPE::CRITICAL },
				{ "ALERT", LOG_PRIORITY_TYPE::ALERT },
				{ "FATAL", LOG_PRIORITY_TYPE::FATAL },
				{ "EMERG", LOG_PRIORITY_TYPE::EMERG },
			};

			auto find = PriorityNumber.find(name);

			if (find == PriorityNumber.end())
			{
				throw std::logic_error("The priority name cannot be recognized : " + name);
			}

			return find->second;
		}

		/**
		 *
		 * 名称
		 *
		 * @param priority 优先级
		 *
		 * @return 名称
		 *
		 */
		const std::string & PriorityCorrespond::Name(LOG_PRIORITY_TYPE priority)
		{
			static std::map<LOG_PRIORITY_TYPE, std::string> PriorityName
			{
				{ LOG_PRIORITY_TYPE::DEBUGS,   "DEBUG" },
				{ LOG_PRIORITY_TYPE::INFO,     "INFO" },
				{ LOG_PRIORITY_TYPE::NOTICE,   "NOTICE" },
				{ LOG_PRIORITY_TYPE::WARNING,  "WARNING" },
				{ LOG_PRIORITY_TYPE::ERRORS,   "ERROR" },
				{ LOG_PRIORITY_TYPE::CRITICAL, "CRITICAL" },
				{ LOG_PRIORITY_TYPE::ALERT,    "ALERT" },
				{ LOG_PRIORITY_TYPE::FATAL,    "FATAL" },
				{ LOG_PRIORITY_TYPE::EMERG,    "EMERG" },
			};

			auto find = PriorityName.find(priority);

			if (find == PriorityName.end())
			{
				throw std::logic_error("The priority type cannot be recognized");
			}

			return find->second;
		}
	}
}
