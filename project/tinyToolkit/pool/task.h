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
	class TINY_TOOLKIT_API TaskPool
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		~TaskPool();

		/**
		 *
		 * 等待
		 *
		 */
		void Wait();

		/**
		 *
		 * 暂停
		 *
		 */
		void Pause();

		/**
		 *
		 * 恢复
		 *
		 */
		void Resume();

		/**
		 *
		 * 终止
		 *
		 */
		void Terminate();

		/**
		 *
		 * 启动任务池
		 *
		 * @param count 任务池个数
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch(std::size_t count = std::thread::hardware_concurrency());

		/**
		 *
		 * 是否为空闲状态
		 *
		 * @return 是否为空闲状态
		 *
		 */
		bool IsIdle();

		/**
		 *
		 * 是否为暂停状态
		 *
		 * @return 是否为暂停状态
		 *
		 */
		bool IsPause();

		/**
		 *
		 * 是否为终止状态
		 *
		 * @return 是否为终止状态
		 *
		 */
		bool IsTerminated();

		/**
		 *
		 * 任务个数
		 *
		 * @return 任务个数
		 *
		 */
		std::size_t TaskSize();

		/**
		 *
		 * 线程个数
		 *
		 * @return 线程个数
		 *
		 */
		std::size_t ThreadSize();

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
		 * @return 操作结果对象
		 *
		 */
		template<typename FunctionTypeT, typename ... Args>
		std::future<typename std::result_of<FunctionTypeT(Args...)>::type> Submit(FunctionTypeT && function, Args &&... args)
		{
			using ResultType = typename std::result_of_t<FunctionTypeT(Args...)>;

			auto task = std::make_shared<std::packaged_task<ResultType()>>
			(
				std::bind(std::forward<FunctionTypeT>(function), std::forward<Args>(args)...)
			);

			std::future<ResultType> future = task->get_future();

			{
				std::lock_guard<std::mutex> lock(_mutex);

				if (_isTerminated)
				{
					throw std::runtime_error("taskPool already terminated");
				}

				_tasks.emplace
				(
					[task]()
					{
						(*task)();
					}
				);
			}

			_condition.notify_one();

			return future;
		};

	protected:
		/**
		 *
		 * 进程
		 *
		 */
		void Process();

	protected:
		bool _isPause{ false };
		bool _isTerminated{ false };

		std::mutex _mutex{ };

		std::condition_variable _condition{ };

		std::atomic<std::size_t> _idles{ 0 };

		std::vector<std::thread> _threads{ };

		std::queue<std::function<void()>> _tasks{ };
	};
}


#endif // __TINY_TOOLKIT__POOL__THREAD__H__
