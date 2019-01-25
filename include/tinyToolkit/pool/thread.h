#ifndef __TINY_TOOLKIT__POOL__THREAD__H__
#define __TINY_TOOLKIT__POOL__THREAD__H__


/**
 *
 *  作者: hm
 *
 *  说明: 线程池
 *
 */


#include "../common/common.h"


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
		explicit ThreadPool(std::size_t size = std::thread::hardware_concurrency());

		/**
		 *
		 * 析构函数
		 *
		 */
		~ThreadPool();

		/**
		 *
		 * 提交任务
		 *
		 * @tparam FunctionTypeT [function types]
		 * @tparam Args [all types]
		 *
		 * @param function 函数
		 * @param args 参数
		 *
		 */
		template<typename FunctionTypeT, typename ... Args>
		void Commit(FunctionTypeT && function, Args &&... args)
		{
			auto task = std::bind(std::forward<FunctionTypeT>(function), std::forward<Args>(args)...);

			{
				std::lock_guard<std::mutex> lock(_mutex);

				_tasks.emplace
				(
					[task]()
					{
						task();
					}
				);
			}

			_condition.notify_one();
		};

		/**
		 *
		 * 释放
		 *
		 */
		void Release();

		/**
		 *
		 * 等待
		 *
		 */
		void Wait();

		/**
		 *
		 * 是否执行完毕
		 *
		 * @return 状态
		 *
		 */
		bool IsComplete();

		/**
		 *
		 * 是否关闭
		 *
		 * @return 状态
		 *
		 */
		bool IsClose() const;

		/**
		 *
		 * 任务个数
		 *
		 * @return 任务个数
		 *
		 */
		std::size_t TaskSize() const;

		/**
		 *
		 * 线程个数
		 *
		 * @return 线程个数
		 *
		 */
		std::size_t ThreadSize() const;

	protected:
		/**
		 *
		 * 创建线程
		 *
		 * @param size 线程个数
		 *
		 */
		void Create(std::size_t size);

	protected:
		std::mutex _mutex{ };

		std::condition_variable _condition{ };

		std::atomic<bool> _isClose{ false };

		std::vector<std::thread> _pool{ };

		std::atomic<std::size_t> _freeSize{ 0 };

		std::queue<std::function<void()>> _tasks{ };
	};
}


#endif // __TINY_TOOLKIT__POOL__THREAD__H__
