#ifndef __TINY_TOOLKIT__LOG__FILTER__H__
#define __TINY_TOOLKIT__LOG__FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志过滤器
 *
 */


#include "event.h"
#include "transition.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ILogFilter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		ILogFilter() = default;

		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~ILogFilter() = default;

		/**
		 *
		 * 添加过滤器
		 *
		 * @param filter 过滤器
		 *
		 */
		void AddFilter(const std::shared_ptr<ILogFilter> & filter)
		{
			ILogFilter * end = this;

			while (end->Next())
			{
				end = end->Next().get();
			}

			end->SetNextFilter(filter);
		}

		/**
		 *
		 * 设置下一个过滤器
		 *
		 * @param filter 过滤器
		 *
		 */
		void SetNextFilter(const std::shared_ptr<ILogFilter> & filter)
		{
			_nextFilter = filter;
		}

		/**
		 *
		 * 过滤判断
		 *
		 * @param event 日志事件
		 *
		 * @return 是否过滤
		 *
		 */
		bool Decide(const LogEvent & event)
		{
			if (Filter(event))
			{
				return true;
			}

			if (Next())
			{
				return Next()->Decide(event);
			}

			return false;
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
		virtual bool Filter(const LogEvent & event) = 0;

		/**
		 *
		 * 下一个过滤器
		 *
		 * @return 过滤器
		 *
		 */
		const std::shared_ptr<ILogFilter> & Next() const
		{
			return _nextFilter;
		}

	protected:
		std::shared_ptr<ILogFilter> _nextFilter;
	};

	class TINY_TOOLKIT_API LogTimeFilter : public ILogFilter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param time 秒数时间戳
		 *
		 */
		explicit LogTimeFilter(std::time_t time)
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
		explicit LogTimeFilter(const ClockTimePoint & time)
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

	class TINY_TOOLKIT_API LogTimeRangeFilter : public ILogFilter
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
		explicit LogTimeRangeFilter(std::time_t head, std::time_t tail)
		{
			SetTime(head, tail);
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param head 时间点
		 * @param tail 时间点
		 *
		 */
		explicit LogTimeRangeFilter(const ClockTimePoint & head, const ClockTimePoint & tail)
		{
			SetTime(head, tail);
		}

		/**
		 *
		 * 设置时间
		 *
		 * @param head 时间点
		 * @param tail 时间点
		 *
		 */
		void SetTime(std::time_t head, std::time_t tail)
		{
			_time.first = tinyToolkit::Time::TimePoint(head);
			_time.second = tinyToolkit::Time::TimePoint(tail);
		}

		/**
		 *
		 * 设置时间
		 *
		 * @param head 时间点
		 * @param tail 时间点
		 *
		 */
		void SetTime(const ClockTimePoint & head, const ClockTimePoint & tail)
		{
			_time.first = head;
			_time.second = tail;
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
			return event.time < _time.first || _time.second < event.time;
		}

	protected:
		std::pair<ClockTimePoint, ClockTimePoint> _time;
	};

	class TINY_TOOLKIT_API LogRegexFilter : public ILogFilter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param rule 过滤规则
		 *
		 */
		explicit LogRegexFilter(const char * rule)
		{
			SetPriority(rule);
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param rule 过滤规则
		 *
		 */
		explicit LogRegexFilter(const std::regex & rule)
		{
			SetPriority(rule);
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param rule 过滤规则
		 *
		 */
		explicit LogRegexFilter(const std::string & rule)
		{
			SetPriority(rule);
		}

		/**
		 *
		 * 设置过滤规则
		 *
		 * @param rule 过滤规则
		 *
		 */
		void SetPriority(const char * rule)
		{
			_rule = rule;
		}

		/**
		 *
		 * 设置过滤规则
		 *
		 * @param rule 过滤规则
		 *
		 */
		void SetPriority(const std::regex & rule)
		{
			_rule = rule;
		}

		/**
		 *
		 * 设置过滤规则
		 *
		 * @param rule 过滤规则
		 *
		 */
		void SetPriority(const std::string & rule)
		{
			_rule = rule;
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
			return std::regex_match(event.message, _rule);
		}

	protected:
		std::regex _rule{ };
	};

	class TINY_TOOLKIT_API LogPriorityFilter : public ILogFilter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param priority 日志优先级
		 *
		 */
		explicit LogPriorityFilter(LOG_PRIORITY_TYPE priority)
		{
			SetPriority(priority);
		}

		/**
		 *
		 * 设置日志优先级
		 *
		 * @param priority 日志优先级
		 *
		 */
		void SetPriority(LOG_PRIORITY_TYPE priority)
		{
			_priority = priority;
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
			return _priority > event.priority;
		}

	protected:
		LOG_PRIORITY_TYPE _priority{ LOG_PRIORITY_TYPE::INFO };
	};

	class TINY_TOOLKIT_API LogPriorityRangeFilter : public ILogFilter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param head 日志优先级
		 * @param tail 日志优先级
		 *
		 */
		LogPriorityRangeFilter(LOG_PRIORITY_TYPE head, LOG_PRIORITY_TYPE tail)
		{
			SetPriority(head, tail);
		}

		/**
		 *
		 * 设置日志优先级
		 *
		 * @param head 日志优先级
		 * @param tail 日志优先级
		 *
		 */
		void SetPriority(LOG_PRIORITY_TYPE head, LOG_PRIORITY_TYPE tail)
		{
			_priority.first = head;
			_priority.second = tail;
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
			return event.priority < _priority.first || _priority.second < event.priority;
		}

	protected:
		std::pair<LOG_PRIORITY_TYPE, LOG_PRIORITY_TYPE> _priority{ LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL };
	};
};


#endif // __TINY_TOOLKIT__LOG__FILTER__H__
