/**
 *
 *  作者: hm
 *
 *  说明: 模式布局
 *
 */


#include "correspond.h"
#include "patternLayout.h"

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
		 * @param storage 存储
		 * @param value 内容
		 *
		 */
		void FormatPattern(const Context & context, std::string & storage, char value)
		{
			switch (value)
			{
				case 'a':  /// 当前区域星期名简写 (Sun..Sat)
				{
					storage += WeekCorrespond::ShortName(context.tm.tm_wday);

					break;
				}

				case 'A':  /// 当前区域星期名全称 (Sunday..Saturday)
				{
					storage += WeekCorrespond::LongName(context.tm.tm_wday);

					break;
				}

				case 'b':  /// 当前区域月份名简写 (Jan..Dec)
				{
					storage += MonthCorrespond::ShortName(context.tm.tm_mon);

					break;
				}

				case 'B':  /// 当前区域月份名全称 (January..December)
				{
					storage += MonthCorrespond::LongName(context.tm.tm_mon);

					break;
				}

				case 'c':  /// 当前区域的日期和时间 (2018-01-01 00:00:00.000000)
				{
					storage += util::String::Format
					(
						"{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:06}",
						context.tm.tm_year + 1900,
						context.tm.tm_mon + 1,
						context.tm.tm_mday,
						context.tm.tm_hour,
						context.tm.tm_min,
						context.tm.tm_sec,
						util::Time::Microseconds(context.time) % 1000000
					);

					break;
				}

				case 'C':  /// 世纪
				{
					storage += std::to_string((context.tm.tm_year + 1900) / 100);

					break;
				}

				case 'd':  /// 按月计的日期 (01..31)
				{
					storage += util::String::Format
					(
						"{:02}",
						context.tm.tm_mday
					);

					break;
				}

				case 'D':  /// 按月计的日期 (mm/dd/yy)
				{
					storage += util::String::Format
					(
						"{:02}{:02}{:02}",
						context.tm.tm_mon + 1,
						context.tm.tm_mday,
						context.tm.tm_year % 100
					);

					break;
				}

				case 'e':  /// 按月计的日期, 添加空格 ( 1..31)
				{
					storage += util::String::Format
					(
						"{:2}",
						context.tm.tm_mday
					);

					break;
				}

				case 'E':  /// 微秒 (000000-999999)
				{
					storage += util::String::Format
					(
						"{:06}",
						util::Time::Nanoseconds(context.time) % 1000000
					);

					break;
				}

				case 'f':  /// 毫秒 (000-999)
				{
					storage += util::String::Format
					(
						"{:03}",
						util::Time::Milliseconds(context.time) % 1000
					);

					break;
				}

				case 'F':  /// 完整日期格式 (2018-01-01)
				{
					storage += util::String::Format
					(
						"{:04}-{:02}-{:02}",
						context.tm.tm_year + 1900,
						context.tm.tm_mon + 1,
						context.tm.tm_mday
					);

					break;
				}

				case 'g':  /// 格式年份的最后两位
				{
					storage += std::to_string(context.tm.tm_year % 100);

					break;
				}

				case 'G':  /// 格式年份
				{
					storage += std::to_string(context.tm.tm_year + 1900);

					break;
				}

				case 'h':  /// 当前区域月份名简写 (Jan..Dec)
				{
					storage += MonthCorrespond::ShortName(context.tm.tm_mon);

					break;
				}

				case 'H':  /// 小时 (00..23)
				{
					storage += util::String::Format
					(
						"{:02}",
						context.tm.tm_hour
					);

					break;
				}

				case 'i':  /// 线程序列号
				{
					storage += std::to_string(context.threadID);

					break;
				}

				case 'I':  /// 小时 (01..12)
				{
					storage += util::String::Format
					(
						"{:02}",
						context.tm.tm_hour > 12 ? context.tm.tm_hour - 12 :
						context.tm.tm_hour
					);

					break;
				}

				case 'j':  /// 按年计的日期 (001..366)
				{
					storage += util::String::Format
					(
						"{:03}",
						context.tm.tm_yday + 1
					);

					break;
				}

				case 'J':  /// 日志名称
				{
					storage += context.name;

					break;
				}

				case 'k':  /// 小时 (0..23)
				{
					storage += std::to_string(context.tm.tm_hour);

					break;
				}

				case 'K':  /// 进程序列号
				{
					storage += std::to_string(context.processID);

					break;
				}

				case 'l':  /// 小时 (1..12)
				{
					storage += std::to_string
					(
						context.tm.tm_hour > 12 ? context.tm.tm_hour - 12 :
						context.tm.tm_hour
					);

					break;
				}

				case 'L':  /// 优先级
				{
					storage += PriorityCorrespond::Name(context.priority);

					break;
				}

				case 'm':  /// 月份 (01..12)
				{
					storage += util::String::Format
					(
						"{:02}",
						context.tm.tm_mon + 1
					);

					break;
				}

				case 'M':  /// 分钟 (00..59)
				{
					storage += util::String::Format
					(
						"{:02}",
						context.tm.tm_min
					);

					break;
				}

				case 'n': /// 换行
				{
					storage += TINY_TOOLKIT_EOL;

					break;
				}

				case 'N': /// 纳秒 (000000000-999999999)
				{
					storage += util::String::Format
					(
						"{:09}",
						util::Time::Nanoseconds(context.time) % 1000000000
					);

					break;
				}

				case 'p':  /// 当前时间是AM还是PM
				{
					storage += context.tm.tm_hour >= 12 ? "PM" : "AM";

					break;
				}

				case 'P':  /// 当前时间是am还是pm
				{
					storage += context.tm.tm_hour >= 12 ? "pm" : "am";

					break;
				}

				case 'r':  /// 12小时时钟时间 (hh:mm:ss [AM/PM])
				{
					storage += util::String::Format
					(
						"{:02}:{:02}:{:02} {}",
						context.tm.tm_hour,
						context.tm.tm_min,
						context.tm.tm_sec,
						context.tm.tm_hour >= 12 ? "PM" : "AM"
					);

					break;
				}

				case 'R':  /// 24小时时间的时和分 (hh:mm)
				{
					storage += util::String::Format
					(
						"{:02}:{:02}",
						context.tm.tm_hour,
						context.tm.tm_min
					);

					break;
				}

				case 's':  /// 自UTC时间 1970-01-01 00:00:00 以来所经过的秒数 (GNU扩充)
				{
					storage += std::to_string(util::Time::Seconds(context.time));

					break;
				}

				case 'S':  /// 秒数 (00..60)
				{
					storage += util::String::Format
					(
						"{:02}",
						context.tm.tm_sec
					);

					break;
				}

				case 't':  /// 输出制表符Tab
				{
					storage += "\t";

					break;
				}

				case 'T':  /// 时间 (hh:mm:ss)
				{
					storage += util::String::Format
					(
						"{:02}:{:02}:{:02}",
						context.tm.tm_hour,
						context.tm.tm_min,
						context.tm.tm_sec
					);

					break;
				}

				case 'U':  /// 一年中的第几周, 以周日为每星期第一天 (00-53)
				{
					/// todo

					break;
				}

				case 'v':  /// 输入的日志信息
				{
					storage += context.content;

					break;
				}

				case 'V':  /// ISO-8601格式规范下的一年中第几周, 以周一为每星期第一天 (01-53)
				{
					/// todo

					break;
				}

				case 'w':  /// 一星期中的第几日(0-6), 0代表周一
				{
					storage += std::to_string(context.tm.tm_wday);

					break;
				}

				case 'W':  ///  一年中的第几周, 以周一为每星期第一天 (00-53)
				{
					/// todo

					break;
				}

				case 'x':  /// 日期描述 (mm/dd/yy)
				{
					storage += util::String::Format
					(
						"{:02}{:02}{:02}",
						context.tm.tm_mon + 1,
						context.tm.tm_mday,
						context.tm.tm_year % 100
					);

					break;
				}

				case 'X':  /// 时间描述 (hh:mm:ss)
				{
					storage += util::String::Format
					(
						"{:02}:{:02}:{:02}",
						context.tm.tm_hour,
						context.tm.tm_min,
						context.tm.tm_sec
					);

					break;
				}

				case 'y':  /// 年份最后两位数位 (00..99)
				{
					storage += std::to_string(context.tm.tm_year % 100);

					break;
				}

				case 'Y':  /// 年份 (1970...)
				{
					storage += std::to_string(context.tm.tm_year + 1900);

					break;
				}

				case 'z':  /// 数字时区
				{
					/// todo

					break;
				}

				case 'Z':  /// 按字母表排序的时区缩写
				{
					/// todo

					break;
				}

				default: /// 其余格式化
				{
					storage.push_back('%');
					storage.push_back(value);

					break;
				}
			}
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 构造函数
		 *
		 * @param pattern 模式
		 *
		 */
		PatternLayout::PatternLayout(std::string pattern) : _pattern(std::move(pattern))
		{

		}

		/**
		 *
		 * 格式化
		 *
		 * @param context 上下文
		 *
		 * @return 格式化后内容
		 *
		 */
		std::string PatternLayout::Format(const Context & context)
		{
			std::string value{ };

			for (auto iter = _pattern.begin(); iter != _pattern.end(); ++iter)
			{
				if (*iter == '%')
				{
					if (++iter == _pattern.end())
					{
						break;
					}

					FormatPattern(context, value, *iter);
				}
				else
				{
					value.push_back(*iter);
				}
			}

			return value;
		}
	}
}
