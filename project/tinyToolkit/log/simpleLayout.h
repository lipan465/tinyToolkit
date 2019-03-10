#ifndef __TINY_TOOLKIT__LOG__SIMPLE_LAYOUT__H__
#define __TINY_TOOLKIT__LOG__SIMPLE_LAYOUT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志简单布局
 *
 */


#include "layout.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API SimpleLogLayout : public ILogLayout
	{
	public:
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
	};
}


#endif // __TINY_TOOLKIT__LOG__SIMPLE_LAYOUT__H__
