#ifndef __TINY_TOOLKIT__LOG__PRIORITY_FILTER__H__
#define __TINY_TOOLKIT__LOG__PRIORITY_FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志优先级过滤器
 *
 */


#include "filter.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API PriorityLogFilter : public ILogFilter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param priority 日志优先级
		 *
		 */
		explicit PriorityLogFilter(LOG_PRIORITY_TYPE priority)
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
};


#endif // __TINY_TOOLKIT__LOG__PRIORITY_FILTER__H__
