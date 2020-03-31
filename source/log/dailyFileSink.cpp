/**
 *
 *  作者: hm
 *
 *  说明: 时间文件节点
 *
 */


#include "dailyFileSink.h"

#include "../util/time.h"
#include "../util/string.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 * @param path 路径
		 * @param hour 小时
		 * @param minute 分钟
		 * @param second 秒数
		 *
		 */
		DailyFileSink::DailyFileSink(std::string name, std::string path, int32_t hour, int32_t minute, int32_t second) : ISink(std::move(name)),
		                                                                                                                 _hour(hour),
		                                                                                                                 _minute(minute),
		                                                                                                                 _second(second),
		                                                                                                                 _path(std::move(path))
		{
			if (!_file.Open(FormatPath()))
			{
				throw std::logic_error("Open daily log file failed : " + _file.Path());
			}

			UpdateTime();
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		DailyFileSink::~DailyFileSink()
		{
			_file.Close();
		}

		/**
		 *
		 * 关闭节点
		 *
		 */
		void DailyFileSink::Close()
		{
			_file.Close();
		}

		/**
		 *
		 * 刷新节点
		 *
		 */
		void DailyFileSink::Flush()
		{
			_file.Flush();
		}

		/**
		 *
		 * 写入内容
		 *
		 * @param context 上下文
		 *
		 */
		void DailyFileSink::Write(const Context & context)
		{
			if (Filter() && Filter()->Decide(context))
			{
				return;
			}

			if (util::Time::Seconds() >= _time)
			{
				_file.Close();

				if (!_file.Open(FormatPath()))
				{
					throw std::logic_error("Open daily log file failed : " + _file.Path());
				}

				UpdateTime();
			}

			_file.Write(Layout() ? Layout()->Format(context) : context.content);

			if (IsAutoFlush())
			{
				Flush();
			}
		}

		/**
		 *
		 * 刷新下次生成的时间
		 *
		 */
		void DailyFileSink::UpdateTime()
		{
			_time = util::Time::DayTime(0, _hour, _minute, _second);

			if (_time < util::Time::Seconds())
			{
				_time += 86400;
			}
		}

		/**
		 *
		 * 格式化路径
		 *
		 * @return 路径
		 *
		 */
		std::string DailyFileSink::FormatPath()
		{
			std::size_t pos = _path.rfind('.');

			auto time = util::Time::LocalTimeString("%4d_%02d_%02d_%02d_%02d_%02d");

			if (pos == std::string::npos)  /// name_0000_00_00_00_00_00
			{
				return util::String::Format
				(
					"{}_{}",
					_path,
					time
				);
			}
			else if (pos == 0)  /// 0000_00_00_00_00_00.log
			{
				return util::String::Format
				(
					"{}{}",
					time,
					_path
				);
			}
			else   /// name_0000_00_00_00_00_00.log
			{
				return util::String::Format
				(
					"{}_{}{}",
					_path.substr(0, pos),
					time,
					_path.substr(pos)
				);
			}
		}
	}
}
