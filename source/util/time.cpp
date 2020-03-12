/**
 *
 *  作者: hm
 *
 *  说明: 时间处理
 *
 */


#include "time.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <mutex>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <mutex>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <mutex>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		/**
		 *
		 * 时间结构体
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
		 * 时间结构体
		 *
		 * @param seconds 秒数时间戳
		 *
		 * @return 时间结构体
		 *
		 */
		std::tm Time::UTCTm(std::time_t seconds)
		{
			std::tm tm{ };

			UTCTm(seconds, tm);

			return tm;
		}

		/**
		 *
		 * 时间结构体
		 *
		 * @param seconds 秒数时间戳
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
		 * 时间结构体
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
		 * 时间结构体
		 *
		 * @param seconds 秒数时间戳
		 *
		 * @return 时间结构体
		 *
		 */
		std::tm Time::LocalTm(std::time_t seconds)
		{
			std::tm tm{ };

			LocalTm(seconds, tm);

			return tm;
		}

		/**
		 *
		 * 时间结构体
		 *
		 * @param seconds 秒数时间戳
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
		 * 时区
		 *
		 * @return 时区
		 *
		 */
		std::time_t Time::Timezone()
		{
			static bool status{ false };

			static std::time_t timezone{ 0 };

			static std::once_flag onceFlag{ };

			std::call_once(onceFlag, [&]()
			{
				if (status)
				{
					return;
				}

				std::tm utc   = UTCTm();
				std::tm local = LocalTm();

				timezone = (FromTm(local) - FromTm(utc)) / 3600;

				status = true;
			});

			return timezone;
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
		std::time_t Time::Hours(const std::chrono::system_clock::time_point & timesPoint)
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
		std::time_t Time::Hours(const std::chrono::system_clock::duration & duration)
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
		std::time_t Time::Minutes(const std::chrono::system_clock::time_point & timesPoint)
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
		std::time_t Time::Minutes(const std::chrono::system_clock::duration & duration)
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
		std::time_t Time::Seconds(const std::chrono::system_clock::time_point & timesPoint)
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
		std::time_t Time::Seconds(const std::chrono::system_clock::duration & duration)
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
		std::time_t Time::Milliseconds(const std::chrono::system_clock::time_point & timesPoint)
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
		std::time_t Time::Milliseconds(const std::chrono::system_clock::duration & duration)
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
		std::time_t Time::Microseconds(const std::chrono::system_clock::time_point & timesPoint)
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
		std::time_t Time::Microseconds(const std::chrono::system_clock::duration & duration)
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
		std::time_t Time::Nanoseconds(const std::chrono::system_clock::time_point & timesPoint)
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
		std::time_t Time::Nanoseconds(const std::chrono::system_clock::duration & duration)
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
		}

		/**
		 *
		 * 基于今日偏移的天数时间戳
		 *
		 * @param day 天
		 * @param hour 时
		 * @param minutes 分
		 * @param seconds 秒
		 *
		 * @return 天数时间戳
		 *
		 */
		std::time_t Time::DayTime(int64_t day, int32_t hour, int32_t minutes, int32_t seconds)
		{
			if (hour < 0 || hour > 23)
			{
				throw std::range_error
				(
					"Hour [" + std::to_string(hour) + "] out of range [0 ~ 23]"
				);
			}

			if (minutes < 0 || minutes > 59)
			{
				throw std::range_error
				(
					"Minutes [" + std::to_string(hour) + "] out of range [0 ~ 59]"
				);
			}

			if (seconds < 0 || seconds > 59)
			{
				throw std::range_error
				(
					"Seconds [" + std::to_string(hour) + "] out of range [0 ~ 59]"
				);
			}

			std::tm date = LocalTm();

			date.tm_hour = hour;
			date.tm_min  = minutes;
			date.tm_sec  = seconds;

			return FromTm(date) + (day * 86400);
		}

		/**
		 *
		 * 转换为秒数时间戳
		 *
		 * @param tm 时间结构体
		 *
		 * @return 秒数时间戳
		 *
		 */
		std::time_t Time::FromTm(std::tm & tm)
		{
			return std::mktime(&tm);
		}

		/**
		 *
		 * 转换为秒数时间戳
		 *
		 * @param date 日期
		 * @param pattern 格式
		 *
		 * @return 秒数时间戳
		 *
		 */
		std::time_t Time::FromString(const char * date, const char * pattern)
		{
			if (date == nullptr || pattern == nullptr)
			{
				return 0;
			}

			std::tm now{ };

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			sscanf_s
			(
				date,
				pattern,
				&now.tm_year,
				&now.tm_mon,
				&now.tm_mday,
				&now.tm_hour,
				&now.tm_min,
				&now.tm_sec
			);

		#else

			sscanf
			(
				date,
				pattern,
				&now.tm_year,
				&now.tm_mon,
				&now.tm_mday,
				&now.tm_hour,
				&now.tm_min,
				&now.tm_sec
			);

		#endif

			now.tm_year -= 1900;
			now.tm_mon -= 1;

			return FromTm(now);
		}

		/**
		 *
		 * 格式化
		 *
		 * @param seconds 秒数时间戳
		 * @param pattern 格式
		 *
		 * @return 日期字符串
		 *
		 */
		std::string Time::Format(std::time_t seconds, const char * pattern)
		{
			return Format(seconds, Timezone(), pattern);
		}

		/**
		 *
		 * 格式化
		 *
		 * @param seconds 秒数时间戳
		 * @param timeZone 时区
		 * @param pattern 格式
		 *
		 * @return 日期字符串
		 *
		 */
		std::string Time::Format(std::time_t seconds, std::time_t timeZone, const char * pattern)
		{
			if (pattern == nullptr)
			{
				return { };
			}

			char str[128]{ 0 };

			std::tm now = UTCTm(seconds + (timeZone * 3600));

			snprintf
			(
				str,
				sizeof(str),
				pattern,
				now.tm_year + 1900,
				now.tm_mon + 1,
				now.tm_mday,
				now.tm_hour,
				now.tm_min,
				now.tm_sec
			);

			return str;
		}

		/**
		 *
		 * 当前UTC时间字符串
		 *
		 * @param pattern 格式
		 *
		 * @return 时间字符串
		 *
		 */
		std::string Time::UTCTimeString(const char * pattern)
		{
			return Format(Seconds(), 0, pattern);
		}

		/**
		 *
		 * 当前本地时间字符串
		 *
		 * @param pattern 格式
		 *
		 * @return 时间字符串
		 *
		 */
		std::string Time::LocalTimeString(const char * pattern)
		{
			return Format(Seconds(), pattern);
		}

		/**
		 *
		 * 时间点
		 *
		 * @return 时间点
		 *
		 */
		std::chrono::system_clock::time_point Time::TimePoint()
		{
			return std::chrono::system_clock::now();
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
		std::chrono::system_clock::time_point Time::TimePoint(std::time_t time)
		{
			return std::chrono::time_point_cast<std::chrono::system_clock::duration>
			(
				std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>
				(
					std::chrono::seconds(time)
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
		std::chrono::system_clock::time_point Time::TimePoint(const struct timeval & time)
		{
			return std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration>
			(
				std::chrono::duration_cast<std::chrono::system_clock::duration>
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
		std::chrono::system_clock::time_point Time::TimePoint(const struct timespec & time)
		{
			return std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration>
			(
				std::chrono::duration_cast<std::chrono::system_clock::duration>
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
		std::chrono::system_clock::duration Time::TimeDuration()
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
		std::chrono::system_clock::duration Time::TimeDuration(std::time_t time)
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
		std::chrono::system_clock::duration Time::TimeDuration(const struct timeval & time)
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
		std::chrono::system_clock::duration Time::TimeDuration(const struct timespec & time)
		{
			return TimePoint(time).time_since_epoch();
		}
	}
}
