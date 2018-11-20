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
	class TINY_TOOLKIT_API ILogSink
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
		 * 设置日志布局
		 *
		 * @param layout 日志布局
		 *
		 */
		void SetLayout(const std::shared_ptr<ILogLayout> & layout)
		{
			_layout = layout;
		}

		/**
		 *
		 * 设置日志过滤器
		 *
		 * @param filter 日志过滤器
		 *
		 */
		void SetFilter(const std::shared_ptr<ILogFilter> & filter)
		{
			_filter = filter;
		}

		/**
		 *
		 * 开启自动刷新
		 *
		 */
		void EnableAutoFlush()
		{
			_autoFlush = true;
		}

		/**
		 *
		 * 禁用自动刷新
		 *
		 */
		void DisableAutoFlush()
		{
			_autoFlush = false;
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
