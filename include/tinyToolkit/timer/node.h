#ifndef __TINY_TOOLKIT__TIMER__NODE__H__
#define __TINY_TOOLKIT__TIMER__NODE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 定时器节点
 *
 */


#include "event.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TimerNode
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param event 事件
		 * @param count 次数
		 * @param expire 到期时间
		 * @param interval 间隔(毫秒)
		 *
		 */
		TimerNode(ITimerEvent * event, int64_t count, std::time_t expire, std::time_t interval);

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
		 * 过期时间
		 *
		 * @return 过期时间
		 *
		 */
		std::time_t Expire();

		/**
		 *
		 * 事件
		 *
		 * @return 事件
		 *
		 */
		ITimerEvent * Event();

	protected:
		bool _isValid{ true };
		bool _isPause{ false };

		int64_t _count{ 0 };

		std::time_t _expire{ 0 };
		std::time_t _interval{ 0 };
		std::time_t _pauseTick{ 0 };

		ITimerEvent * _event{ nullptr };
	};
}


#endif // __TINY_TOOLKIT__TIMER__NODE__H__
