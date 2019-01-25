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
		explicit PriorityLogFilter(const char * priority);

		/**
		 *
		 * 构造函数
		 *
		 * @param priority 日志优先级
		 *
		 */
		explicit PriorityLogFilter(const std::string & priority);

		/**
		 *
		 * 构造函数
		 *
		 * @param priority 日志优先级
		 *
		 */
		explicit PriorityLogFilter(LOG_PRIORITY_TYPE priority);

		/**
		 *
		 * 设置日志优先级
		 *
		 * @param priority 日志优先级
		 *
		 */
		void SetPriority(const char * priority);

		/**
		 *
		 * 设置日志优先级
		 *
		 * @param priority 日志优先级
		 *
		 */
		void SetPriority(const std::string & priority);

		/**
		 *
		 * 设置日志优先级
		 *
		 * @param priority 日志优先级
		 *
		 */
		void SetPriority(LOG_PRIORITY_TYPE priority);

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
		bool Filter(const LogEvent & event) override;

	protected:
		LOG_PRIORITY_TYPE _priority{ LOG_PRIORITY_TYPE::INFO };
	};
};


#endif // __TINY_TOOLKIT__LOG__PRIORITY_FILTER__H__
