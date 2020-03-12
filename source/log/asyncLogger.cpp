/**
 *
 *  作者: hm
 *
 *  说明: 异步日志
 *
 */


#include "type.h"
#include "asyncLogger.h"

#include "../util/time.h"
#include "../util/singleton.h"
#include "../util/application.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 构造函数
		 *
		 */
		AsyncLogger::AsyncLogger() : AsyncLogger(util::Application::Name())
		{

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
			_thread = std::thread
			(
				[this]()
				{
					while (true)
					{
						Context context{ };

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

							context = std::move(_queue.front());

							_queue.pop();
						}

						if (context.option == LOG_OPTION_TYPE::WRITE)
						{
							for (auto &iter : _sinks)
							{
								iter.second->Write(context);
							}
						}
						else if (context.option == LOG_OPTION_TYPE::FLUSH)
						{
							for (auto &iter : _sinks)
							{
								iter.second->Flush();
							}
						}
						else if (context.option == LOG_OPTION_TYPE::TERMINATE)
						{
							for (auto &iter : _sinks)
							{
								iter.second->Flush();
							}

							break;
						}
					}
				}
			);
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		AsyncLogger::~AsyncLogger()
		{
			ILogger::Write(LOG_OPTION_TYPE::TERMINATE);

			_thread.join();
		}

		/**
		 *
		 * 等待
		 *
		 */
		void AsyncLogger::Wait()
		{
			while (!_queue.empty())
			{
				std::this_thread::yield();
			}
		}

		/**
		 *
		 * 关闭
		 *
		 */
		void AsyncLogger::Close()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			for (auto &iter : _sinks)
			{
				iter.second->Close();
			}
		}

		/**
		 *
		 * 刷新
		 *
		 */
		void AsyncLogger::Flush()
		{
			ILogger::Write(LOG_OPTION_TYPE::FLUSH);
		}

		/**
		 *
		 * 写入内容
		 *
		 * @param context 上下文
		 *
		 */
		void AsyncLogger::Write(Context & context)
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);

				context.time = util::Time::TimePoint();

				std::time_t second = util::Time::Seconds(context.time);

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

					util::Time::LocalTm(second, _tm);  /// 重新处理
				}

				context.tm = _tm;

				_queue.push(context);
			}

			_condition.notify_one();
		}

		/**
		 *
		 * 单例对象
		 *
		 * @return 单例对象
		 *
		 */
		AsyncLogger & AsyncLogger::Instance()
		{
			return util::Singleton<AsyncLogger>::Instance();
		}
	}
}
