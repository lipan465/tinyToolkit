#ifndef __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__
#define __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 同步日志
 *
 */


#include "logger.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <ctime>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API SyncLogger : public ILogger
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			SyncLogger();

			/**
			 *
			 * 构造函数
			 *
			 * @param name 名称
			 *
			 */
			explicit SyncLogger(std::string name);

			/**
			 *
			 * 析构函数
			 *
			 */
			~SyncLogger() override = default;

			/**
			 *
			 * 等待
			 *
			 */
			void Wait() override;

			/**
			 *
			 * 关闭
			 *
			 */
			void Close() override;

			/**
			 *
			 * 刷新
			 *
			 */
			void Flush() override;

			/**
			 *
			 * 写入内容
			 *
			 * @param context 上下文
			 *
			 */
			void Write(Context & context) override;

			/**
			 *
			 * 单例对象
			 *
			 * @return 单例对象
			 *
			 */
			static SyncLogger & Instance();

		private:
			std::tm _tm{ };

			std::time_t _second{ 0 };
			std::time_t _minutes{ 0 };
		};
	}
}


#define TINY_TOOLKIT_SYNC_LOG_DEBUG(fmt, ...)			TINY_TOOLKIT_LOG_DEBUG(tinyToolkit::log::SyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_INFO(fmt, ...)			TINY_TOOLKIT_LOG_INFO(tinyToolkit::log::SyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_NOTICE(fmt, ...)			TINY_TOOLKIT_LOG_NOTICE(tinyToolkit::log::SyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_WARNING(fmt, ...)			TINY_TOOLKIT_LOG_WARNING(tinyToolkit::log::SyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_ERROR(fmt, ...)			TINY_TOOLKIT_LOG_ERROR(tinyToolkit::log::SyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_CRITICAL(fmt, ...)		TINY_TOOLKIT_LOG_CRITICAL(tinyToolkit::log::SyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_ALERT(fmt, ...)			TINY_TOOLKIT_LOG_ALERT(tinyToolkit::log::SyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_FATAL(fmt, ...)			TINY_TOOLKIT_LOG_FATAL(tinyToolkit::log::SyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_EMERG(fmt, ...)			TINY_TOOLKIT_LOG_EMERG(tinyToolkit::log::SyncLogger::Instance(), fmt, ##__VA_ARGS__)


#endif // __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__
