#ifndef __TINY_TOOLKIT__TIMER__TASK__H__
#define __TINY_TOOLKIT__TIMER__TASK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 任务
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <cstdint>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <cstdint>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstdint>
#
#endif


namespace tinyToolkit
{
	namespace timer
	{
		class TINY_TOOLKIT_API ITask
		{
			friend class Event;

		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~ITask() = default;

			/**
			 *
			 * 杀死事件
			 *
			 */
			virtual void OnKill();

			/**
			 *
			 * 暂停事件
			 *
			 */
			virtual void OnPause();

			/**
			 *
			 * 恢复事件
			 *
			 */
			virtual void OnResume();

			/**
			 *
			 * 完成事件
			 *
			 */
			virtual void OnFinish();

			/**
			 *
			 * 触发事件
			 *
			 */
			virtual void OnTrigger();

			/**
			 *
			 * 任务次数
			 *
			 * @return 任务次数
			 *
			 */
			int64_t TotalCount() const;

			/**
			 *
			 * 触发次数
			 *
			 * @return 触发次数
			 *
			 */
			int64_t TriggerCount() const;

		private:
			int64_t _totalCount{ 0 };
			int64_t _triggerCount{ 0 };
		};
	}
}


#endif // __TINY_TOOLKIT__TIMER__TASK__H__
