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
		static bool IsSameYear(std::time_t sSeconds, std::time_t dSeconds);

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
		static bool IsSameMonth(std::time_t sSeconds, std::time_t dSeconds);

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
		static bool IsSameDay(std::time_t sSeconds, std::time_t dSeconds);

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
		static bool IsSameHour(std::time_t sSeconds, std::time_t dSeconds);

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
		static bool IsSameMinute(std::time_t sSeconds, std::time_t dSeconds);

		/**
		 *
		 * utc时间对应的tm结构体
		 *
		 * @return 时间结构体
		 *
		 */
		static std::tm UTCTm();

		/**
		 *
		 * utc时间对应的tm结构体
		 *
		 * @param seconds 秒
		 *
		 * @return 时间结构体
		 *
		 */
		static std::tm UTCTm(std::time_t seconds);

		/**
		 *
		 * utc时间对应的tm结构体
		 *
		 * @param seconds 秒
		 * @param tm 时间结构体
		 *
		 */
		static void UTCTm(std::time_t seconds, std::tm & tm);

		/**
		 *
		 * 本地时间对应的tm结构体
		 *
		 * @return 时间结构体
		 *
		 */
		static std::tm LocalTm();

		/**
		 *
		 * 本地时间对应的tm结构体
		 *
		 * @param seconds 秒
		 *
		 * @return 时间结构体
		 *
		 */
		static std::tm LocalTm(std::time_t seconds);

		/**
		 *
		 * 本地时间对应的tm结构体
		 *
		 * @param seconds 秒
		 * @param tm 时间结构体
		 *
		 */
		static void LocalTm(std::time_t seconds, std::tm & tm);

		/**
		 *
		 * 转换时间戳
		 *
		 * @tparam CastTypeT [all build-in type]
		 *
		 * @return 转换时间戳
		 *
		 */
		template <typename CastTypeT>
		static CastTypeT Cast()
		{
			return Cast<CastTypeT>(TimePoint());
		}

		/**
		 *
		 * 转换时间戳
		 *
		 * @tparam CastTypeT [all build-in type]
		 *
		 * @param timesPoint 时间点
		 *
		 * @return 转换时间戳
		 *
		 */
		template <typename CastTypeT>
		static CastTypeT Cast(const ClockTimePoint & timesPoint)
		{
			return Cast<CastTypeT>(timesPoint.time_since_epoch());
		}

		/**
		 *
		 * 转换时间戳
		 *
		 * @tparam CastTypeT [all build-in type]
		 *
		 * @param duration 时间段
		 *
		 * @return 转换时间戳
		 *
		 */
		template <typename CastTypeT>
		static CastTypeT Cast(const ClockDuration & duration)
		{
			return std::chrono::duration_cast<std::chrono::duration<CastTypeT>>(duration).count();
		}

		/**
		 *
		 * 小时时间戳
		 *
		 * @return 小时时间戳
		 *
		 */
		static std::time_t Hours();

		/**
		 *
		 * 小时时间戳
		 *
		 * @param timesPoint 时间点
		 *
		 * @return 小时时间戳
		 *
		 */
		static std::time_t Hours(const ClockTimePoint & timesPoint);

		/**
		 *
		 * 小时时间戳
		 *
		 * @param duration 时间段
		 *
		 * @return 小时时间戳
		 *
		 */
		static std::time_t Hours(const ClockDuration & duration);

		/**
		 *
		 * 分钟时间戳
		 *
		 * @return 分钟时间戳
		 *
		 */
		static std::time_t Minutes();

		/**
		 *
		 * 分钟时间戳
		 *
		 * @param timesPoint 时间点
		 *
		 * @return 分钟时间戳
		 *
		 */
		static std::time_t Minutes(const ClockTimePoint & timesPoint);

		/**
		 *
		 * 分钟时间戳
		 *
		 * @param duration 时间段
		 *
		 * @return 分钟时间戳
		 *
		 */
		static std::time_t Minutes(const ClockDuration & duration);

		/**
		 *
		 * 秒数时间戳
		 *
		 * @return 秒数时间戳
		 *
		 */
		static std::time_t Seconds();

		/**
		 *
		 * 秒数时间戳
		 *
		 * @param timesPoint 时间点
		 *
		 * @return 秒数时间戳
		 *
		 */
		static std::time_t Seconds(const ClockTimePoint & timesPoint);

		/**
		 *
		 * 秒数时间戳
		 *
		 * @param duration 时间段
		 *
		 * @return 秒数时间戳
		 *
		 */
		static std::time_t Seconds(const ClockDuration & duration);

		/**
		 *
		 * 毫秒时间戳
		 *
		 * @return 毫秒时间戳
		 *
		 */
		static std::time_t Milliseconds();

		/**
		 *
		 * 毫秒时间戳
		 *
		 * @param timesPoint 时间点
		 *
		 * @return 毫秒时间戳
		 *
		 */
		static std::time_t Milliseconds(const ClockTimePoint & timesPoint);

		/**
		 *
		 * 毫秒时间戳
		 *
		 * @param duration 时间段
		 *
		 * @return 毫秒时间戳
		 *
		 */
		static std::time_t Milliseconds(const ClockDuration & duration);

		/**
		 *
		 * 微秒时间戳
		 *
		 * @return 微秒时间戳
		 *
		 */
		static std::time_t Microseconds();

		/**
		 *
		 * 微秒时间戳
		 *
		 * @param timesPoint 时间点
		 *
		 * @return 微秒时间戳
		 *
		 */
		static std::time_t Microseconds(const ClockTimePoint & timesPoint);

		/**
		 *
		 * 微秒时间戳
		 *
		 * @param duration 时间段
		 *
		 * @return 微秒时间戳
		 *
		 */
		static std::time_t Microseconds(const ClockDuration & duration);

		/**
		 *
		 * 纳秒时间戳
		 *
		 * @return 纳秒时间戳
		 *
		 */
		static std::time_t Nanoseconds();

		/**
		 *
		 * 纳秒时间戳
		 *
		 * @param timesPoint 时间点
		 *
		 * @return 纳秒时间戳
		 *
		 */
		static std::time_t Nanoseconds(const ClockTimePoint & timesPoint);

		/**
		 *
		 * 纳秒时间戳
		 *
		 * @param duration 时间段
		 *
		 * @return 纳秒时间戳
		 *
		 */
		static std::time_t Nanoseconds(const ClockDuration & duration);

		/**
		 *
		 * 时区
		 *
		 * @return 时区
		 *
		 */
		static std::time_t TimeZone();

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
		static std::time_t NextDayTime(int32_t hour = 0, int32_t minutes = 0, int32_t seconds = 0);

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
		static std::time_t CurrentDayTime(int32_t hour = 0, int32_t minutes = 0, int32_t seconds = 0);

		/**
		 *
		 * tm转换成秒数时间戳
		 *
		 * @param date 时间结构体
		 *
		 * @return 秒数时间戳
		 *
		 */
		static std::time_t FromTm(std::tm & date);

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
		static std::time_t FromTimeString(const char * value, const char * format = "%4d-%02d-%02d %02d:%02d:%02d");

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
		static std::time_t FromTimeString(const std::string & value, const char * format = "%4d-%02d-%02d %02d:%02d:%02d");

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
		static std::string FormatTimeString(std::time_t seconds, const char * format = "%4d-%02d-%02d %02d:%02d:%02d");

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
		static std::string FormatTimeString(std::time_t seconds, std::time_t timeZone, const char * format = "%4d-%02d-%02d %02d:%02d:%02d");

		/**
		 *
		 * 格式化当前UTC日期字符串
		 *
		 * @param format 格式
		 *
		 * @return 日期字符串
		 *
		 */
		static std::string CurrentUTCTimeString(const char * format = "%4d-%02d-%02d %02d:%02d:%02d");

		/**
		 *
		 * 格式化当前本地日期字符串
		 *
		 * @param format 格式
		 *
		 * @return 日期字符串
		 *
		 */
		static std::string CurrentLocalTimeString(const char * format = "%4d-%02d-%02d %02d:%02d:%02d");

		/**
		 *
		 * 时间点
		 *
		 * @return 时间点
		 *
		 */
		static ClockTimePoint TimePoint();

		/**
		 *
		 * 时间点
		 *
		 * @param seconds 秒数时间戳
		 *
		 * @return 时间点
		 *
		 */
		static ClockTimePoint TimePoint(std::time_t time);

		/**
		 *
		 * 时间点
		 *
		 * @param time 时间结构体
		 *
		 * @return 时间点
		 *
		 */
		static ClockTimePoint TimePoint(const struct timeval & time);

		/**
		 *
		 * 时间点
		 *
		 * @param time 时间结构体
		 *
		 * @return 时间点
		 *
		 */
		static ClockTimePoint TimePoint(const struct timespec & time);

		/**
		 *
		 * 时间段
		 *
		 * @return 时间段
		 *
		 */
		static ClockDuration TimeDuration();

		/**
		 *
		 * 时间段
		 *
		 * @param time 秒数时间戳
		 *
		 * @return 时间段
		 *
		 */
		static ClockDuration TimeDuration(std::time_t time);

		/**
		 *
		 * 时间段
		 *
		 * @param time 时间结构体
		 *
		 * @return 时间段
		 *
		 */
		static ClockDuration TimeDuration(const struct timeval & time);

		/**
		 *
		 * 时间段
		 *
		 * @param time 时间结构体
		 *
		 * @return 时间段
		 *
		 */
		static ClockDuration TimeDuration(const struct timespec & time);
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__TIME__H__
