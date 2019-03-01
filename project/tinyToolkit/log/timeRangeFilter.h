#ifndef __TINY_TOOLKIT__LOG__TIME_RANGE_FILTER__H__
#define __TINY_TOOLKIT__LOG__TIME_RANGE_FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志时间范围过滤器
 *
 */


#include "filter.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TimeRangeLogFilter : public ILogFilter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param head 时间点
		 * @param tail 时间点
		 *
		 */
		explicit TimeRangeLogFilter(std::time_t head, std::time_t tail);

		/**
		 *
		 * 构造函数
		 *
		 * @param head 时间点
		 * @param tail 时间点
		 *
		 */
		explicit TimeRangeLogFilter(const ClockTimePoint & head, const ClockTimePoint & tail);

		/**
		 *
		 * 设置时间
		 *
		 * @param head 时间点
		 * @param tail 时间点
		 *
		 */
		void SetTime(std::time_t head, std::time_t tail);

		/**
		 *
		 * 设置时间
		 *
		 * @param head 时间点
		 * @param tail 时间点
		 *
		 */
		void SetTime(const ClockTimePoint & head, const ClockTimePoint & tail);

	protected:
		/**
		 *
		 * 过滤处理
		 *
		 * @param event 日志事件
		 *
		 * @return 是否过滤
		 *
		 */
		bool Filter(const LogEvent & event) override;

	protected:
		std::pair<ClockTimePoint, ClockTimePoint> _time{ };
	};
};


#endif // __TINY_TOOLKIT__LOG__TIME_RANGE_FILTER__H__
