/**
 *
 *  作者: hm
 *
 *  说明: 时间处理
 *
 */


#include "time.h"


namespace tinyToolkit
{
	/**
	 *
	 * 是否是同一年
	 *
	 * @param sSeconds 秒数时间戳
	 * @param dSeconds 秒数时间戳
	 *
	 * @return 状态
	 *
	 */
	bool Time::IsSameYear(std::time_t sSeconds, std::time_t dSeconds)
	{
		std::tm src = LocalTm(sSeconds);
		std::tm dst = LocalTm(dSeconds);

		return (src.tm_year == dst.tm_year);
	}

	/**
	 *
	 * 是否是同一月
	 *
	 * @param sSeconds 秒数时间戳
	 * @param dSeconds 秒数时间戳
	 *
	 * @return 状态
	 *
	 */
	bool Time::IsSameMonth(std::time_t sSeconds, std::time_t dSeconds)
	{
		std::tm src = LocalTm(sSeconds);
		std::tm dst = LocalTm(dSeconds);

		return (src.tm_year == dst.tm_year &&
				src.tm_mon == dst.tm_mon);
	}

	/**
	 *
	 * 是否是同一天
	 *
	 * @param sSeconds 秒数时间戳
	 * @param dSeconds 秒数时间戳
	 *
	 * @return 状态
	 *
	 */
	bool Time::IsSameDay(std::time_t sSeconds, std::time_t dSeconds)
	{
		std::tm src = LocalTm(sSeconds);
		std::tm dst = LocalTm(dSeconds);

		return (src.tm_year == dst.tm_year &&
				src.tm_mon == dst.tm_mon &&
				src.tm_mday == dst.tm_mday);
	}

	/**
	 *
	 * 是否是同一小时
	 *
	 * @param sSeconds 秒数时间戳
	 * @param dSeconds 秒数时间戳
	 *
	 * @return 状态
	 *
	 */
	bool Time::IsSameHour(std::time_t sSeconds, std::time_t dSeconds)
	{
		std::tm src = LocalTm(sSeconds);
		std::tm dst = LocalTm(dSeconds);

		return (src.tm_year == dst.tm_year &&
				src.tm_mon == dst.tm_mon &&
				src.tm_mday == dst.tm_mday &&
				src.tm_hour == dst.tm_hour);
	}

	/**
	 *
	 * 是否是同一分钟
	 *
	 * @param sSeconds 秒数时间戳
	 * @param dSeconds 秒数时间戳
	 *
	 * @return 状态
	 *
	 */
	bool Time::IsSameMinute(std::time_t sSeconds, std::time_t dSeconds)
	{
		std::tm src = LocalTm(sSeconds);
		std::tm dst = LocalTm(dSeconds);

		return (src.tm_year == dst.tm_year &&
				src.tm_mon == dst.tm_mon &&
				src.tm_mday == dst.tm_mday &&
				src.tm_hour == dst.tm_hour &&
				src.tm_min == dst.tm_min);
	}

	/**
	 *
	 * utc时间对应的tm结构体
	 *
	 * @return 时间结构体
	 *
	 */
	std::tm Time::UTCTm()
	{
		return UTCTm(Seconds());
	}

	/**
	 *
	 * utc时间对应的tm结构体
	 *
	 * @param seconds 秒
	 *
	 * @return 时间结构体
	 *
	 */
	std::tm Time::UTCTm(std::time_t seconds)
	{
		std::tm tm = { };

		UTCTm(seconds, tm);

		return tm;
	}

	/**
	 *
	 * utc时间对应的tm结构体
	 *
	 * @param seconds 秒
	 * @param tm 时间结构体
	 *
	 */
	void Time::UTCTm(std::time_t seconds, std::tm & tm)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		gmtime_s(&tm, &seconds);

#else

