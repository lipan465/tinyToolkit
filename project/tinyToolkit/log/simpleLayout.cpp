/**
 *
 *  作者: hm
 *
 *  说明: 日志简单布局
 *
 */


#include "transition.h"
#include "simpleLayout.h"

#include "../utilities/time.h"
#include "../utilities/string.h"


namespace tinyToolkit
{
	/**
	 *
	 * 格式化日志
	 *
	 * @param event 日志事件
	 *
	 * @return 格式化信息
	 *
	 */
	std::string SimpleLogLayout::Format(const LogEvent & event)
	{
		return String::Format
		(
			"[{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:06}][{}][{:<8}] {}{}",
			event.tm.tm_year + 1900,
			event.tm.tm_mon + 1,
			event.tm.tm_mday,
			event.tm.tm_hour,
			event.tm.tm_min,
			event.tm.tm_sec,
			Time::Microseconds(event.time) % 1000000,
			event.name,
			LogPriority::Name(event.priority),
			event.message,
			TINY_TOOLKIT_EOL
		);
	}
}
