#ifndef __TINY_TOOLKIT__UTILITIES__TIME__H__
#define __TINY_TOOLKIT__UTILITIES__TIME__H__


/**
 *
 *  作者: hm
 *
 *  说明: 时间处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Time
	{
		using ClockType = std::chrono::system_clock;
		using ClockDuration = std::chrono::system_clock::duration;
		using ClockTimePoint = std::chrono::system_clock::time_point;

	public:
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
		static bool IsSameYear(std::time_t sSeconds, std::time_t dSeconds)
		{
			std::tm src = UTCTm(sSeconds);
			std::tm dst = UTCTm(dSeconds);

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
		static bool IsSameMonth(std::time_t sSeconds, std::time_t dSeconds)
		{
			std::tm src = UTCTm(sSeconds);
			std::tm dst = UTCTm(dSeconds);

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
		static bool IsSameDay(std::time_t sSeconds, std::time_t dSeconds)
		{
			std::tm src = UTCTm(sSeconds);
			std::tm dst = UTCTm(dSeconds);

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
		static bool IsSameHour(std::time_t sSeconds, std::time_t dSeconds)
		{
			std::tm src = UTCTm(sSeconds);
			std::tm dst = UTCTm(dSeconds);

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
		static bool IsSameMinute(std::time_t sSeconds, std::time_t dSeconds)
		{
			std::tm src = UTCTm(sSeconds);
			std::tm dst = UTCTm(dSeconds);

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
		static std::tm UTCTm()
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
		static std::tm UTCTm(std::time_t seconds)
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
		static void UTCTm(std::time_t seconds, std::tm & tm)
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
		static std::tm LocalTm()
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
		static std::tm LocalTm(std::time_t seconds)
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
		static void LocalTm(std::time_t seconds, std::tm & tm)
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
		static std::time_t Hours()
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
		static std::time_t Hours(const ClockTimePoint & timesPoint)
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
		static std::time_t Hours(const ClockDuration & duration)
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
		static std::time_t Minutes()
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
		static std::time_t Minutes(const ClockTimePoint & timesPoint)
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
		static std::time_t Minutes(const ClockDuration & duration)
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
		static std::time_t Seconds()
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
		static std::time_t Seconds(const ClockTimePoint & timesPoint)
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
		static std::time_t Seconds(const ClockDuration & duration)
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
		static std::time_t Milliseconds()
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
		static std::time_t Milliseconds(const ClockTimePoint & timesPoint)
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
		static std::time_t Milliseconds(const ClockDuration & duration)
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
		static std::time_t Microseconds()
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
		static std::time_t Microseconds(const ClockTimePoint & timesPoint)
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
		static std::time_t Microseconds(const ClockDuration & duration)
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
		static std::time_t Nanoseconds()
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
		static std::time_t Nanoseconds(const ClockTimePoint & timesPoint)
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
		static std::time_t Nanoseconds(const ClockDuration & duration)
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
		static std::time_t TimeZone()
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
		 * tm转换成秒数时间戳
		 *
		 * @param date 时间结构体
		 *
		 * @return 秒数时间戳
		 *
		 */
		static std::time_t FromTm(std::tm & date)
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
		static std::time_t FromTimeString(const char * value, const char * format = "%4d-%02d-%02d %02d:%02d:%02d")
		{
			assert(value && format);

			std::tm now = { 0 };

			sscanf(value, format, &now.tm_year, &now.tm_mon, &now.tm_mday, &now.tm_hour, &now.tm_min, &now.tm_sec);

			now.tm_year -= 1900;
			now.tm_mon -= 1;

			return FromTm(now);
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
		static std::string FormatTimeString(std::time_t seconds, const char * format = "%4d-%02d-%02d %02d:%02d:%02d")
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
		static std::string FormatTimeString(std::time_t seconds, std::time_t timeZone, const char * format = "%4d-%02d-%02d %02d:%02d:%02d")
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
		static std::string CurrentUTCTimeString(const char * format = "%4d-%02d-%02d %02d:%02d:%02d")
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
		static std::string CurrentLocalTimeString(const char * format = "%4d-%02d-%02d %02d:%02d:%02d")
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
		static ClockTimePoint TimePoint()
		{
			return ClockType::now();
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
		static ClockTimePoint TimePoint(std::time_t time)
		{
			return std::chrono::time_point_cast<ClockDuration>(std::chrono::time_point<ClockType, std::chrono::seconds>(std::chrono::seconds(time)));
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
		static ClockTimePoint TimePoint(const struct timeval & time)
		{
			return std::chrono::time_point<ClockType, ClockDuration>
			(
				std::chrono::duration_cast<ClockDuration>
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
		static ClockTimePoint TimePoint(const struct timespec & time)
		{
			return std::chrono::time_point<ClockType, ClockDuration>
			(
				std::chrono::duration_cast<ClockDuration>
				(
					std::chrono::seconds(time.tv_sec) + std::chrono::nanoseconds(time.tv_nsec)
				)
			);
		}

		static ClockDuration TimeDuration()
		{
			return TimePoint().time_since_epoch();
		}

		static ClockDuration TimeDuration(std::time_t time)
		{
			return TimePoint(time).time_since_epoch();
		}

		static ClockDuration TimeDuration(const struct timeval & time)
		{
			return TimePoint(time).time_since_epoch();
		}

		static ClockDuration TimeDuration(const struct timespec & time)
		{
			return TimePoint(time).time_since_epoch();
		}
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__TIME__H__
