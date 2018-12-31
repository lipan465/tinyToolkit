#ifndef __TINY_TOOLKIT__LOG__SINK__H__
#define __TINY_TOOLKIT__LOG__SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志节点
 *
 */


#include "filter.h"
#include "layout.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ILogSink : public std::enable_shared_from_this<ILogSink>
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param name 节点名称
		 *
		 */
		explicit ILogSink(std::string name) : _name(std::move(name))
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~ILogSink() = default;

		/**
		 *
		 * 关闭日志
		 *
		 */
		virtual void Close() = 0;

		/**
		 *
		 * 刷新日志
		 *
		 */
		virtual void Flush() = 0;

		/**
		 *
		 * 重新打开日志
		 *
		 */
		virtual void Reopen() = 0;

		/**
		 *
		 * 写入日志
		 *
		 * @param event 日志事件
		 *
		 */
		virtual void Write(const LogEvent & event) = 0;

		/**
		 *
		 * 是否自动刷新
		 *
		 * @return 是否自动刷新
		 *
		 */
		bool IsAutoFlush() const
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
		const std::string & Name() const
		{
			return _name;
		}

		/**
		 *
		 * 开启自动刷新
		 *
		 */
		std::shared_ptr<ILogSink> EnableAutoFlush()
		{
			_autoFlush = true;

			return shared_from_this();
		}

		/**
		 *
		 * 禁用自动刷新
		 *
		 */
		std::shared_ptr<ILogSink> DisableAutoFlush()
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
		std::shared_ptr<ILogSink> SetLayout(std::shared_ptr<ILogLayout> layout)
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
		std::shared_ptr<ILogSink> SetFilter(std::shared_ptr<ILogFilter> filter)
		{
			_filter = std::move(filter);

			return shared_from_this();
		}

		/**
		 * 添加设置日志过滤器
		 *
		 * @param filter 日志过滤器
		 *
		 */
		std::shared_ptr<ILogSink> AddFilter(std::shared_ptr<ILogFilter> filter)
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
		const std::shared_ptr<ILogLayout> & Layout() const
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
		const std::shared_ptr<ILogFilter> & Filter() const
		{
			return _filter;
		}

	protected:
		bool _autoFlush{ false };

		std::string _name{ };

		std::shared_ptr<ILogLayout> _layout;
		std::shared_ptr<ILogFilter> _filter;
	};
}


#endif // __TINY_TOOLKIT__LOG__SINK__H__
