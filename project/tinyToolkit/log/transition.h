#ifndef __TINY_TOOLKIT__LOG__TRANSITION__H__
#define __TINY_TOOLKIT__LOG__TRANSITION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志类型处理
 *
 */


#include "detail.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API LogWeek
	{
	public:
		/**
		 *
		 * 星期数值
		 *
		 * @param week 星期名称
		 *
		 * @return 星期数值
		 *
		 */
		static int32_t Value(const char * week);

		/**
		 *
		 * 星期数值
		 *
		 * @param week 星期名称
		 *
		 * @return 星期数值
		 *
		 */
		static int32_t Value(const std::string & week);

		/**
		 *
		 * 星期缩略名称
		 *
		 * @param week 星期数值
		 *
		 * @return 星期缩略名称
		 *
		 */
		static std::string & Name(int32_t week);

		/**
		 *
		 * 星期名称
		 *
		 * @param week 星期数值
		 *
		 * @return 星期名称
		 *
		 */
		static std::string & SimpleName(int32_t week);
	};

	class TINY_TOOLKIT_API LogMonth
	{
	public:
		/**
		 *
		 * 月份数值
		 *
		 * @param month 月份名称
		 *
		 * @return 月份数值
		 *
		 */
		static int32_t Value(const char * month);

		/**
		 *
		 * 月份数值
		 *
		 * @param week 月份名称
		 *
		 * @return 月份数值
		 *
		 */
		static int32_t Value(const std::string & month);

		/**
		 *
		 * 月份缩略名称
		 *
		 * @param month 月份数值
		 *
		 * @return 月份缩略名称
		 *
		 */
		static std::string & Name(int32_t month);

		/**
		 *
		 * 月份名称
		 *
		 * @param month 月份数值
		 *
		 * @return 月份名称
		 *
		 */
		static std::string & SimpleName(int32_t month);
	};

	class TINY_TOOLKIT_API LogPriority
	{
	public:
		/**
		 *
		 * 优先级名称
		 *
		 * @param priority 优先级数值
		 *
		 * @return 优先级名称
		 *
		 */
		static std::string & Name(LOG_PRIORITY_TYPE priority);

		/**
		 *
		 * 优先级数值
		 *
		 * @param priority 优先级名称
		 *
		 * @return 优先级数值
		 *
		 */
		static LOG_PRIORITY_TYPE Value(const char * priority);

		/**
		 *
		 * 优先级数值
		 *
		 * @param priority 优先级名称
		 *
		 * @return 优先级数值
		 *
		 */
		static LOG_PRIORITY_TYPE Value(const std::string & priority);
	};
}


#endif // __TINY_TOOLKIT__LOG__TRANSITION__H__
