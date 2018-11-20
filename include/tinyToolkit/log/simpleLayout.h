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
#include "transition.h"


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
		std::string Format(const LogEvent & event) override
		{
			return tinyToolkit::String::Format
			(
				"[{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:06}] [{}] [{}] {}",
				event.tm.tm_year + 1900,
				event.tm.tm_mon + 1,
				event.tm.tm_mday,
				event.tm.tm_hour,
				event.tm.tm_min,
				event.tm.tm_sec,
				tinyToolkit::Time::Microseconds(event.time) % 1000000,
				event.name,
				LogPriority::Name(event.priority),
				event.message
			);
		}
	};
}


#endif // __TINY_TOOLKIT__LOG__SIMPLE_LAYOUT__H__
