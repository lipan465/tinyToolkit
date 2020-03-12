/**
 *
 *  作者: hm
 *
 *  说明: 节点
 *
 */


#include "sink.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 *
		 */
		ISink::ISink(std::string name) : _name(std::move(name))
		{

		}

		/**
		 *
		 * 是否自动刷新
		 *
		 * @return 是否自动刷新
		 *
		 */
		bool ISink::IsAutoFlush()
		{
			return _isAutoFlush;
		}

		/**
		 *
		 * 开启自动刷新
		 *
		 * @return 节点
		 *
		 */
		std::shared_ptr<ISink> ISink::EnableAutoFlush()
		{
			_isAutoFlush = true;

			return shared_from_this();
		}

		/**
		 *
		 * 禁用自动刷新
		 *
		 * @return 节点
		 *
		 */
		std::shared_ptr<ISink> ISink::DisableAutoFlush()
		{
			_isAutoFlush = false;

			return shared_from_this();
		}

		/**
		 *
		 * 设置布局
		 *
		 * @param layout 布局
		 *
		 * @return 节点
		 *
		 */
		std::shared_ptr<ISink> ISink::SetLayout(std::shared_ptr<ILayout> layout)
		{
			_layout = std::move(layout);

			return shared_from_this();
		}

		/**
		 *
		 * 设置过滤器
		 *
		 * @param filter 过滤器
		 *
		 * @return 节点
		 *
		 */
		std::shared_ptr<ISink> ISink::SetFilter(std::shared_ptr<IFilter> filter)
		{
			_filter = std::move(filter);

			return shared_from_this();
		}

		/**
		 *
		 * 添加过滤器
		 *
		 * @param filter 过滤器
		 *
		 * @return 节点
		 *
		 */
		std::shared_ptr<ISink> ISink::AddFilter(std::shared_ptr<IFilter> filter)
		{
			if (_filter)
			{
				_filter->AddFilter(std::move(filter));
			}
			else
			{
				_filter = std::move(filter);
			}

			return shared_from_this();
		}

		/**
		 *
		 * 名称
		 *
		 * @return 名称
		 *
		 */
		const std::string & ISink::Name() const
		{
			return _name;
		}

		/**
		 *
		 * 布局
		 *
		 * @return 布局
		 *
		 */
		const std::shared_ptr<ILayout> & ISink::Layout() const
		{
			return _layout;
		}

		/**
		 *
		 * 过滤器
		 *
		 * @return 过滤器
		 *
		 */
		const std::shared_ptr<IFilter> & ISink::Filter() const
		{
			return _filter;
		}
	}
}
