/**
 *
 *  作者: hm
 *
 *  说明: 简单布局
 *
 */


#include "correspond.h"
#include "simpleLayout.h"

#include "../util/time.h"
#include "../util/string.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 格式化
		 *
		 * @param context 上下文
		 *
		 * @return 格式化后内容
		 *
		 */
		std::string SimpleLayout::Format(const Context & context)
		{
			return util::String::Format
			(
				"[{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:06}][{}][{:<8}] {}{}",
				context.tm.tm_year + 1900,
				context.tm.tm_mon + 1,
				context.tm.tm_mday,
				context.tm.tm_hour,
				context.tm.tm_min,
				context.tm.tm_sec,
				util::Time::Microseconds(context.time) % 1000000,
				context.name,
				PriorityCorrespond::Name(context.priority),
				context.content,
				TINY_TOOLKIT_EOL
			);
		}
	}
}
