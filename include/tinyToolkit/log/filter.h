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
};


#endif // __TINY_TOOLKIT__LOG__FILTER__H__
