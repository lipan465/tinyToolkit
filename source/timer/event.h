#ifndef __TINY_TOOLKIT__TIMER__EVENT__H__
#define __TINY_TOOLKIT__TIMER__EVENT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 事件
 *
 */


#include "task.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <ctime>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <ctime>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <ctime>
#
#endif


namespace tinyToolkit
{
	namespace timer
	{
		class TINY_TOOLKIT_API Event
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param owner 归属
			 * @param task 任务
			 * @param count 次数
			 * @param expire 到期时间
			 * @param interval 间隔(毫秒)
			 *
			 */
			Event(bool owner, ITask * task, int64_t count, std::time_t expire, std::time_t interval);

			/**
			 *
			 * 析构函数
			 *
			 */
			~Event();

			/**
			 *
			 * 杀死
			 *
			 */
			void Kill();

			/**
			 *
			 * 触发
			 *
			 */
			void Trigger();

			/**
			 *
			 * 暂停
			 *
			 * @param tick 时间戳
			 *
			 */
			void Pause(std::time_t tick);

			/**
			 *
			 * 恢复
			 *
			 * @param tick 时间戳
			 *
			 */
			void Resume(std::time_t tick);

			/**
			 *
			 * 修正过期时间
			 *
			 * @param tick 时间戳
			 *
			 */
			void RevisedExpire(std::time_t tick);

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
			 * 是否暂停
			 *
			 * @return 是否暂停
			 *
			 */
			bool IsPause();

			/**
			 *
			 * 任务
			 *
			 * @return 任务
			 *
			 */
			ITask * Task();

			/**
			 *
			 * 过期时间
			 *
			 * @return 过期时间
			 *
			 */
			std::time_t Expire();

		private:
			bool _isValid{ true };
			bool _isPause{ false };
			bool _isOwner{ false };

			ITask * _task{ nullptr };

			int64_t _count{ 0 };

			std::time_t _expire{ 0 };
			std::time_t _interval{ 0 };
			std::time_t _pauseTick{ 0 };
		};
	}
}


#endif // __TINY_TOOLKIT__TIMER__EVENT__H__
