/**
 *
 *  作者: hm
 *
 *  说明: 定时器节点
 *
 */


#include "node.h"


namespace tinyToolkit
{
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
	TimerNode::TimerNode(ITimerEvent * event, int64_t count, std::time_t expire, std::time_t interval) : _count(count),
																										 _expire(expire),
																										 _interval(interval),
																										 _event(event)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	TimerNode::~TimerNode()
	{
		Kill();
	}

	/**
	 *
	 * 杀死
	 *
	 */
	void TimerNode::Kill()
	{
		if (_isValid)
		{
			_isValid = false;

			if (_event)
			{
				_event->OnFinish(true);
			}
		}
	}

	/**
	 *
	 * 触发
	 *
	 */
	void TimerNode::Trigger()
	{
		if (_isValid)
		{
			_expire += _interval;

			if (_count != 0)
			{
				if (_event)
				{
					_event->OnTrigger();
				}
			}

			if (_count > 0)
			{
				--_count;
			}
		}

		if (_isValid && _count == 0)
		{
			_isValid = false;

			if (_event)
			{
				_event->OnFinish(false);
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
	void TimerNode::Pause(std::time_t tick)
	{
		if (_isValid && !_isPause)
		{
			_isPause = true;

			_pauseTick = tick;

			if (_event)
			{
				_event->OnPause();
			}
		}
	}

	/**
	 *
	 * 恢复
	 *
	 * @param tick 时间戳
	 *
	 */
	void TimerNode::Resume(std::time_t tick)
	{
		if (_isValid && _isPause)
		{
			_expire = tick + _expire - _pauseTick;

			_isPause = false;

			if (_event)
			{
				_event->OnResume();
			}
		}
	}

	/**
	 *
	 * 修正过期时间
	 *
	 * @param tick 时间戳
	 *
	 */
	void TimerNode::RevisedExpire(std::time_t tick)
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
	bool TimerNode::IsValid()
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
	bool TimerNode::IsPause()
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
	std::time_t TimerNode::Expire()
	{
		return _expire;
	}

	/**
	 *
	 * 事件
	 *
	 * @return 事件
	 *
	 */
	ITimerEvent * TimerNode::Event()
	{
		return _event;
	}
}

