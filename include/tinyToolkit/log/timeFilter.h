#ifndef __TINY_TOOLKIT__LOG__TIME_FILTER__H__
#define __TINY_TOOLKIT__LOG__TIME_FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志时间过滤器
 *
 */


#include "filter.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TimeLogFilter : public ILogFilter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param time 秒数时间戳
		 *
		 */
		explicit TimeLogFilter(std::time_t time)
		{
			SetTime(time);
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param time 时间点
		 *
		 */
		explicit TimeLogFilter(const ClockTimePoint & time)
		{
			SetTime(time);
		}

		/**
		 *
		 * 设置时间
		 *
		 * @param time 秒数时间戳
		 *
		 */
		void SetTime(std::time_t time)
		{
			_time = tinyToolkit::Time::TimePoint(time);
		}

		/**
		 *
		 * 设置时间
		 *
		 * @param time 时间点
		 *
		 */
		void SetTime(const ClockTimePoint & time)
		{
			_time = time;
		}

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
		bool Filter(const LogEvent & event) override
		{
			return _time > event.time;
		}

	protected:
		ClockTimePoint _time;
	};
};


#endif // __TINY_TOOLKIT__LOG__TIME_FILTER__H__
