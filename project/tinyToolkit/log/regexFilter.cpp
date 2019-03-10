/**
 *
 *  作者: hm
 *
 *  说明: 日志正则过滤器
 *
 */


#include "regexFilter.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param rule 过滤规则
	 *
	 */
	RegexLogFilter::RegexLogFilter(const std::regex & rule)
	{
		SetPriority(rule);
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param rule 过滤规则
	 *
	 */
	RegexLogFilter::RegexLogFilter(const char * rule)
	{
		SetPriority(rule);
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param rule 过滤规则
	 *
	 */
	RegexLogFilter::RegexLogFilter(const std::string & rule)
	{
		SetPriority(rule);
	}

	/**
	 *
	 * 设置过滤规则
	 *
	 * @param rule 过滤规则
	 *
	 */
	void RegexLogFilter::SetPriority(const std::regex & rule)
	{
		_rule = rule;
	}

	/**
	 *
	 * 设置过滤规则
	 *
	 * @param rule 过滤规则
	 *
	 */
	void RegexLogFilter::SetPriority(const char * rule)
	{
		assert(rule);

		_rule = rule;
	}

	/**
	 *
	 * 设置过滤规则
	 *
	 * @param rule 过滤规则
	 *
	 */
	void RegexLogFilter::SetPriority(const std::string & rule)
	{
		_rule = rule;
	}

	/**
	 *
	 * 过滤处理
	 *
	 * @param event 日志事件
	 *
	 * @return 是否过滤
	 *
	 */
	bool RegexLogFilter::Filter(const LogEvent & event)
	{
		return std::regex_match(event.message, _rule);
	}
}
