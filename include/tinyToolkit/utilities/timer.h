#ifndef __TINY_TOOLKIT__UTILITIES__TIMER__H__
#define __TINY_TOOLKIT__UTILITIES__TIMER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 定时器
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	#define TIMER_WHEEL_NUM  4
	#define TIMER_NEAR_BITS  8
	#define TIMER_WHEEL_BITS 6

	#define TIMER_NEAR_SIZE   (1 << TIMER_NEAR_BITS)
	#define TIMER_WHEEL_SIZE  (1 << TIMER_WHEEL_BITS)

	#define TIMER_NEAR_MASK   (TIMER_NEAR_SIZE  - 1)
	#define TIMER_WHEEL_MASK  (TIMER_WHEEL_SIZE - 1)

	class TINY_TOOLKIT_API Timer
	{
		typedef struct TimerNode
		{
			bool circle{ false };

			std::time_t expire{ 0 };
			std::time_t interval{ 0 };

			std::function<void()> callback;
		}TimerNode;

		using TimerSpokes  = std::vector<TimerNode *>;

	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		Timer();

		/**
		 *
		 * 析构函数
		 *
		 */
		~Timer();

		/**
		 *
		 * 提交单次任务
		 *
		 * @tparam FunctionTypeT [function types]
		 * @tparam Args [all types]
		 *
		 * @param interval 定时间隔
		 * @param function 函数
		 * @param args 参数
		 *
		 */
		template<typename FunctionTypeT, typename ... Args>
		void CommitOnce(std::time_t interval, FunctionTypeT && function, Args &&... args)
		{
			Commit(false, interval, std::forward<FunctionTypeT>(function), std::forward<Args>(args)...);
		}

		/**
		 *
		 * 提交循环任务
		 *
		 * @tparam FunctionTypeT [function types]
		 * @tparam Args [all types]
		 *
		 * @param interval 定时间隔
		 * @param function 函数
		 * @param args 参数
		 *
		 */
		template<typename FunctionTypeT, typename ... Args>
		void CommitCircle(std::time_t interval, FunctionTypeT && function, Args &&... args)
		{
			Commit(true, interval, std::forward<FunctionTypeT>(function), std::forward<Args>(args)...);
		}

		/**
		 *
		 * 提交任务
		 *
		 * @tparam FunctionTypeT [function types]
		 * @tparam Args [all types]
		 *
		 * @param circle 是否循环执行
		 * @param interval 定时间隔
		 * @param function 函数
		 * @param args 参数
		 *
		 */
		template<typename FunctionTypeT, typename ... Args>
		void Commit(bool circle, std::time_t interval, FunctionTypeT && function, Args &&... args)
		{
			auto task = std::bind(std::forward<FunctionTypeT>(function), std::forward<Args>(args)...);

			{
				std::lock_guard<std::mutex> lock(_lock);

				auto * node = new TimerNode;

				node->circle   = circle;
				node->expire   = interval + _tick.load();
				node->interval = interval;
				node->callback = [task]() { task(); };

				AddNode(node);
			}
		}

		/**
		 *
		 * 关闭定时器
		 *
		 */
		void Close();

		/**
		 *
		 * 定时器是否启动
		 *
		 * @return 状态
		 *
		 */
		bool Status();

	protected:
		/**
		 *
		 * 线程函数(更新定时器tick)
		 *
		 */
		void ThreadProcess();

		/**
		 *
		 * 更新定时器tick
		 *
		 */
		void Update();

		/**
		 *
		 * 执行定时器
		 *
		 */
		void Executes();

		/**
		 *
		 * 添加定时器
		 *
		 * @param node 待添加定时器
		 *
		 */
		void AddNode(TimerNode * node);

		/**
		 *
		 * 清理时间轮
		 *
		 * @param spokesList 待清理时间轮
		 *
		 */
		void ClearSpokes(TimerSpokes & spokesList);

		/**
		 *
		 * 转动时间轮
		 *
		 * @param wheel 第几个时间轮
		 * @param index 时间轮索引
		 *
		 * @return 索引是否是起始位置
		 *
		 */
		bool Cascade(std::size_t wheel, std::size_t index);

	protected:
		bool _status{ true };

		std::mutex _lock;

		std::thread _thread;

		std::atomic<std::time_t> _tick{ 0 };
		std::atomic<std::time_t> _lastTime{ 0 };

		TimerSpokes _nearList[TIMER_NEAR_SIZE]{ };
		TimerSpokes _wheelList[TIMER_WHEEL_NUM][TIMER_WHEEL_SIZE]{ };
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__TIMER__H__
