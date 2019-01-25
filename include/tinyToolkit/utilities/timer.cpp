/**
 *
 *  作者: hm
 *
 *  说明: 定时器
 *
 */


#include "time.h"
#include "timer.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	Timer::Timer()
	{
		_tick.store(0);
		_lastTime.store(Time::Milliseconds());

		_thread = std::thread(&Timer::ThreadProcess, this);
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	Timer::~Timer()
	{
		Close();

		if (_thread.joinable())
		{
			_thread.join();
		}

		for (auto & spokes : _nearList)
		{
			ClearSpokes(spokes);
		}

		for (auto & wheel : _wheelList)
		{
			for (auto & spokes : wheel)
			{
				ClearSpokes(spokes);
			}
		}
	}

	/**
	 *
	 * 关闭定时器
	 *
	 */
	void Timer::Close()
	{
		_status = false;
	}

	/**
	 *
	 * 定时器是否启动
	 *
	 * @return 状态
	 *
	 */
	bool Timer::Status()
	{
		return _status;
	}

	/**
	 *
	 * 线程函数(更新定时器tick)
	 *
	 */
	void Timer::ThreadProcess()
	{
		while (Status())
		{
			Update();

			TINY_TOOLKIT_SLEEP_MS(10)
		}
	}

	/**
	 *
	 * 更新定时器tick
	 *
	 */
	void Timer::Update()
	{
		std::time_t now = Time::Milliseconds();

		if (now < _lastTime.load())
		{
			throw std::runtime_error("Time go backwards");
		}
		else
		{
			std::time_t ticks = now - _lastTime.load();

			if (ticks > 0)
			{
				_lastTime.store(now);

				for (int i = 0; i < ticks; i++)
				{
					Executes();
				}
			}
		}
	}

	/**
	 *
	 * 执行定时器
	 *
	 */
	void Timer::Executes()
	{
		auto index = _tick.load() & TIMER_NEAR_MASK;

		if (index == 0)
		{
			if (Cascade(0, (std::size_t)(_tick.load() >> (TIMER_NEAR_BITS + 0 * TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK) &&
				Cascade(1, (std::size_t)(_tick.load() >> (TIMER_NEAR_BITS + 1 * TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK) &&
				Cascade(2, (std::size_t)(_tick.load() >> (TIMER_NEAR_BITS + 2 * TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK))
			{
				Cascade(3, (std::size_t)(_tick.load() >> (TIMER_NEAR_BITS + 3 * TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK);
			}
		}

		++_tick;

		TimerSpokes spokesList;

		_nearList[index].swap(spokesList);

		for (TimerNode * node : spokesList)
		{
			if (node)
			{
				if (node->callback)
				{
					node->callback();
				}

				if (node->circle)
				{
					node->expire = node->interval + _tick.load();

					AddNode(node);

					continue;
				}

				delete node;

				node = nullptr;
			}
		}
	}

	/**
	 *
	 * 添加定时器
	 *
	 * @param node 待添加定时器
	 *
	 */
	void Timer::AddNode(TimerNode * node)
	{
		std::time_t expire = node->expire;

		auto offset = (uint64_t)(expire - _tick.load());

		if (offset < TIMER_NEAR_SIZE) /// [0, 0x100)
		{
			auto index = expire & TIMER_NEAR_MASK;

			_nearList[index].push_back(node);
		}
		else if (offset < (1 << (TIMER_NEAR_BITS + TIMER_WHEEL_BITS)))  /// [0x100, 0x4000)
		{
			auto index = (expire >> TIMER_NEAR_BITS) & TIMER_WHEEL_MASK;

			_wheelList[0][index].push_back(node);
		}
		else if (offset < (1 << (TIMER_NEAR_BITS + 2 * TIMER_WHEEL_BITS))) /// [0x4000, 0x100000)
		{
			auto index = (expire >> (TIMER_NEAR_BITS + TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK;

			_wheelList[1][index].push_back(node);
		}
		else if (offset < (1 << (TIMER_NEAR_BITS + 3 * TIMER_WHEEL_BITS))) /// [0x100000, 0x4000000)
		{
			auto index = (expire >> (TIMER_NEAR_BITS + 2 * TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK;

			_wheelList[2][index].push_back(node);
		}
		else if ((int64_t)offset < 0)
		{
			auto index = _tick.load() & TIMER_NEAR_MASK;

			_nearList[index].push_back(node);
		}
		else if (offset <= 0xffffffffUL)
		{
			auto index = (expire >> (TIMER_NEAR_BITS + 3 * TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK;

			_wheelList[3][index].push_back(node);
		}
		else
		{
			throw std::runtime_error("The timing interval exceeds the maximum limit");
		}
	}

	/**
	 *
	 * 清理时间轮
	 *
	 * @param spokesList 待清理时间轮
	 *
	 */
	void Timer::ClearSpokes(TimerSpokes & spokesList)
	{
		for (TimerNode * node : spokesList)
		{
			if (node)
			{
				delete node;

				node = nullptr;
			}
		}

		spokesList.clear();
	}

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
	bool Timer::Cascade(std::size_t wheel, std::size_t index)
	{
		std::lock_guard<std::mutex> lock(_lock);

		TimerSpokes spokesList;

		_wheelList[wheel][index].swap(spokesList);

		for (TimerNode * node : spokesList)
		{
			if (node)
			{
				AddNode(node);
			}
		}

		return index == 0;
	}
}
