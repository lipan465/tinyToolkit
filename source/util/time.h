#ifndef __TINY_TOOLKIT__UTIL__TIME__H__
#define __TINY_TOOLKIT__UTIL__TIME__H__


/**
 *
 *  作者: hm
 *
 *  说明: 时间处理
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <ctime>
#  include <string>
#  include <chrono>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <string>
#  include <chrono>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <string>
#  include <chrono>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		class TINY_TOOLKIT_API Time
		{
		public:
			/**
			 *
			 * 时间结构体
			 *
			 * @return 时间结构体
			 *
			 */
			static std::tm UTCTm();

			/**
			 *
			 * 时间结构体
			 *
			 * @param seconds 秒数时间戳
			 *
			 * @return 时间结构体
			 *
			 */
			static std::tm UTCTm(std::time_t seconds);

			/**
			 *
			 * 时间结构体
			 *
			 * @param seconds 秒数时间戳
			 * @param tm 时间结构体
			 *
			 */
			static void UTCTm(std::time_t seconds, std::tm & tm);

			/**
			 *
			 * 时间结构体
			 *
			 * @return 时间结构体
			 *
			 */
			static std::tm LocalTm();

			/**
			 *
			 * 时间结构体
			 *
			 * @param seconds 秒数时间戳
			 *
			 * @return 时间结构体
			 *
			 */
			static std::tm LocalTm(std::time_t seconds);

			/**
			 *
			 * 时间结构体
			 *
			 * @param seconds 秒数时间戳
			 * @param tm 时间结构体
			 *
			 */
			static void LocalTm(std::time_t seconds, std::tm & tm);

			/**
			 *
			 * 时区
			 *
			 * @return 时区
			 *
			 */
			static std::time_t Timezone();

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
			static std::time_t Hours(const std::chrono::system_clock::time_point & timesPoint);

			/**
			 *
			 * 小时时间戳
			 *
			 * @param duration 时间段
			 *
			 * @return 小时时间戳
			 *
			 */
			static std::time_t Hours(const std::chrono::system_clock::duration & duration);

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
			static std::time_t Minutes(const std::chrono::system_clock::time_point & timesPoint);

			/**
			 *
			 * 分钟时间戳
			 *
			 * @param duration 时间段
			 *
			 * @return 分钟时间戳
			 *
			 */
			static std::time_t Minutes(const std::chrono::system_clock::duration & duration);

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
			static std::time_t Seconds(const std::chrono::system_clock::time_point & timesPoint);

			/**
			 *
			 * 秒数时间戳
			 *
			 * @param duration 时间段
			 *
			 * @return 秒数时间戳
			 *
			 */
			static std::time_t Seconds(const std::chrono::system_clock::duration & duration);

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
			static std::time_t Milliseconds(const std::chrono::system_clock::time_point & timesPoint);

			/**
			 *
			 * 毫秒时间戳
			 *
			 * @param duration 时间段
			 *
			 * @return 毫秒时间戳
			 *
			 */
			static std::time_t Milliseconds(const std::chrono::system_clock::duration & duration);

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
			static std::time_t Microseconds(const std::chrono::system_clock::time_point & timesPoint);

			/**
			 *
			 * 微秒时间戳
			 *
			 * @param duration 时间段
			 *
			 * @return 微秒时间戳
			 *
			 */
			static std::time_t Microseconds(const std::chrono::system_clock::duration & duration);

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
			static std::time_t Nanoseconds(const std::chrono::system_clock::time_point & timesPoint);

			/**
			 *
			 * 纳秒时间戳
			 *
			 * @param duration 时间段
			 *
			 * @return 纳秒时间戳
			 *
			 */
			static std::time_t Nanoseconds(const std::chrono::system_clock::duration & duration);

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
			static std::time_t DayTime(int64_t day = 0, int32_t hour = 0, int32_t minutes = 0, int32_t seconds = 0);

			/**
			 *
			 * 转换为秒数时间戳
			 *
			 * @param tm 时间结构体
			 *
			 * @return 秒数时间戳
			 *
			 */
			static std::time_t FromTm(std::tm & tm);

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
			static std::time_t FromString(const char * date, const char * pattern = "%4d-%02d-%02d %02d:%02d:%02d");

			/**
			 *
			 * 格式化时间字符串
			 *
			 * @param seconds 秒数时间戳
			 * @param pattern 格式
			 *
			 * @return 时间字符串
			 *
			 */
			static std::string Format(std::time_t seconds, const char * pattern = "%4d-%02d-%02d %02d:%02d:%02d");

			/**
			 *
			 * 格式化时间字符串
			 *
			 * @param seconds 秒数时间戳
			 * @param timeZone 时区
			 * @param pattern 格式
			 *
			 * @return 时间字符串
			 *
			 */
			static std::string Format(std::time_t seconds, std::time_t timeZone, const char * pattern = "%4d-%02d-%02d %02d:%02d:%02d");

			/**
			 *
			 * 当前UTC时间字符串
			 *
			 * @param pattern 格式
			 *
			 * @return 时间字符串
			 *
			 */
			static std::string UTCTimeString(const char * pattern = "%4d-%02d-%02d %02d:%02d:%02d");

			/**
			 *
			 * 当前本地时间字符串
			 *
			 * @param pattern 格式
			 *
			 * @return 时间字符串
			 *
			 */
			static std::string LocalTimeString(const char * pattern = "%4d-%02d-%02d %02d:%02d:%02d");

			/**
			 *
			 * 时间点
			 *
			 * @return 时间点
			 *
			 */
			static std::chrono::system_clock::time_point TimePoint();

			/**
			 *
			 * 时间点
			 *
			 * @param seconds 秒数时间戳
			 *
			 * @return 时间点
			 *
			 */
			static std::chrono::system_clock::time_point TimePoint(std::time_t time);

			/**
			 *
			 * 时间点
			 *
			 * @param time 时间结构体
			 *
			 * @return 时间点
			 *
			 */
			static std::chrono::system_clock::time_point TimePoint(const struct timeval & time);

			/**
			 *
			 * 时间点
			 *
			 * @param time 时间结构体
			 *
			 * @return 时间点
			 *
			 */
			static std::chrono::system_clock::time_point TimePoint(const struct timespec & time);

			/**
			 *
			 * 时间段
			 *
			 * @return 时间段
			 *
			 */
			static std::chrono::system_clock::duration TimeDuration();

			/**
			 *
			 * 时间段
			 *
			 * @param time 秒数时间戳
			 *
			 * @return 时间段
			 *
			 */
			static std::chrono::system_clock::duration TimeDuration(std::time_t time);

			/**
			 *
			 * 时间段
			 *
			 * @param time 时间结构体
			 *
			 * @return 时间段
			 *
			 */
			static std::chrono::system_clock::duration TimeDuration(const struct timeval & time);

			/**
			 *
			 * 时间段
			 *
			 * @param time 时间结构体
			 *
			 * @return 时间段
			 *
			 */
			static std::chrono::system_clock::duration TimeDuration(const struct timespec & time);
		};
	}
}


#endif // __TINY_TOOLKIT__UTIL__TIME__H__
