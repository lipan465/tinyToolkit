/**
 *
 *  作者: hm
 *
 *  说明: 定时器管理器
 *
 */


#include "manager.h"

#include "../debug/trace.h"
#include "../utilities/time.h"
#include "../pool/application.h"
#include "../utilities/operator.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API InstanceTimerEvent;

	ApplicationPool<InstanceTimerEvent> sApplicationPool;

	class TINY_TOOLKIT_API InstanceTimerEvent : public ITimerEvent
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param function 函数
		 *
		 */
		explicit InstanceTimerEvent(std::function<void()> function) : _function(std::move(function))
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~InstanceTimerEvent() override = default;

		/**
		 *
		 * 暂停事件调用回调函数
		 *
		 */
		void OnPause() override
		{

		}

		/**
		 *
		 * 恢复事件调用回调函数
		 *
		 */
		void OnResume() override
		{

		}

		/**
		 *
		 * 触发事件调用回调函数
		 *
		 */
		void OnTrigger() override
		{
			if (_function)
			{
				_function();
			}
		}

		/**
		 *
		 * 结束事件调用回调函数
		 *
		 * @param forced 强制性
		 *
		 */
		void OnFinish(bool forced) override
		{
			(void)forced;

			sApplicationPool.Recover(this);
		}

	private:
		std::function<void()> _function{ };
	};


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 构造函数
	 *
	 */
	TimerManager::TimerManager()
	{
		_tickTime.store(0);
		_lastTime.store(Time::Milliseconds());

		_thread = std::thread(&TimerManager::ThreadProcess, this);
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	TimerManager::~TimerManager()
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
	 * 关闭
	 *
	 */
	void TimerManager::Close()
	{
		_status = false;
	}

	/**
	 *
	 * 关闭事件
	 *
	 * @param event 事件
	 *
	 * @return 是否关闭成功
	 *
	 */
	bool TimerManager::Kill(ITimerEvent * event)
	{
		TINY_TOOLKIT_ASSERT(event, "Timer event null");

		auto find = _manager.find(event);

		if (find == _manager.end())
		{
			return false;
		}

		auto node = find->second;

		node->Kill();

		return true;
	}

	/**
	 *
	 * 暂停事件
	 *
	 * @param event 事件
	 *
	 * @return 是否暂停成功
	 *
	 */
	bool TimerManager::Pause(ITimerEvent * event)
	{
		TINY_TOOLKIT_ASSERT(event, "Timer event null");

		auto find = _normalList.find(event);

		if (find == _normalList.end())
		{
			return false;
		}

		auto node = find->second;

		node->Pause(_tickTime);

		_normalList.erase(find);

		_pauseList.insert(std::make_pair(event, node));

		return true;
	}

	/**
	 *
	 * 恢复事件
	 *
	 * @param event 事件
	 *
	 * @return 是否恢复成功
	 *
	 */
	bool TimerManager::Resume(ITimerEvent * event)
	{
		TINY_TOOLKIT_ASSERT(event, "Timer event null");

		auto find = _pauseList.find(event);

		if (find == _pauseList.end())
		{
			return false;
		}

		auto node = find->second;

		node->Resume(_tickTime);

		AddNode(node);

		_pauseList.erase(find);

		_normalList.insert(std::make_pair(event, node));

		return true;
	}

	/**
	 *
	 * 启动事件
	 *
	 * @param event 事件
	 * @param count 次数
	 * @param interval 间隔(毫秒)
	 *
	 * @return 是否启动成功
	 *
	 */
	bool TimerManager::Start(ITimerEvent * event, int64_t count, std::time_t interval)
	{
		TINY_TOOLKIT_ASSERT(event, "Timer event null");

		if (_manager.find(event) == _manager.end())
		{
			auto node = new TimerNode(event, count, interval + _tickTime, interval);

			AddNode(node);

			_manager.insert(std::make_pair(event, node));

			_normalList.insert(std::make_pair(event, node));

			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 *
	 * 启动事件
	 *
	 * @param function 函数
	 * @param count 次数
	 * @param interval 间隔(毫秒)
	 *
	 * @return 是否启动成功
	 *
	 */
	bool TimerManager::Start(std::function<void()> function, int64_t count, std::time_t interval)
	{
		auto event = sApplicationPool.Create(std::move(function));

		if (_manager.find(event) == _manager.end())
		{
			auto node = new TimerNode(event, count, interval + _tickTime, interval);

			AddNode(node);

			_manager.insert(std::make_pair(event, node));

			_normalList.insert(std::make_pair(event, node));

			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 *
	 * 线程函数(更新定时器tick)
	 *
	 */
	void TimerManager::ThreadProcess()
	{
		while (_status)
		{
			Update();

			TINY_TOOLKIT_SLEEP_MS(5)
		}
	}

	/**
	 *
	 * 更新定时器tick
	 *
	 */
	void TimerManager::Update()
	{
		std::time_t now = Time::Milliseconds();

		if (now < _lastTime.load())
		{
			throw std::runtime_error("Time go backwards");
		}
		else
		{
			std::time_t ticks = now - _lastTime;

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
	void TimerManager::Executes()
	{
		auto index = _tickTime & TIMER_NEAR_MASK;

		if (index == 0)
		{
			if (Cascade(0, (std::size_t)(_tickTime >> (TIMER_NEAR_BITS + 0 * TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK) &&
				Cascade(1, (std::size_t)(_tickTime >> (TIMER_NEAR_BITS + 1 * TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK) &&
				Cascade(2, (std::size_t)(_tickTime >> (TIMER_NEAR_BITS + 2 * TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK))
			{
				Cascade(3, (std::size_t)(_tickTime >> (TIMER_NEAR_BITS + 3 * TIMER_WHEEL_BITS)) & TIMER_WHEEL_MASK);
			}
		}

		++_tickTime;

		std::vector<TimerNode *> spokesList;

		_nearList[index].swap(spokesList);

		for (TimerNode * node : spokesList)
		{
			if (node)
			{
				if (node->IsValid())
				{
					if (node->IsPause())
					{

					}
					else
					{
						node->Trigger();

						node->RevisedExpire(_tickTime);

						AddNode(node);
					}
				}
				else
				{
					{
						auto find = _manager.find(node->Event());

						if (find != _manager.end())
						{
							_manager.erase(find);
						}
					}

					{
						auto find = _pauseList.find(node->Event());

						if (find != _pauseList.end())
						{
							_pauseList.erase(find);
						}
					}

					{
						auto find = _normalList.find(node->Event());

						if (find != _normalList.end())
						{
							_normalList.erase(find);
						}
					}

					delete node;

					node = nullptr;
				}
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
	void TimerManager::AddNode(TimerNode * node)
	{
		std::lock_guard<std::mutex> lock(_lock);

		std::time_t expire = node->Expire();

		auto offset = (uint64_t)(expire - _tickTime);

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
			auto index = _tickTime & TIMER_NEAR_MASK;

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
	void TimerManager::ClearSpokes(std::vector<TimerNode *> & spokesList)
	{
		for (TimerNode * node : spokesList)
		{
			if (node)
			{
				delete node;

				node = nullptr;
			}
		}

		Operator::Clear(spokesList);
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
	bool TimerManager::Cascade(std::size_t wheel, std::size_t index)
	{
		std::vector<TimerNode *> spokesList{ };

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
