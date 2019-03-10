/**
 *
 *  作者: hm
 *
 *  说明: 线程池
 *
 */


#include "thread.h"

#include "../utilities/operator.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param size 线程数
	 *
	 */
	ThreadPool::ThreadPool(std::size_t size)
	{
		_isClose.store(false);

		_freeSize.store(size);

		Create(size);
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	ThreadPool::~ThreadPool()
	{
		Release();
	}

	/**
	 *
	 * 释放
	 *
	 */
	void ThreadPool::Release()
	{
		if (_isClose.load())
		{
			return;
		}

		_isClose.store(true);

		_condition.notify_all();

		for (auto &thread : _pool)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}

		Operator::Clear(_pool);
	}

	/**
	 *
	 * 等待
	 *
	 */
	void ThreadPool::Wait()
	{
		while (!IsComplete())
		{
			TINY_TOOLKIT_SLEEP_MS(10);
		}
	}

	/**
	 *
	 * 是否执行完毕
	 *
	 * @return 状态
	 *
	 */
	bool ThreadPool::IsComplete()
	{
		return _tasks.empty() && _freeSize.load() == _pool.size();
	}

	/**
	 *
	 * 是否关闭
	 *
	 * @return 状态
	 *
	 */
	bool ThreadPool::IsClose() const
	{
		return _isClose.load();
	}

	/**
	 *
	 * 任务个数
	 *
	 * @return 任务个数
	 *
	 */
	std::size_t ThreadPool::TaskSize() const
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
	std::size_t ThreadPool::ThreadSize() const
	{
		return _pool.size();
	}

	/**
	 *
	 * 创建线程
	 *
	 * @param size 线程个数
	 *
	 */
	void ThreadPool::Create(std::size_t size)
	{
		for (std::size_t i = 0; i < size; ++i)
		{
			_pool.emplace_back
			(
				[this]
				{
					while (true)
					{
						std::function<void()> task;

						{
							std::unique_lock<std::mutex> lock(_mutex);

							_condition.wait
							(
								lock,

								[this]
								{
									return !_tasks.empty() || _isClose.load();
								}
							);

							if (_isClose.load())
							{
								return false;
							}

							task = std::move(_tasks.front());

							_tasks.pop();
						}

						--_freeSize;

						task();

						++_freeSize;
					}
				}
			);
		}
	}
}
