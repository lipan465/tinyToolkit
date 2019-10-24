/**
 *
 *  作者: hm
 *
 *  说明: 日志节点
 *
 */


#include "sink.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param name 节点名称
	 *
	 */
	ILogSink::ILogSink(std::string name) : _name(std::move(name))
	{

	}

	/**
	 *
	 * 是否自动刷新
	 *
	 * @return 是否自动刷新
	 *
	 */
	bool ILogSink::IsAutoFlush() const
	{
		return _autoFlush;
	}

	/**
	 *
	 * 节点名称
	 *
	 * @return 节点名称
	 *
	 */
	const std::string & ILogSink::Name() const
	{
		return _name;
	}

	/**
	 *
	 * 开启自动刷新
	 *
	 */
	std::shared_ptr<ILogSink> ILogSink::EnableAutoFlush()
	{
		_autoFlush = true;

		return shared_from_this();
	}

	/**
	 *
	 * 禁用自动刷新
	 *
	 */
	std::shared_ptr<ILogSink> ILogSink::DisableAutoFlush()
	{
		_autoFlush = false;

		return shared_from_this();
	}

	/**
	 *
	 * 设置日志布局
	 *
	 * @param layout 日志布局
	 *
	 */
	std::shared_ptr<ILogSink> ILogSink::SetLayout(std::shared_ptr<ILogLayout> layout)
	{
		_layout = std::move(layout);

		return shared_from_this();
	}

	/**
	 *
	 * 设置日志过滤器
	 *
	 * @param filter 日志过滤器
	 *
	 */
	std::shared_ptr<ILogSink> ILogSink::SetFilter(std::shared_ptr<ILogFilter> filter)
	{
		_filter = std::move(filter);

		return shared_from_this();
	}

	/**
	 *
	 * 添加设置日志过滤器
	 *
	 * @param filter 日志过滤器
	 *
	 */
	std::shared_ptr<ILogSink> ILogSink::AddFilter(std::shared_ptr<ILogFilter> filter)
	{
		_filter->AddFilter(std::move(filter));

		return shared_from_this();
	}

	/**
	 *
	 * 日志布局
	 *
	 * @return 日志布局
	 *
	 */
	const std::shared_ptr<ILogLayout> & ILogSink::Layout() const
	{
		return _layout;
	}

	/**
	 *
	 * 日志过滤器
	 *
	 * @return 日志过滤器
	 *
	 */
	const std::shared_ptr<ILogFilter> & ILogSink::Filter() const
	{
		return _filter;
	}
}
