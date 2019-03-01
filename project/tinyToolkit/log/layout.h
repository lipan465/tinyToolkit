#ifndef __TINY_TOOLKIT__LOG__LAYOUT__H__
#define __TINY_TOOLKIT__LOG__LAYOUT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志布局
 *
 */


#include "event.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ILogLayout
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~ILogLayout() = default;

		/**
		 *
		 * 格式化日志
		 *
		 * @param event 日志事件
		 *
		 * @return 格式化信息
		 *
		 */
		virtual std::string Format(const LogEvent & event) = 0;
	};
}


#endif // __TINY_TOOLKIT__LOG__LAYOUT__H__
