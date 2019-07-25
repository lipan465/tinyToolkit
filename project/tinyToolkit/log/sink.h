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
		explicit ILogSink(std::string name);

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
		bool IsAutoFlush() const;

		/**
		 *
		 * 节点名称
		 *
		 * @return 节点名称
		 *
		 */
		const std::string & Name() const;

		/**
		 *
		 * 开启自动刷新
		 *
		 */
		std::shared_ptr<ILogSink> EnableAutoFlush();

		/**
		 *
		 * 禁用自动刷新
		 *
		 */
		std::shared_ptr<ILogSink> DisableAutoFlush();

		/**
		 *
		 * 设置日志布局
		 *
		 * @tparam LayoutTypeT [layout types]
		 * @tparam Args [all types]
		 *
		 * @param args 参数
		 *
		 */
		template<class LayoutTypeT, typename... Args>
		std::shared_ptr<ILogSink> SetLayout(Args &&... args)
		{
			_layout = std::make_shared<LayoutTypeT>(std::forward<Args>(args)...);

			return shared_from_this();
		}

		/**
		 *
		 * 设置日志过滤器
		 *
		 * @tparam FilterTypeT [layout types]
		 * @tparam Args [all types]
		 *
		 * @param args 参数
		 *
		 */
		template<class FilterTypeT, typename... Args>
		std::shared_ptr<ILogSink> SetFilter(Args &&... args)
		{
			_filter = std::make_shared<FilterTypeT>(std::forward<Args>(args)...);

			return shared_from_this();
		}

		/**
		 *
		 * 添加设置日志过滤器
		 *
		 * @tparam FilterTypeT [layout types]
		 * @tparam Args [all types]
		 *
		 * @param args 参数
		 *
		 */
		template<class FilterTypeT, typename... Args>
		std::shared_ptr<ILogSink> AddFilter(Args &&... args)
		{
			auto filter = std::make_shared<FilterTypeT>(std::forward<Args>(args)...);

			_filter->AddFilter(std::move(filter));

			return shared_from_this();
		}

		/**
		 *
		 * 设置日志布局
		 *
		 * @param layout 日志布局
		 *
		 */
		std::shared_ptr<ILogSink> SetLayout(std::shared_ptr<ILogLayout> layout);

		/**
		 *
		 * 设置日志过滤器
		 *
		 * @param filter 日志过滤器
		 *
		 */
		std::shared_ptr<ILogSink> SetFilter(std::shared_ptr<ILogFilter> filter);

		/**
		 * 添加设置日志过滤器
		 *
		 * @param filter 日志过滤器
		 *
		 */
		std::shared_ptr<ILogSink> AddFilter(std::shared_ptr<ILogFilter> filter);

		/**
		 *
		 * 日志布局
		 *
		 * @return 日志布局
		 *
		 */
		const std::shared_ptr<ILogLayout> & Layout() const;

		/**
		 *
		 * 日志过滤器
		 *
		 * @return 日志过滤器
		 *
		 */
		const std::shared_ptr<ILogFilter> & Filter() const;

	protected:
		bool _autoFlush{ false };

		std::string _name{ };

		std::shared_ptr<ILogLayout> _layout{ };
		std::shared_ptr<ILogFilter> _filter{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__SINK__H__
