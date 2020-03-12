#ifndef __TINY_TOOLKIT__LOG__ASYNC_LOGGER__H__
#define __TINY_TOOLKIT__LOG__ASYNC_LOGGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 异步日志
 *
 */


#include "logger.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <ctime>
#  include <queue>
#  include <thread>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <queue>
#  include <thread>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <queue>
#  include <thread>
#  include <condition_variable>
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API AsyncLogger : public ILogger
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			AsyncLogger();

			/**
			 *
			 * 构造函数
			 *
			 * @param name 名称
			 *
			 */
			explicit AsyncLogger(std::string name);

			/**
			 *
			 * 析构函数
			 *
			 */
			~AsyncLogger() override;

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
			static AsyncLogger & Instance();

		private:
			std::tm _tm{ };

			std::time_t _second{ 0 };
			std::time_t _minutes{ 0 };

			std::thread _thread{ };

			std::queue<Context> _queue{ };

			std::condition_variable _condition{ };
		};
	}
}


#define TINY_TOOLKIT_ASYNC_LOG_DEBUG(fmt, ...)			TINY_TOOLKIT_LOG_DEBUG(tinyToolkit::log::AsyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_ASYNC_LOG_INFO(fmt, ...)			TINY_TOOLKIT_LOG_INFO(tinyToolkit::log::AsyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_ASYNC_LOG_NOTICE(fmt, ...)			TINY_TOOLKIT_LOG_NOTICE(tinyToolkit::log::AsyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_ASYNC_LOG_WARNING(fmt, ...)		TINY_TOOLKIT_LOG_WARNING(tinyToolkit::log::AsyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_ASYNC_LOG_ERROR(fmt, ...)			TINY_TOOLKIT_LOG_ERROR(tinyToolkit::log::AsyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_ASYNC_LOG_CRITICAL(fmt, ...)		TINY_TOOLKIT_LOG_CRITICAL(tinyToolkit::log::AsyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_ASYNC_LOG_ALERT(fmt, ...)			TINY_TOOLKIT_LOG_ALERT(tinyToolkit::log::AsyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_ASYNC_LOG_FATAL(fmt, ...)			TINY_TOOLKIT_LOG_FATAL(tinyToolkit::log::AsyncLogger::Instance(), fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_ASYNC_LOG_EMERG(fmt, ...)			TINY_TOOLKIT_LOG_EMERG(tinyToolkit::log::AsyncLogger::Instance(), fmt, ##__VA_ARGS__)


#endif // __TINY_TOOLKIT__LOG__ASYNC_LOGGER__H__
