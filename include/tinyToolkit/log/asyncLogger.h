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
		AsyncLogger() : ILogger()
		{
			_thread = std::thread(&AsyncLogger::ThreadProcess, this);
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param name 日志名称
		 *
		 */
		explicit AsyncLogger(std::string name) : ILogger(std::move(name))
		{
			_thread = std::thread(&AsyncLogger::ThreadProcess, this);
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~AsyncLogger() override
		{
			Write(LogEvent(_name, LOG_OPTION_TYPE::TERMINATE));

			_thread.join();
		}

		/**
		 *
		 * 获取单例
		 *
		 * @return 单例对象引用
		 *
		 */
		static AsyncLogger & Instance()
		{
			return Singleton<AsyncLogger>::Instance();
		}

		/**
		 *
		 * 等待日志写入
		 *
		 */
		void Wait() override
		{
			while (!_queue.empty())
			{
				TINY_TOOLKIT_YIELD()
			}
		}

	protected:
		/**
		 *
		 * 写入日志
		 *
		 * @param event 日志事件
		 *
		 */
		void Write(const LogEvent & event) override
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);

				_queue.push(event);
			}

			_condition.notify_one();
		}

		/**
		 *
		 * 日志处理线程函数
		 *
		 */
		void ThreadProcess()
		{
			while (true)
			{
				LogEvent event;

				{
					std::unique_lock<std::mutex> lock(_mutex);

					_condition.wait
					(
						lock,

						[this]
						{
							return !_queue.empty();
						}
					);

					event = std::move(_queue.front());

					_queue.pop();
				}

				if (event.option == LOG_OPTION_TYPE::WRITE)
				{
					for (auto &iter : _container)
					{
						iter.second->Write(event);
					}
				}
				else if (event.option == LOG_OPTION_TYPE::FLUSH)
				{
					for (auto &iter : _container)
					{
						iter.second->Flush();
					}
				}
				else if (event.option == LOG_OPTION_TYPE::TERMINATE)
				{
					for (auto &iter : _container)
					{
						iter.second->Flush();
					}

					break;
				}
			}
		}

	protected:
		std::thread _thread{ };

		std::queue<LogEvent> _queue{ };

		std::condition_variable _condition{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__ASYNC_LOGGER__H__
