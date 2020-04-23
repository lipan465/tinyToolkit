/**
 *
 *  作者: hm
 *
 *  说明: 任务
 *
 */


#include "task.h"


namespace tinyToolkit
{
	namespace timer
	{
		/**
		 *
		 * 杀死事件
		 *
		 */
		void ITask::OnKill()
		{

		}

		/**
		 *
		 * 暂停事件
		 *
		 */
		void ITask::OnPause()
		{

		}

		/**
		 *
		 * 恢复事件
		 *
		 */
		void ITask::OnResume()
		{

		}

		/**
		 *
		 * 完成事件
		 *
		 */
		void ITask::OnFinish()
		{

		}

		/**
		 *
		 * 触发事件
		 *
		 */
		void ITask::OnTrigger()
		{

		}

		/**
		 *
		 * 任务次数
		 *
		 * @return 任务次数
		 *
		 */
		int64_t ITask::TotalCount() const
		{
			return _totalCount;
		}

		/**
		 *
		 * 触发次数
		 *
		 * @return 触发次数
		 *
		 */
		int64_t ITask::TriggerCount() const
		{
			return _triggerCount;
		}
	}
}
