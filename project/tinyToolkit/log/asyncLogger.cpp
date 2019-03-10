/**
 *
 *  作者: hm
 *
 *  说明: 日志异步管理器
 *
 */


#include "asyncLogger.h"

#include "../utilities/time.h"
#include "../utilities/singleton.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	AsyncLogger::AsyncLogger() : ILogger()
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
	AsyncLogger::AsyncLogger(std::string name) : ILogger(std::move(name))
	{
		_thread = std::thread(&AsyncLogger::ThreadProcess, this);
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	AsyncLogger::~AsyncLogger()
	{
		LogEvent event(_name, LOG_OPTION_TYPE::TERMINATE);

		Write(event);

		_thread.join();
	}

	/**
	 *
	 * 获取单例
	 *
	 * @return 单例对象引用
	 *
	 */
	AsyncLogger & AsyncLogger::Instance()
	{
		return Singleton<AsyncLogger>::Instance();
	}

	/**
	 *
	 * 等待日志写入
	 *
	 */
	void AsyncLogger::Wait()
	{
		while (!_queue.empty())
		{
			TINY_TOOLKIT_YIELD();
		}
	}

	/**
	 *
	 * 写入日志
	 *
	 * @param event 日志事件
	 *
	 */
	void AsyncLogger::Write(LogEvent & event)
	{
		{
			std::lock_guard<std::mutex> lock(_mutex);

			event.time = Time::TimePoint();

			std::time_t second = Time::Seconds(event.time);

			if (second == _second)  /// 同一秒生成的日志
			{
				/// 不需要处理
			}
			else if (second / 60 == _minutes)  /// 同一分钟生成的日志
			{
				_second = second;

				_tm.tm_sec = static_cast<int32_t>(_second % 60);  /// 更新秒
			}
			else  /// 不同分钟生成的日志
			{
				_minutes = second / 60;

				Time::LocalTm(second, _tm);  /// 重新处理
			}

			event.tm = _tm;

			_queue.push(event);
		}

		_condition.notify_one();
	}

	/**
	 *
	 * 日志处理线程函数
	 *
	 */
	void AsyncLogger::ThreadProcess()
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
}
