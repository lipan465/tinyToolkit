#ifndef __TINY_TOOLKIT__POOL__TASK__H__
#define __TINY_TOOLKIT__POOL__TASK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 任务池
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <queue>
#  include <future>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <queue>
#  include <future>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <queue>
#  include <future>
#  include <functional>
#
#endif


namespace tinyToolkit
{
	namespace pool
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
			 * 关闭
			 *
			 */
			void Close();

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
			 * 启动任务池
			 *
			 * @param count 线程个数
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
			 * 是否为关闭状态
			 *
			 * @return 是否为关闭状态
			 *
			 */
			bool IsClose();

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
			 * 是否有效
			 *
			 * @return 是否有效
			 *
			 */
			bool IsValid();

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
			 * 添加任务
			 *
			 * @param function 函数
			 * @param args 参数
			 *
			 * @return 操作结果对象
			 *
			 */
			template<typename FunctionTypeT, typename ... Args>
			std::future<typename std::result_of<FunctionTypeT(Args...)>::type> AddTask(FunctionTypeT && function, Args &&... args)
			{
				using ResultType = typename std::result_of_t<FunctionTypeT(Args...)>;

				auto task = std::make_shared<std::packaged_task<ResultType()>>
				(
					std::bind(std::forward<FunctionTypeT>(function), std::forward<Args>(args)...)
				);

				std::future<ResultType> future = task->get_future();

				{
					std::lock_guard<std::mutex> lock(_mutex);

					if (_isClose)
					{
						throw std::runtime_error("Task Pool already close");
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

		private:
			bool _isValid{ false };
			bool _isClose{ false };
			bool _isPause{ false };

			std::mutex _mutex{ };

			std::condition_variable _condition{ };

			std::atomic<std::size_t> _idles{ 0 };

			std::vector<std::thread> _threads{ };

			std::queue<std::function<void()>> _tasks{ };
		};
	}
}


#endif // __TINY_TOOLKIT__POOL__TASK__H__
