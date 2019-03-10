#ifndef __TINY_TOOLKIT__LOG__PATTERN_LAYOUT__H__
#define __TINY_TOOLKIT__LOG__PATTERN_LAYOUT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志模式布局
 *
 */


#include "layout.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API PatternLogLayout : public ILogLayout
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param pattern 规则
		 *
		 */
		explicit PatternLogLayout(std::string pattern);

		/**
		 *
		 * 格式化日志
		 *
		 * @param event 日志事件
		 *
		 * @return 格式化信息
		 *
		 */
		std::string Format(const LogEvent & event) override;

	protected:
		/**
		 *
		 * 格式化日志
		 *
		 * @param event 日志事件
		 * @param value 信息记录
		 * @param flag 格式化标志
		 *
		 */
		void Format(const LogEvent & event, std::string & value, char flag);

	protected:
		std::string _pattern{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__PATTERN_LAYOUT__H__
