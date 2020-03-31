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


#define TINY_TOOLKIT_SYNC_LOG_DEBUG(fmt, ...)			tinyToolkit::log::SyncLogger::Instance().Debug(fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_INFO(fmt, ...)			tinyToolkit::log::SyncLogger::Instance().Info(fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_NOTICE(fmt, ...)			tinyToolkit::log::SyncLogger::Instance().Notice(fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_WARNING(fmt, ...)		    tinyToolkit::log::SyncLogger::Instance().Warning(fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_ERROR(fmt, ...)			tinyToolkit::log::SyncLogger::Instance().Error(fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_CRITICAL(fmt, ...)		tinyToolkit::log::SyncLogger::Instance().Critical(fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_ALERT(fmt, ...)			tinyToolkit::log::SyncLogger::Instance().Alert(fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_FATAL(fmt, ...)			tinyToolkit::log::SyncLogger::Instance().Fatal(fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_SYNC_LOG_EMERG(fmt, ...)			tinyToolkit::log::SyncLogger::Instance().Emerg(fmt, ##__VA_ARGS__)


#endif // __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__
