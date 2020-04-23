/**
 *
 *  作者: hm
 *
 *  说明: 任务池
 *
 */


#include "task.h"

#include "../container/operation.h"


namespace tinyToolkit
{
	namespace pool
	{
		/**
		 *
		 * 析构函数
		 *
		 */
		TaskPool::~TaskPool()
		{
			Close();

			for (auto &thread : _threads)
			{
				if (thread.joinable())
				{
					thread.join();
				}
			}

			container::Operation::Swap(_tasks);
			container::Operation::Swap(_threads);
		}

		/**
		 *
		 * 等待
		 *
		 */
		void TaskPool::Wait()
		{
			while (!IsIdle())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}

		/**
		 *
		 * 关闭
		 *
		 */
		void TaskPool::Close()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_isClose = true;

			_condition.notify_all();
		}

		/**
		 *
		 * 暂停
		 *
		 */
		void TaskPool::Pause()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_isPause = true;
		}

		/**
		 *
		 * 恢复
		 *
		 */
		void TaskPool::Resume()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_isPause = false;

			_condition.notify_all();
		}

		/**
		 *
		 * 启动任务池
		 *
		 * @param count 线程个数
		 *
		 * @return 是否启动成功
		 *
		 */
		bool TaskPool::Launch(size_t count)
		{
			if (count == 0)
			{
				return false;
			}

			if (!_threads.empty())
			{
				return false;
			}

			if (_isPause || _isClose)
			{
				return false;
			}

			_idles.store(count);

			for (std::size_t i = 0; i < count; ++i)
			{
				_threads.emplace_back
				(
					[this]()
					{
						std::function<void()> task{ };

						while (true)
						{
							{
								std::unique_lock<std::mutex> lock(_mutex);

								_condition.wait
								(
									lock,

									[this]
									{
										if (_tasks.empty())
										{
											return _isClose;
										}
										else
										{
											return !_isPause;
										}
									}
								);

								if (_isClose)
								{
									return;
								}

								task = std::move(_tasks.front());

								_tasks.pop();
							}

							--_idles;

							task();

							++_idles;
						}
					}
				);
			}

			_isValid = true;

			return true;
		}

		/**
		 *
		 * 是否为空闲状态
		 *
		 * @return 是否为空闲状态
		 *
		 */
		bool TaskPool::IsIdle() const
		{
			return _tasks.empty() && _idles == _threads.size();
		}

		/**
		 *
		 * 是否为关闭状态
		 *
		 * @return 是否为关闭状态
		 *
		 */
		bool TaskPool::IsClose() const
		{
			return _isClose;
		}

		/**
		 *
		 * 是否为暂停状态
		 *
		 * @return 是否为暂停状态
		 *
		 */
		bool TaskPool::IsPause() const
		{
			return _isPause;
		}

		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool TaskPool::IsValid() const
		{
			return _isValid;
		}

		/**
		 *
		 * 任务个数
		 *
		 * @return 任务个数
		 *
		 */
		std::size_t TaskPool::TaskSize() const
		{
			return _tasks.size();
		}

		/**
		 *
		 * 线程个数
		 *
		 * @return 线程个数
		 *
		 */
		std::size_t TaskPool::ThreadSize() const
		{
			return _threads.size();
		}
	}
}
