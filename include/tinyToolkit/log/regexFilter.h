#ifndef __TINY_TOOLKIT__LOG__REGEX_FILTER__H__
#define __TINY_TOOLKIT__LOG__REGEX_FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志正则过滤器
 *
 */


#include "filter.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API RegexLogFilter : public ILogFilter
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param rule 过滤规则
		 *
		 */
		explicit RegexLogFilter(const std::regex & rule);

		/**
		 *
		 * 构造函数
		 *
		 * @param rule 过滤规则
		 *
		 */
		explicit RegexLogFilter(const char * rule);

		/**
		 *
		 * 构造函数
		 *
		 * @param rule 过滤规则
		 *
		 */
		explicit RegexLogFilter(const std::string & rule);

		/**
		 *
		 * 设置过滤规则
		 *
		 * @param rule 过滤规则
		 *
		 */
		void SetPriority(const std::regex & rule);

		/**
		 *
		 * 设置过滤规则
		 *
		 * @param rule 过滤规则
		 *
		 */
		void SetPriority(const char * rule);

		/**
		 *
		 * 设置过滤规则
		 *
		 * @param rule 过滤规则
		 *
		 */
		void SetPriority(const std::string & rule);

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
		std::regex _rule{ };
	};
};


#endif // __TINY_TOOLKIT__LOG__REGEX_FILTER__H__
