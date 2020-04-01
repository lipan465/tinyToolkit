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
			static char buffer[128]{ 0 };

			static std::size_t size = sizeof(buffer);
			static std::size_t length = 0;

			switch (value)
			{
				case 'a':  /// 当前时区的星期名简写 (Sun..Sat)
				{
					storage += WeekCorrespond::ShortName(context.tm.tm_wday);

					break;
				}

				case 'A':  /// 当前时区的星期名全称 (Sunday..Saturday)
				{
					storage += WeekCorrespond::LongName(context.tm.tm_wday);

					break;
				}

				case 'b':  /// 当前时区的月份名简写 (Jan..Dec)
				{
					storage += MonthCorrespond::ShortName(context.tm.tm_mon);

					break;
				}

				case 'B':  /// 当前时区的月份名全称 (January..December)
				{
					storage += MonthCorrespond::LongName(context.tm.tm_mon);

					break;
				}

				case 'c':  /// 当前时区的日期和时间 (Sun 01 Jan 1970 00:00:00 AM)
				{
					storage += util::String::Format
					(
						"{} {:02} {} {:04} {:02}:{:02}:{:02} {}",
						WeekCorrespond::ShortName(context.tm.tm_wday),
						context.tm.tm_mday,
						MonthCorrespond::ShortName(context.tm.tm_mon),
						context.tm.tm_year + 1900,
						context.tm.tm_hour > 12 ? context.tm.tm_hour - 12 : context.tm.tm_hour,
						context.tm.tm_min,
						context.tm.tm_sec,
						context.tm.tm_hour >= 12 ? "PM" : "AM"
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
						"{:02}/{:02}/{:02}",
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

				case 'E':  /// 微秒 (000000..999999)
				{
					storage += util::String::Format
					(
						"{:06}",
						util::Time::Nanoseconds(context.time) % 1000000
					);

					break;
				}

				case 'f':  /// 毫秒 (000..999)
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

				case 'g':  /// ISO-8601格式年份的最后两位
				{
					storage += std::to_string(context.tm.tm_year % 100);

					break;
				}

				case 'G':  /// ISO-8601格式年份
				{
					storage += std::to_string(context.tm.tm_year + 1900);

					break;
				}

				case 'h':  /// 当前时区的月份名简写 (Jan..Dec)
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
						context.tm.tm_hour > 12 ? context.tm.tm_hour - 12 : context.tm.tm_hour
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

				case 'k':  /// 小时 ( 0..23)
				{
					storage += util::String::Format
					(
						"{:2}",
						context.tm.tm_hour
					);

					break;
				}

				case 'K':  /// 进程序列号
				{
					storage += std::to_string(context.processID);

					break;
				}

				case 'l':  /// 小时 ( 1..12)
				{
					storage += util::String::Format
					(
						"{:2}",
						context.tm.tm_hour > 12 ? context.tm.tm_hour - 12 : context.tm.tm_hour
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

				case 'N': /// 纳秒 (000000000..999999999)
				{
					storage += util::String::Format
					(
						"{:09}",
						util::Time::Nanoseconds(context.time) % 1000000000
					);

					break;
				}

				case 'p':  /// 当前时区下的AM或者PM
				{
					storage += context.tm.tm_hour >= 12 ? "PM" : "AM";

					break;
				}

				case 'P':  /// 当前时区下的am或者pm
				{
					storage += context.tm.tm_hour >= 12 ? "pm" : "am";

					break;
				}

				case 'r':  /// 12小时时钟时间 (hh:mm:ss AM/PM)
				{
					storage += util::String::Format
					(
						"{:02}:{:02}:{:02} {}",
						context.tm.tm_hour > 12 ? context.tm.tm_hour - 12 : context.tm.tm_hour,
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

				case 's':  /// 自UTC时间 1970-01-01 00:00:00 以来所经过的秒数
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

				case 'u':  /// 星期 (1..7), 1代表周一
				{
					storage += std::to_string(context.tm.tm_wday == 0 ? 7 : context.tm.tm_wday);

					break;
				}

				case 'U':  /// 一年中的第几周, 以周日为每星期第一天 (00-53)
				{
					storage += util::String::Format
					(
						"{:02}",
						(context.tm.tm_yday + (6 - context.tm.tm_wday)) / 7
					);

					break;
				}

				case 'v':  /// 输入的日志信息
				{
					storage += context.content;

					break;
				}

				case 'V':  /// ISO-8601格式规范下的一年中第几周, 以周一为每星期第一天 (01-53)
				{
					length = strftime(buffer, size, "%V", &context.tm);

					buffer[length] = '\0';

					storage.append(buffer, length);

					break;
				}

				case 'w':  /// 一星期中的第几日(0..6), 0代表周一
				{
					if (context.tm.tm_wday == 0)
					{
						storage += "6";
					}
					else
					{
						storage += std::to_string(context.tm.tm_wday - 1);
					}

					break;
				}

				case 'W':  /// 一年中的第几周, 以周一为每星期第一天 (00..53)
				{
					storage += util::String::Format
					(
						"{:02}",
						(context.tm.tm_yday + (context.tm.tm_wday == 0 ? 0 : (7 - context.tm.tm_wday))) / 7
					);

					break;
				}

				case 'x':  /// 当前时区下的日期描述 (mm/dd/yy)
				{
					storage += util::String::Format
					(
						"{:02}/{:02}/{:04}",
						context.tm.tm_mon + 1,
						context.tm.tm_mday,
						context.tm.tm_year + 1900
					);

					break;
				}

				case 'X':  /// 当前时区下的时间描述 (hh:mm:ss AM/PM)
				{
					storage += util::String::Format
					(
						"{:02}:{:02}:{:02} {}",
						context.tm.tm_hour > 12 ? context.tm.tm_hour - 12 : context.tm.tm_hour,
						context.tm.tm_min,
						context.tm.tm_sec,
						context.tm.tm_hour >= 12 ? "PM" : "AM"
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

				case 'z':  /// 数字时区 (+0800)
				{
					length = strftime(buffer, size, "%z", &context.tm);

					buffer[length] = '\0';

					storage.append(buffer, length);

					break;
				}

				case 'Z':  /// 按字母表排序的时区缩写 (CST)
				{
					length = strftime(buffer, size, "%Z", &context.tm);

					buffer[length] = '\0';

					storage.append(buffer, length);

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
