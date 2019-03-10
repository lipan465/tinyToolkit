#ifndef __TINY_TOOLKIT__LOG__ASYNC_LOGGER__H__
#define __TINY_TOOLKIT__LOG__ASYNC_LOGGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志异步管理器
 *
 */


#include "logger.h"


namespace tinyToolkit
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
		 * @param name 日志名称
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
		 * 获取单例
		 *
		 * @return 单例对象引用
		 *
		 */
		static AsyncLogger & Instance();

		/**
		 *
		 * 等待日志写入
		 *
		 */
		void Wait() override;

	protected:
		/**
		 *
		 * 写入日志
		 *
		 * @param event 日志事件
		 *
		 */
		void Write(LogEvent & event) override;

		/**
		 *
		 * 日志处理线程函数
		 *
		 */
		void ThreadProcess();

	protected:
		std::tm _tm{ };
		std::time_t _second{ 0 };
		std::time_t _minutes{ 0 };

		std::thread _thread{ };

		std::queue<LogEvent> _queue{ };

		std::condition_variable _condition{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__ASYNC_LOGGER__H__
