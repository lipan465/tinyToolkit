/**
 *
 *  作者: hm
 *
 *  说明: 事件
 *
 */


#include "event.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <cstdlib>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <cstdlib>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstdlib>
#
#endif


namespace tinyToolkit
{
	namespace timer
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param task 事件
		 * @param count 次数
		 * @param expire 到期时间
		 * @param interval 间隔(毫秒)
		 *
		 */
		Event::Event(std::shared_ptr<ITask> task, int64_t count, std::time_t expire, std::time_t interval) : _count(count),
		                                                                                                     _expire(expire),
		                                                                                                     _interval(interval),
		                                                                                                     _task(std::move(task))
		{
			if (_task)
			{
				_task->_totalCount = count;
			}
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		Event::~Event()
		{
			Kill();
		}

		/**
		 *
		 * 杀死
		 *
		 */
		void Event::Kill()
		{
			if (!_isValid)
			{
				return;
			}

			_isValid = false;

			if (_task)
			{
				_task->OnKill();
			}
		}

		/**
		 *
		 * 触发
		 *
		 */
		void Event::Trigger()
		{
			if (!_isValid)
			{
				return;
			}

			_expire += _interval;

			if (_count != 0)
			{
				if (_task)
				{
					++_task->_triggerCount;

					_task->OnTrigger();
				}
			}

			if (_count > 0)
			{
				--_count;
			}

			if (_count == 0)
			{
				_isValid = false;

				if (_task)
				{
					_task->OnFinish();
				}
			}
		}

		/**
		 *
		 * 暂停
		 *
		 * @param tick 时间戳
		 *
		 */
		void Event::Pause(std::time_t tick)
		{
			if (!_isValid || _isPause)
			{
				return;
			}

			_isPause = true;

			_pauseTick = tick;

			if (_task)
			{
				_task->OnPause();
			}
		}

		/**
		 *
		 * 恢复
		 *
		 * @param tick 时间戳
		 *
		 */
		void Event::Resume(std::time_t tick)
		{
			if (!_isValid || !_isPause)
			{
				return;
			}

			_expire = tick + _expire - _pauseTick;

			_isPause = false;

			if (_task)
			{
				_task->OnResume();
			}
		}

		/**
		 *
		 * 修正过期时间
		 *
		 * @param tick 时间戳
		 *
		 */
		void Event::RevisedExpire(std::time_t tick)
		{
			auto interval = _expire - tick;

			if (interval < 0 && std::abs(interval) > _interval)
			{
				_expire += (std::abs(interval) / _interval) * _interval;
			}
		}

		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool Event::IsValid() const
		{
			return _isValid;
		}

		/**
		 *
		 * 是否暂停
		 *
		 * @return 是否暂停
		 *
		 */
		bool Event::IsPause() const
		{
			return _isPause;
		}

		/**
		 *
		 * 过期时间
		 *
		 * @return 过期时间
		 *
		 */
		std::time_t Event::Expire() const
		{
			return _expire;
		}

		/**
		 *
		 * 任务
		 *
		 * @return 任务
		 *
		 */
		std::shared_ptr<ITask> Event::Task()
		{
			return _task;
		}
	}
}