		gmtime_r(&seconds, &tm);

#endif
	}

	/**
	 *
	 * 本地时间对应的tm结构体
	 *
	 * @return 时间结构体
	 *
	 */
	std::tm Time::LocalTm()
	{
		return LocalTm(Seconds());
	}

	/**
	 *
	 * 本地时间对应的tm结构体
	 *
	 * @param seconds 秒
	 *
	 * @return 时间结构体
	 *
	 */
	std::tm Time::LocalTm(std::time_t seconds)
	{
		std::tm tm = { };

		LocalTm(seconds, tm);

		return tm;
	}

	/**
	 *
	 * 本地时间对应的tm结构体
	 *
	 * @param seconds 秒
	 * @param tm 时间结构体
	 *
	 */
	void Time::LocalTm(std::time_t seconds, std::tm & tm)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		localtime_s(&tm, &seconds);

#else

		localtime_r(&seconds, &tm);

#endif
	}

	/**
	 *
	 * 小时时间戳
	 *
	 * @return 小时时间戳
	 *
	 */
	std::time_t Time::Hours()
	{
		return Hours(TimePoint());
	}

	/**
	 *
	 * 小时时间戳
	 *
	 * @param timesPoint 时间点
	 *
	 * @return 小时时间戳
	 *
	 */
	std::time_t Time::Hours(const SystemClockTimePoint & timesPoint)
	{
		return Hours(timesPoint.time_since_epoch());
	}

	/**
	 *
	 * 小时时间戳
	 *
	 * @param duration 时间段
	 *
	 * @return 小时时间戳
	 *
	 */
	std::time_t Time::Hours(const SystemClockDuration & duration)
	{
		return std::chrono::duration_cast<std::chrono::hours>(duration).count();
	}

	/**
	 *
	 * 分钟时间戳
	 *
	 * @return 分钟时间戳
	 *
	 */
	std::time_t Time::Minutes()
	{
		return Minutes(TimePoint());
	}

	/**
	 *
	 * 分钟时间戳
	 *
	 * @param timesPoint 时间点
	 *
	 * @return 分钟时间戳
	 *
	 */
	std::time_t Time::Minutes(const SystemClockTimePoint & timesPoint)
	{
		return Minutes(timesPoint.time_since_epoch());
	}

	/**
	 *
	 * 分钟时间戳
	 *
	 * @param duration 时间段
	 *
	 * @return 分钟时间戳
	 *
	 */
	std::time_t Time::Minutes(const SystemClockDuration & duration)
	{
		return std::chrono::duration_cast<std::chrono::minutes>(duration).count();
	}

	/**
	 *
	 * 秒数时间戳
	 *
	 * @return 秒数时间戳
	 *
	 */
	std::time_t Time::Seconds()
	{
		return Seconds(TimePoint());
	}

	/**
	 *
	 * 秒数时间戳
	 *
	 * @param timesPoint 时间点
	 *
	 * @return 秒数时间戳
	 *
	 */
	std::time_t Time::Seconds(const SystemClockTimePoint & timesPoint)
	{
		return Seconds(timesPoint.time_since_epoch());
	}

	/**
	 *
	 * 秒数时间戳
	 *
	 * @param duration 时间段
	 *
	 * @return 秒数时间戳
	 *
	 */
	std::time_t Time::Seconds(const SystemClockDuration & duration)
	{
		return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
	}

	/**
	 *
	 * 毫秒时间戳
	 *
	 * @return 毫秒时间戳
	 *
	 */
	std::time_t Time::Milliseconds()
	{
		return Milliseconds(TimePoint());
	}

	/**
	 *
	 * 毫秒时间戳
	 *
	 * @param timesPoint 时间点
	 *
	 * @return 毫秒时间戳
	 *
	 */
	std::time_t Time::Milliseconds(const SystemClockTimePoint & timesPoint)
	{
		return Milliseconds(timesPoint.time_since_epoch());
	}

	/**
	 *
	 * 毫秒时间戳
	 *
	 * @param duration 时间段
	 *
	 * @return 毫秒时间戳
	 *
	 */
	std::time_t Time::Milliseconds(const SystemClockDuration & duration)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	}

	/**
	 *
	 * 微秒时间戳
	 *
	 * @return 微秒时间戳
	 *
	 */
	std::time_t Time::Microseconds()
	{
		return Microseconds(TimePoint());
	}

	/**
	 *
	 * 微秒时间戳
	 *
	 * @param timesPoint 时间点
	 *
	 * @return 微秒时间戳
	 *
	 */
	std::time_t Time::Microseconds(const SystemClockTimePoint & timesPoint)
	{
		return Microseconds(timesPoint.time_since_epoch());
	}

	/**
	 *
	 * 微秒时间戳
	 *
	 * @param duration 时间段
	 *
	 * @return 微秒时间戳
	 *
	 */
	std::time_t Time::Microseconds(const SystemClockDuration & duration)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	}

	/**
	 *
	 * 纳秒时间戳
	 *
	 * @return 纳秒时间戳
	 *
	 */
	std::time_t Time::Nanoseconds()
	{
		return Nanoseconds(TimePoint());
	}

	/**
	 *
	 * 纳秒时间戳
	 *
	 * @param timesPoint 时间点
	 *
	 * @return 纳秒时间戳
	 *
	 */
	std::time_t Time::Nanoseconds(const SystemClockTimePoint & timesPoint)
	{
		return Nanoseconds(timesPoint.time_since_epoch());
	}

	/**
	 *
	 * 纳秒时间戳
	 *
	 * @param duration 时间段
	 *
	 * @return 纳秒时间戳
	 *
	 */
	std::time_t Time::Nanoseconds(const SystemClockDuration & duration)
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
	}

	/**
	 *
	 * 时区
	 *
	 * @return 时区
	 *
	 */
	std::time_t Time::TimeZone()
	{
		static std::pair<std::time_t, bool> value(0, true);

		if (value.second)
		{
			std::tm utc   = UTCTm();
			std::tm local = LocalTm();

			value.first = (FromTm(local) - FromTm(utc)) / TINY_TOOLKIT_HOUR;

			value.second = false;
		}

		return value.first;
	}

	/**
	 *
	 * 下一天指定时间的时间戳
	 *
	 * @param hour 时
	 * @param minutes 分
	 * @param seconds 秒
	 *
	 * @return 下一天指定时间的时间戳
	 *
	 */
	std::time_t Time::NextDayTime(int32_t hour, int32_t minutes, int32_t seconds)
	{
		if (hour < 0 || hour > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
		{
			throw std::logic_error("Invalid Time");
		}

		return CurrentDayTime(hour, minutes, seconds) + TINY_TOOLKIT_DAY;
	}

	/**
	 *
	 * 当天指定时间的时间戳
	 *
	 * @param hour 时
	 * @param minutes 分
	 * @param seconds 秒
	 *
	 * @return 当天指定时间的时间戳
	 *
	 */
	std::time_t Time::CurrentDayTime(int32_t hour, int32_t minutes, int32_t seconds)
	{
		if (hour < 0 || hour > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
		{
			throw std::logic_error("Invalid Time");
		}

		std::tm date = LocalTm();

		date.tm_hour = hour;
		date.tm_min  = minutes;
		date.tm_sec  = seconds;

		return FromTm(date);
	}

	/**
	 *
	 * tm转换成秒数时间戳
	 *
	 * @param date 时间结构体
	 *
	 * @return 秒数时间戳
	 *
	 */
	std::time_t Time::FromTm(std::tm & date)
	{
		return std::mktime(&date);
	}

	/**
	 *
	 * 日期字符串转换成秒数时间戳
	 *
	 * @param value 日期字符串
	 * @param format 格式
	 *
	 * @return 秒数时间戳
	 *
	 */
	std::time_t Time::FromTimeString(const char * value, const char * format)
	{
		assert(value && format);

		std::tm now{ };

		sscanf(value, format, &now.tm_year, &now.tm_mon, &now.tm_mday, &now.tm_hour, &now.tm_min, &now.tm_sec);

		now.tm_year -= 1900;
		now.tm_mon -= 1;

		return FromTm(now);
	}

	/**
	 *
	 * 日期字符串转换成秒数时间戳
	 *
	 * @param value 日期字符串
	 * @param format 格式
	 *
	 * @return 秒数时间戳
	 *
	 */
	std::time_t Time::FromTimeString(const std::string & value, const char * format)
	{
		return FromTimeString(value.c_str(), format);
	}

	/**
	 *
	 * 格式化日期字符串
	 *
	 * @param seconds 秒数时间戳
	 * @param format 格式
	 *
	 * @return 日期字符串
	 *
	 */
	std::string Time::FormatTimeString(std::time_t seconds, const char * format)
	{
		assert(format);

		return FormatTimeString(seconds, TimeZone(), format);
	}

	/**
	 *
	 * 格式化日期字符串
	 *
	 * @param seconds 秒数时间戳
	 * @param timeZone 时区
	 * @param format 格式
	 *
	 * @return 日期字符串
	 *
	 */
	std::string Time::FormatTimeString(std::time_t seconds, std::time_t timeZone, const char * format)
	{
		assert(format);

		char str[128] = { 0 };

		std::tm now = UTCTm(seconds + timeZone * TINY_TOOLKIT_HOUR);

		snprintf(str, sizeof(str), format, now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);

		return str;
	}

	/**
	 *
	 * 格式化当前UTC日期字符串
	 *
	 * @param format 格式
	 *
	 * @return 日期字符串
	 *
	 */
	std::string Time::CurrentUTCTimeString(const char * format)
	{
		return FormatTimeString(Seconds(), 0, format);
	}

	/**
	 *
	 * 格式化当前本地日期字符串
	 *
	 * @param format 格式
	 *
	 * @return 日期字符串
	 *
	 */
	std::string Time::CurrentLocalTimeString(const char * format)
	{
		return FormatTimeString(Seconds(), format);
	}

	/**
	 *
	 * 时间点
	 *
	 * @return 时间点
	 *
	 */
	SystemClockTimePoint Time::TimePoint()
	{
		return SystemClockType::now();
	}

	/**
	 *
	 * 时间点
	 *
	 * @param seconds 秒数时间戳
	 *
	 * @return 时间点
	 *
	 */
	SystemClockTimePoint Time::TimePoint(std::time_t time)
	{
		return std::chrono::time_point_cast<SystemClockDuration>(std::chrono::time_point<SystemClockType, std::chrono::seconds>(std::chrono::seconds(time)));
	}

	/**
	 *
	 * 时间点
	 *
	 * @param time 时间结构体
	 *
	 * @return 时间点
	 *
	 */
	SystemClockTimePoint Time::TimePoint(const struct timeval & time)
	{
		return std::chrono::time_point<SystemClockType, SystemClockDuration>
		(
			std::chrono::duration_cast<SystemClockDuration>
			(
				std::chrono::seconds(time.tv_sec) + std::chrono::microseconds(time.tv_usec)
			)
		);
	}

	/**
	 *
	 * 时间点
	 *
	 * @param time 时间结构体
	 *
	 * @return 时间点
	 *
	 */
	SystemClockTimePoint Time::TimePoint(const struct timespec & time)
	{
		return std::chrono::time_point<SystemClockType, SystemClockDuration>
		(
			std::chrono::duration_cast<SystemClockDuration>
			(
				std::chrono::seconds(time.tv_sec) + std::chrono::nanoseconds(time.tv_nsec)
			)
		);
	}

	/**
	 *
	 * 时间段
	 *
	 * @return 时间段
	 *
	 */
	SystemClockDuration Time::TimeDuration()
	{
		return TimePoint().time_since_epoch();
	}

	/**
	 *
	 * 时间段
	 *
	 * @param time 秒数时间戳
	 *
	 * @return 时间段
	 *
	 */
	SystemClockDuration Time::TimeDuration(std::time_t time)
	{
		return TimePoint(time).time_since_epoch();
	}

	/**
	 *
	 * 时间段
	 *
	 * @param time 时间结构体
	 *
	 * @return 时间段
	 *
	 */
	SystemClockDuration Time::TimeDuration(const struct timeval & time)
	{
		return TimePoint(time).time_since_epoch();
	}

	/**
	 *
	 * 时间段
	 *
	 * @param time 时间结构体
	 *
	 * @return 时间段
	 *
	 */
	SystemClockDuration Time::TimeDuration(const struct timespec & time)
	{
		return TimePoint(time).time_since_epoch();
	}
}
