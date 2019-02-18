#ifndef __TINY_TOOLKIT__TIMER__MANAGER__H__
#define __TINY_TOOLKIT__TIMER__MANAGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 定时器管理器
 *
 */


#include "node.h"


namespace tinyToolkit
{
	#define TIMER_WHEEL_NUM  4
	#define TIMER_NEAR_BITS  8
	#define TIMER_WHEEL_BITS 6

	#define TIMER_NEAR_SIZE   (1 << TIMER_NEAR_BITS)
	#define TIMER_WHEEL_SIZE  (1 << TIMER_WHEEL_BITS)

	#define TIMER_NEAR_MASK   (TIMER_NEAR_SIZE  - 1)
	#define TIMER_WHEEL_MASK  (TIMER_WHEEL_SIZE - 1)

	class TINY_TOOLKIT_API TimerManager
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		TimerManager();

		/**
		 *
		 * 析构函数
		 *
		 */
		~TimerManager();

		/**
		 *
		 * 关闭
		 *
		 */
		void Close();

		/**
		 *
		 * 关闭事件
		 *
		 * @param event 事件
		 *
		 */
		void Kill(ITimerEvent * event);

		/**
		 *
		 * 暂停事件
		 *
		 * @param event 事件
		 *
		 */
		void Pause(ITimerEvent * event);

		/**
		 *
		 * 恢复事件
		 *
		 * @param event 事件
		 *
		 */
		void Resume(ITimerEvent * event);

		/**
		 *
		 * 启动事件
		 *
		 * @param event 事件
		 * @param count 次数
		 * @param interval 间隔(毫秒)
		 *
		 */
		void Start(ITimerEvent * event, int64_t count, std::time_t interval);

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
		 * 转动时间轮
		 *
		 * @param wheel 第几个时间轮
		 * @param index 时间轮索引
		 *
		 * @return 索引是否是起始位置
		 *
		 */
		bool Cascade(std::size_t wheel, std::size_t index);

		/**
		 *
		 * 清理时间轮
		 *
		 * @param spokesList 待清理时间轮
		 *
		 */
		void ClearSpokes(std::vector<TimerNode *> & spokesList);

	protected:
		bool _status{ true };

		std::mutex _lock{ };

		std::thread _thread{ };

		std::atomic<std::time_t> _tickTime{ 0 };
		std::atomic<std::time_t> _lastTime{ 0 };

		std::vector<TimerNode *> _nearList[TIMER_NEAR_SIZE]{ };
		std::vector<TimerNode *> _wheelList[TIMER_WHEEL_NUM][TIMER_WHEEL_SIZE]{ };

		std::unordered_map<ITimerEvent *, TimerNode *> _manager{ };
		std::unordered_map<ITimerEvent *, TimerNode *> _pauseList{ };
		std::unordered_map<ITimerEvent *, TimerNode *> _normalList{ };
	};
}


#endif // __TINY_TOOLKIT__TIMER__MANAGER__H__
