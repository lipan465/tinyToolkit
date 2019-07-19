/**
 *
 *  作者: hm
 *
 *  说明: 线程池
 *
 */


#include "task.h"

#include "../utilities/operator.h"


namespace tinyToolkit
{
	/**
	 *
	 * 析构函数
	 *
	 */
	TaskPool::~TaskPool()
	{
		Terminate();

		for (auto &thread : _threads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}

		Operator::Clear(_tasks);
		Operator::Clear(_threads);
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
			TINY_TOOLKIT_SLEEP_MS(10);
		}
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
	 * 终止
	 *
	 */
	void TaskPool::Terminate()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_isTerminated = true;

		_condition.notify_all();
	}

	/**
	 *
	 * 启动任务池
	 *
	 * @param count 任务池个数
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

		if (_isPause || _isTerminated)
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
					Process();
				}
			);
		}

		return true;
	}

	/**
	 *
	 * 是否为空闲状态
	 *
	 * @return 是否为空闲状态
	 *
	 */
	bool TaskPool::IsIdle()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		return _tasks.empty() && _idles == _threads.size();
	}

	/**
	 *
	 * 是否为暂停状态
	 *
	 * @return 是否为暂停状态
	 *
	 */
	bool TaskPool::IsPause()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		return _isPause;
	}

	/**
	 *
	 * 是否为终止状态
	 *
	 * @return 是否为终止状态
	 *
	 */
	bool TaskPool::IsTerminated()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		return _isTerminated;
	}

	/**
	 *
	 * 任务个数
	 *
	 * @return 任务个数
	 *
	 */
	std::size_t TaskPool::TaskSize()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		return _tasks.size();
	}

	/**
	 *
	 * 线程个数
	 *
	 * @return 线程个数
	 *
	 */
	std::size_t TaskPool::ThreadSize()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		return _threads.size();
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 进程
	 *
	 */
	void TaskPool::Process()
	{
		std::function<void()> task;

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
							return _isTerminated;
						}
						else
						{
							return !_isPause;
						}
					}
				);

				if (_isTerminated)
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
}
