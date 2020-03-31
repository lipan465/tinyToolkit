#ifndef __TINY_TOOLKIT__LOG__REGEX_FILTER__H__
#define __TINY_TOOLKIT__LOG__REGEX_FILTER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 正则过滤器
 *
 */


#include "filter.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <regex>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <regex>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <regex>
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API RegexFilter : public IFilter
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param rule 规则
			 *
			 */
			explicit RegexFilter(std::regex rule);

			/**
			 *
			 * 构造函数
			 *
			 * @param rule 规则
			 *
			 */
			explicit RegexFilter(const std::string & rule);

		private:
			/**
			 *
			 * 过滤处理
			 *
			 * @param context 上下文
			 *
			 * @return 是否过滤
			 *
			 */
			bool Filter(const Context & context) override;

		private:
			std::regex _rule{ };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__REGEX_FILTER__H__
