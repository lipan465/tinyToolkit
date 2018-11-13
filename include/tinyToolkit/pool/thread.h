#ifndef __TINY_TOOLKIT__POOL__THREAD__H__
#define __TINY_TOOLKIT__POOL__THREAD__H__


/**
 *
 *  作者: hm
 *
 *  说明: 线程池
 *
 */


#include "../utilities/container.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ThreadPool
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param size 线程数
		 *
		 */
		explicit ThreadPool(std::size_t size = std::thread::hardware_concurrency())
		{
			_isClose.store(false);

			_freeSize.store(size);
			_threadSize.store(size);

			Create();
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~ThreadPool()
		{
			Release();
		}

		/**
		 *
		 * 提交任务
		 *
		 * @tparam Func [function]
		 * @tparam Args [all types]
		 *
		 * @param func 待运行函数
		 * @param args 待运行函数参数
		 *
		 * @return future对象(调用.get()获取返回值会等待任务执行完)
		 *
		 */
		template<class Func, class... Args>
		inline decltype(auto) Commit(Func && func, Args &&... args)
		{
			using RetType = decltype(func(args...));

			auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<Func>(func),
																				  std::forward<Args>(args)...));

			auto res = task->get_future();

			{
				std::lock_guard<std::mutex> lock(_lock);

				_tasks.emplace
				(
					[task]()
					{
						(*task)();
					}
				);
			}

			_condition.notify_one();

			return res;
		};

		/**
		 *
		 * 销毁线程池
		 *
		 */
		void Release()
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

			tinyToolkit::Container::Clear(_pool);
		}

		/**
		 *
		 * 等待所有线程执行
		 *
		 */
		void Wait()
		{
			while (!IsComplete())
			{
				TINY_TOOLKIT_SLEEP_MS(10)
			}
		}

		/**
		 *
		 * 线程池是否执行完毕
		 *
		 * @return 状态
		 *
		 */
		bool IsComplete()
		{
			return _tasks.empty() && _freeSize == _threadSize;
		}

		/**
		 *
		 * 线程池是否开启
		 *
		 * @return 状态
		 *
		 */
		bool IsStart() const
		{
			return !_isClose.load();
		}

		/**
		 *
		 * 线程池是否关闭
		 *
		 * @return 状态
		 *
		 */
		bool IsClose() const
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
		std::size_t TaskSize() const
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
		std::size_t ThreadSize() const
		{
			return _threadSize.load();
		}

	protected:
		/**
		 *
		 * 创建线程
		 *
		 */
		void Create()
		{
			for (std::size_t i = 0; i < _threadSize.load(); ++i)
			{
				_pool.emplace_back
				(
					[this]
					{
						while (true)
						{
							std::function<void()> task;

							{
								std::unique_lock<std::mutex> lock(_lock);

								_condition.wait
								(
									lock,

									[this]
									{
										TINY_TOOLKIT_SLEEP_MS(10)

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

	protected:
		std::mutex _lock{ };

		std::condition_variable _condition{ };

		std::atomic<bool> _isClose{ false };

		std::vector<std::thread> _pool{ };

		std::atomic<std::size_t> _freeSize{ 0 };
		std::atomic<std::size_t> _threadSize{ 0 };

		std::queue<std::function<void()>> _tasks{ };
	};
}


#endif // __TINY_TOOLKIT__POOL__THREAD__H__
