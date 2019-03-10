/**
 *
 *  作者: hm
 *
 *  说明: 日志时间文件节点
 *
 */


#include "dailyFileSink.h"

#include "../utilities/time.h"
#include "../utilities/string.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param name 节点名称
	 * @param path 日志路径
	 * @param hour 时
	 * @param minutes 分
	 * @param seconds 秒
	 *
	 */
	DailyFileLogSink::DailyFileLogSink(std::string name, std::string path, int32_t hour, int32_t minutes, int32_t seconds) : ILogSink(std::move(name)),
																															 _hour(hour),
																															 _minutes(minutes),
																															 _seconds(seconds),
																															 _path(std::move(path))
	{
		if (hour < 0 || hour > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
		{
			throw std::logic_error("Invalid Time");
		}

		if (!_file.Open(CalculatePath(_path)))
		{
			throw std::logic_error("Open daily log file failed : " + _file.Path());
		}

		RotatingTime();
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	DailyFileLogSink::~DailyFileLogSink()
	{
		Close();
	}

	/**
	 *
	 * 关闭日志
	 *
	 */
	void DailyFileLogSink::Close()
	{
		_file.Close();
	}

	/**
	 *
	 * 刷新日志
	 *
	 */
	void DailyFileLogSink::Flush()
	{
		_file.Flush();
	}

	/**
	 *
	 * 重新打开日志
	 *
	 */
	void DailyFileLogSink::Reopen()
	{
		if (!_file.Reopen())
		{
			throw std::logic_error("ReOpen daily log file failed : " + _file.Path());
		}
	}

	/**
	 *
	 * 写入日志
	 *
	 * @param event 日志事件
	 *
	 */
	void DailyFileLogSink::Write(const LogEvent & event)
	{
		if (Filter() && Filter()->Decide(event))
		{
			return;
		}

		if (Time::Seconds() >= _time)
		{
			_file.Close();

			if (!_file.Open(CalculatePath(_path)))
			{
				throw std::logic_error("Open daily log file failed : " + _file.Path());
			}

			RotatingTime();
		}

		_file.Write(Layout() ? Layout()->Format(event) : event.message);

		if (_autoFlush)
		{
			Flush();
		}
	}

	/**
	 *
	 * 是否已经打开
	 *
	 * @return 是否已经打开
	 *
	 */
	bool DailyFileLogSink::IsOpen()
	{
		return _file.IsOpen();
	}

	/**
	 *
	 * 日志大小
	 *
	 * @return 日志大小
	 *
	 */
	std::size_t DailyFileLogSink::Size() const
	{
		return _file.Size();
	}

	/**
	 *
	 * 下次生成日志时间
	 *
	 * @return 下次生成日志时间
	 *
	 */
	std::time_t DailyFileLogSink::NextTime() const
	{
		return _time;
	}

	/**
	 *
	 * 日志路径
	 *
	 * @return 日志路径
	 *
	 */
	const std::string & DailyFileLogSink::Path() const
	{
		return _file.Path();
	}

	/**
	 *
	 * 计算下一次生成日志的时间
	 *
	 */
	void DailyFileLogSink::RotatingTime()
	{
		_time = Time::CurrentDayTime(_hour, _minutes, _seconds);

		if (_time < Time::Seconds())
		{
			_time += TINY_TOOLKIT_DAY;
		}
	}

	/**
	 *
	 * 推导日志路径
	 *
	 * @param path 日志路径
	 *
	 * @return 日志路径
	 *
	 */
	std::string DailyFileLogSink::CalculatePath(const std::string & path)
	{
		std::size_t pos = path.rfind('.');

		if (pos == std::string::npos)  /// name_2018_01_01_00_00_00
		{
			return String::Format
			(
				"{}_{}",
				path,
				Time::CurrentLocalTimeString("%4d_%02d_%02d_%02d_%02d_%02d")
			);
		}
		else if (pos == 0)  /// 2018_01_01_00_00_00.log
		{
			return String::Format
			(
				"{}{}",
				Time::CurrentLocalTimeString("%4d_%02d_%02d_%02d_%02d_%02d"),
				path
			);
		}
		else   /// name_2018_01_01_00_00_00.log
		{
			return String::Format
			(
				"{}_{}{}",
				path.substr(0, pos),
				Time::CurrentLocalTimeString("%4d_%02d_%02d_%02d_%02d_%02d"),
				path.substr(pos)
			);
		}
	}
}
