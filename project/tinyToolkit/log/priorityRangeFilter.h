#ifndef __TINY_TOOLKIT__LOG__PRIORITY_RANGE_FILTER__H__
#define __TINY_TOOLKIT__LOG__PRIORITY_RANGE_FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志优先级范围过滤器
 *
 */


#include "filter.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API PriorityRangeLogFilter : public ILogFilter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param head 日志优先级
		 * @param tail 日志优先级
		 *
		 */
		PriorityRangeLogFilter(LOG_PRIORITY_TYPE head, LOG_PRIORITY_TYPE tail);

		/**
		 *
		 * 设置日志优先级
		 *
		 * @param head 日志优先级
		 * @param tail 日志优先级
		 *
		 */
		void SetPriority(const char * head, const char * tail);

		/**
		 *
		 * 设置日志优先级
		 *
		 * @param head 日志优先级
		 * @param tail 日志优先级
		 *
		 */
		void SetPriority(LOG_PRIORITY_TYPE head, LOG_PRIORITY_TYPE tail);

		/**
		 *
		 * 设置日志优先级
		 *
		 * @param head 日志优先级
		 * @param tail 日志优先级
		 *
		 */
		void SetPriority(const std::string & head, const std::string & tail);

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
		std::pair<LOG_PRIORITY_TYPE, LOG_PRIORITY_TYPE> _priority{ LOG_PRIORITY_TYPE::INFO, LOG_PRIORITY_TYPE::FATAL };
	};
};


#endif // __TINY_TOOLKIT__LOG__PRIORITY_RANGE_FILTER__H__
