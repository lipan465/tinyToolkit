#ifndef __TINY_TOOLKIT__TIMER__TIMER__H__
#define __TINY_TOOLKIT__TIMER__TIMER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 定时器
 *
 */


#include "event.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <mutex>
#  include <atomic>
#  include <functional>
#  include <unordered_map>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <thread>
#  include <vector>
#  include <unordered_map>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <mutex>
#  include <atomic>
#  include <thread>
#  include <vector>
#  include <functional>
#  include <unordered_map>
#
#endif


namespace tinyToolkit
{
	namespace timer
	{
		class TINY_TOOLKIT_API Timer
		{
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
			 * 单例对象
			 *
			 * @return 单例对象
			 *
			 */
			static Timer & Instance();

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
			 * @param task 任务
			 *
			 * @return 是否关闭成功
			 *
			 */
			bool Kill(const std::shared_ptr<ITask> & task);

			/**
			 *
			 * 是否存在
			 *
			 * @param task 任务
			 *
			 * @return 是否存在
			 *
			 */
			bool Exist(const std::shared_ptr<ITask> & task);

			/**
			 *
			 * 暂停事件
			 *
			 * @param task 任务
			 *
			 * @return 是否暂停成功
			 *
			 */
			bool Pause(const std::shared_ptr<ITask> & task);

			/**
			 *
			 * 恢复事件
			 *
			 * @param task 任务
			 *
			 * @return 是否恢复成功
			 *
			 */
			bool Resume(const std::shared_ptr<ITask> & task);

			/**
			 *
			 * 添加任务
			 *
			 * @param task 任务
			 * @param count 次数
			 * @param interval 间隔(毫秒)
			 *
			 * @return 是否启动成功
			 *
			 */
			bool AddTask(const std::shared_ptr<ITask> & task, int64_t count, std::time_t interval);

			/**
			 *
			 * 添加任务
			 *
			 * @param function 函数
			 * @param count 次数
			 * @param interval 间隔(毫秒)
			 *
			 * @return 是否启动成功
			 *
			 */
			bool AddTask(const std::function<void()> & function, int64_t count, std::time_t interval);

			/**
			 *
			 * 工作任务个数
			 *
			 * @return 工作任务个数
			 *
			 */
			std::size_t WorkTaskCount() const;

			/**
			 *
			 * 暂停任务个数
			 *
			 * @return 暂停任务个数
			 *
			 */
			std::size_t PauseTaskCount() const;

			/**
			 *
			 * 定时任务个数
			 *
			 * @return 定时任务个数
			 *
			 */
			std::size_t TotalTaskCount() const;

		protected:
			/**
			 *
			 * 更新定时器
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
			 * 添加事件
			 *
			 * @param event 事件
			 *
			 */
			void AddEvent(Event * event);

			/**
			 *
			 * 删除事件
			 *
			 * @param event 事件
			 *
			 */
			void DelEvent(Event * event);

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

		private:
			bool _status{ true };

			std::mutex _taskMutex{ };
			std::mutex _eventMutex{ };

			std::thread _thread{ };

			std::atomic<std::time_t> _tickTime{ 0 };
			std::atomic<std::time_t> _lastTime{ 0 };

			std::vector<Event *> _nearList[TINY_TOOLKIT_TIMER_NEAR_SIZE]{ };
			std::vector<Event *> _wheelList[TINY_TOOLKIT_TIMER_WHEEL_NUM][TINY_TOOLKIT_TIMER_WHEEL_SIZE]{ };

			std::unordered_map<std::shared_ptr<ITask>, Event *> _taskList{ };
			std::unordered_map<std::shared_ptr<ITask>, Event *> _workList{ };
			std::unordered_map<std::shared_ptr<ITask>, Event *> _pauseList{ };
		};
	}
}


#endif // __TINY_TOOLKIT__TIMER__TIMER__H__
