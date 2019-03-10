/**
 *
 *  作者: hm
 *
 *  说明: 日志过滤器
 *
 */


#include "filter.h"


namespace tinyToolkit
{
	/**
	 *
	 * 过滤判断
	 *
	 * @param event 日志事件
	 *
	 * @return 是否过滤
	 *
	 */
	bool ILogFilter::Decide(const LogEvent & event)
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

	/**
	 *
	 * 添加过滤器
	 *
	 * @param filter 过滤器
	 *
	 */
	std::shared_ptr<ILogFilter> ILogFilter::AddFilter(std::shared_ptr<ILogFilter> filter)
	{
		ILogFilter * end = this;

		while (end->Next())
		{
			end = end->Next().get();
		}

		return end->SetNextFilter(std::move(filter));
	}

	/**
	 *
	 * 设置下一个过滤器
	 *
	 * @param filter 过滤器
	 *
	 */
	std::shared_ptr<ILogFilter> ILogFilter::SetNextFilter(std::shared_ptr<ILogFilter> filter)
	{
		_nextFilter = std::move(filter);

		return shared_from_this();
	}

	/**
	 *
	 * 下一个过滤器
	 *
	 * @return 过滤器
	 *
	 */
	const std::shared_ptr<ILogFilter> & ILogFilter::Next() const
	{
		return _nextFilter;
	}
}
