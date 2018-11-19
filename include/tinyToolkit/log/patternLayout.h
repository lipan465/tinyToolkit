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
#include "transition.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API LogPatternLayout : public ILogLayout
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param pattern 规则
		 *
		 */
		explicit LogPatternLayout(const std::string & pattern)
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param pattern 规则
		 *
		 */
		explicit LogPatternLayout(const char * pattern) : _pattern(pattern)
		{

		}

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
			std::string value = { };

			auto end = _pattern.end();

			for (auto iter = _pattern.begin(); iter != end; ++iter)
			{
				if (*iter == '%')
				{
					if (++iter != end)
					{
						Format(event, value, *iter);
					}
					else
					{
						break;
					}
				}
				else
				{
					value.push_back(*iter);
				}
			}

			return value;
		}

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
		void Format(const LogEvent & event, std::string & value, char flag)
		{
			switch (flag)
			{
				case 'a':  /// 当前区域星期名简写 (Sun..Sat)
				{
					value += LogWeek::SimpleName(event.tm.tm_wday);

					break;
				}

				case 'A':  /// 当前区域星期名全称 (Sunday..Saturday)
				{
					value += LogWeek::Name(event.tm.tm_wday);

					break;
				}

				case 'b':  /// 当前区域月份名简写 (Jan..Dec)
				{
					value += LogMonth::SimpleName(event.tm.tm_mon);

					break;
				}

				case 'B':  /// 当前区域月份名全称 (January..December)
				{
					value += LogMonth::Name(event.tm.tm_mon);

					break;
				}

				case 'c':  /// 当前区域的日期和时间 (2018-01-01 00:00:00.000000)
				{
					value += tinyToolkit::String::Format
					(
						"{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:06}",
						event.tm.tm_year + 1900,
						event.tm.tm_mon + 1,
						event.tm.tm_mday,
						event.tm.tm_hour,
						event.tm.tm_min,
						event.tm.tm_sec,
						tinyToolkit::Time::Microseconds(event.time) % 1000000
					);

					break;
				}

				case 'C':  /// 世纪
				{
					value += std::to_string((event.tm.tm_year + 1900) / 100);
				}

				case 'd':  /// 按月计的日期 (01..31)
				{
					value += tinyToolkit::String::Format("{:02}", event.tm.tm_mday);

					break;
				}

				case 'D':  /// 按月计的日期 (mm/dd/yy)
				{
					value += tinyToolkit::String::Format("{:02}{:02}{:02}", event.tm.tm_mon + 1, event.tm.tm_mday, event.tm.tm_year % 100);

					break;
				}

				case 'e':  /// 按月计的日期, 添加空格 ( 1..31)
				{
					value += tinyToolkit::String::Format("{:2}", event.tm.tm_mday);

					break;
				}

				case 'E':  /// 微秒 (000000-999999)
				{
					value += tinyToolkit::String::Format("{:06}", tinyToolkit::Time::Nanoseconds(event.time) % 1000000);

					break;
				}

				case 'f':  /// 毫秒 (000-999)
				{
					value += tinyToolkit::String::Format("{:03}", tinyToolkit::Time::Milliseconds(event.time) % 1000);

					break;
				}

				case 'F':  /// 完整日期格式 (2018-01-01)
				{
					value += tinyToolkit::String::Format
					(
						"{:04}-{:02}-{:02}",
						event.tm.tm_year + 1900,
						event.tm.tm_mon + 1,
						event.tm.tm_mday
					);

					break;
				}

				case 'g':  /// 格式年份的最后两位
				{
					value += std::to_string(event.tm.tm_year % 100);

					break;
				}

				case 'G':  /// 格式年份
				{
					value += std::to_string(event.tm.tm_year + 1900);

					break;
				}

				case 'h':  /// 当前区域月份名简写 (Jan..Dec)
				{
					value += LogMonth::SimpleName(event.tm.tm_mon);

					break;
				}

				case 'H':  /// 小时 (00..23)
				{
					value += tinyToolkit::String::Format("{:02}", event.tm.tm_hour);

					break;
				}

				case 'i':  /// 线程序列号
				{
					value += std::to_string(event.threadID);

					break;
				}

				case 'I':  /// 小时 (01..12)
				{
					value += tinyToolkit::String::Format("{:02}", event.tm.tm_hour > 12 ? event.tm.tm_hour - 12 : event.tm.tm_hour);

					break;
				}

				case 'j':  /// 按年计的日期 (001..366)
				{
					value += tinyToolkit::String::Format("{:03}", event.tm.tm_yday + 1);

					break;
				}

				case 'J':  /// 日志名称
				{
					value += event.name;

					break;
				}

				case 'k':  /// 小时 (0..23)
				{
					value += std::to_string(event.tm.tm_hour);

					break;
				}

				case 'K':  /// 进程序列号
				{
					value += std::to_string(event.processID);

					break;
				}

				case 'l':  /// 小时 (1..12)
				{
					value += std::to_string(event.tm.tm_hour > 12 ? event.tm.tm_hour - 12 : event.tm.tm_hour);

					break;
				}

				case 'L':  /// 优先级
				{
					value += LogPriority::Name(event.priority);

					break;
				}

				case 'm':  /// 月份 (01..12)
				{
					value += tinyToolkit::String::Format("{:02}", event.tm.tm_mon + 1);

					break;
				}

				case 'M':  /// 分钟 (00..59)
				{
					value += tinyToolkit::String::Format("{:02}", event.tm.tm_min);

					break;
				}

				case 'n': /// 换行
				{
					value += TINY_TOOLKIT_EOL;

					break;
				}

				case 'N': /// 纳秒 (000000000-999999999)
				{
					value += tinyToolkit::String::Format("{:09}", tinyToolkit::Time::Nanoseconds(event.time) % 1000000000);

					break;
				}

				case 'p':  /// 当前时间是AM还是PM
				{
					value += event.tm.tm_hour >= 12 ? "PM" : "AM";

					break;
				}

				case 'P':  /// 当前时间是am还是pm
				{
					value += event.tm.tm_hour >= 12 ? "pm" : "am";

					break;
				}

				case 'r':  /// 12小时时钟时间 (hh:mm:ss [AM/PM])
				{
					value += tinyToolkit::String::Format
					(
						"{:02}:{:02}:{:02} {}",
						event.tm.tm_hour,
						event.tm.tm_min,
						event.tm.tm_sec,
						event.tm.tm_hour >= 12 ? "PM" : "AM"
					);

					break;
				}

				case 'R':  /// 24小时时间的时和分 (hh:mm)
				{
					value += tinyToolkit::String::Format
					(
						"{:02}:{:02}",
						event.tm.tm_hour,
						event.tm.tm_min
					);

					break;
				}

				case 's':  /// 自UTC时间 1970-01-01 00:00:00 以来所经过的秒数 (GNU扩充)
				{
					value += std::to_string(tinyToolkit::Time::Seconds(event.time));

					break;
				}

				case 'S':  /// 秒数 (00..60)
				{
					value += tinyToolkit::String::Format("{:02}", event.tm.tm_sec);

					break;
				}

				case 't':  /// 输出制表符Tab
				{
					value += "\t";

					break;
				}

				case 'T':  /// 时间 (hh:mm:ss)
				{
					value += tinyToolkit::String::Format
					(
						"{:02}:{:02}:{:02}",
						event.tm.tm_hour,
						event.tm.tm_min,
						event.tm.tm_sec
					);

					break;
				}

				case 'U':  /// 一年中的第几周, 以周日为每星期第一天 (00-53)
				{
					break;
				}

				case 'v':  /// 输入的日志信息
				{
					value += event.message;

					break;
				}

				case 'V':  /// ISO-8601格式规范下的一年中第几周, 以周一为每星期第一天 (01-53)
				{
					break;
				}

				case 'w':  /// 一星期中的第几日(0-6), 0代表周一
				{
					value += std::to_string(event.tm.tm_wday);

					break;
				}

				case 'W':  ///  一年中的第几周, 以周一为每星期第一天 (00-53)
				{
					break;
				}

				case 'x':  /// 日期描述 (mm/dd/yy)
				{
					value += tinyToolkit::String::Format("{:02}{:02}{:02}", event.tm.tm_mon + 1, event.tm.tm_mday, event.tm.tm_year % 100);

					break;
				}

				case 'X':  /// 时间描述 (hh:mm:ss)
				{
					value += tinyToolkit::String::Format
					(
						"{:02}:{:02}:{:02}",
						event.tm.tm_hour,
						event.tm.tm_min,
						event.tm.tm_sec
					);

					break;
				}

				case 'y':  /// 年份最后两位数位 (00..99)
				{
					value += std::to_string(event.tm.tm_year % 100);

					break;
				}

				case 'Y':  /// 年份 (1970...)
				{
					value += std::to_string(event.tm.tm_year + 1900);

					break;
				}

				case 'z':  /// 数字时区
				{
					break;
				}

				case 'Z':  /// 按字母表排序的时区缩写
				{
					break;
				}

				case '+': /// 完整格式化
				{
					value += tinyToolkit::String::Format
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

					break;
				}

				default: /// 其余格式化
				{
					value.push_back('%');
					value.push_back(flag);

					break;
				}
			}
		}

	protected:
		std::string _pattern{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__PATTERN_LAYOUT__H__
