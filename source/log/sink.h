#ifndef __TINY_TOOLKIT__LOG__SINK__H__
#define __TINY_TOOLKIT__LOG__SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 节点
 *
 */


#include "filter.h"
#include "layout.h"


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API ISink : public std::enable_shared_from_this<ISink>
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param name 名称
			 *
			 */
			explicit ISink(std::string name);

			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~ISink() = default;

			/**
			 *
			 * 关闭节点
			 *
			 */
			virtual void Close() = 0;

			/**
			 *
			 * 刷新节点
			 *
			 */
			virtual void Flush() = 0;

			/**
			 *
			 * 写入内容
			 *
			 * @param context 上下文
			 *
			 */
			virtual void Write(const Context & context) = 0;

			/**
			 *
			 * 是否自动刷新
			 *
			 * @return 是否自动刷新
			 *
			 */
			bool IsAutoFlush();

			/**
			 *
			 * 开启自动刷新
			 *
			 * @return 节点
			 *
			 */
			std::shared_ptr<ISink> EnableAutoFlush();

			/**
			 *
			 * 禁用自动刷新
			 *
			 * @return 节点
			 *
			 */
			std::shared_ptr<ISink> DisableAutoFlush();

			/**
			 *
			 * 设置布局
			 *
			 * @param args 参数
			 *
			 * @return 节点
			 *
			 */
			template<class LayoutTypeT, typename... Args>
			std::shared_ptr<ISink> SetLayout(Args &&... args)
			{
				return SetLayout(std::make_shared<LayoutTypeT>(std::forward<Args>(args)...));
			}

			/**
			 *
			 * 设置日志过滤器
			 *
			 * @param args 参数
			 *
			 * @return 节点
			 *
			 */
			template<class FilterTypeT, typename... Args>
			std::shared_ptr<ISink> SetFilter(Args &&... args)
			{
				return SetFilter(std::make_shared<FilterTypeT>(std::forward<Args>(args)...));
			}

			/**
			 *
			 * 添加过滤器
			 *
			 * @param args 参数
			 *
			 * @return 节点
			 *
			 */
			template<class FilterTypeT, typename... Args>
			std::shared_ptr<ISink> AddFilter(Args &&... args)
			{
				return AddFilter(std::make_shared<FilterTypeT>(std::forward<Args>(args)...));
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
			std::shared_ptr<ISink> SetLayout(std::shared_ptr<ILayout> layout);

			/**
			 *
			 * 设置过滤器
			 *
			 * @param filter 过滤器
			 *
			 * @return 节点
			 *
			 */
			std::shared_ptr<ISink> SetFilter(std::shared_ptr<IFilter> filter);

			/**
			 *
			 * 添加过滤器
			 *
			 * @param filter 过滤器
			 *
			 * @return 节点
			 *
			 */
			std::shared_ptr<ISink> AddFilter(std::shared_ptr<IFilter> filter);

			/**
			 *
			 * 名称
			 *
			 * @return 名称
			 *
			 */
			const std::string & Name() const;

			/**
			 *
			 * 布局
			 *
			 * @return 布局
			 *
			 */
			const std::shared_ptr<ILayout> & Layout() const;

			/**
			 *
			 * 过滤器
			 *
			 * @return 过滤器
			 *
			 */
			const std::shared_ptr<IFilter> & Filter() const;

		private:
			bool _isAutoFlush{ false };

			std::string _name{ };

			std::shared_ptr<ILayout> _layout{ };
			std::shared_ptr<IFilter> _filter{ };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__SINK__H__
