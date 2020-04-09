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


#endif // __TINY_TOOLKIT__LOG__ASYNC_LOGGER__H__
