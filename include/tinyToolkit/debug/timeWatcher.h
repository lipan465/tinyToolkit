#ifndef __TINY_TOOLKIT__DEBUG__TIME_WATCHER__H__
#define __TINY_TOOLKIT__DEBUG__TIME_WATCHER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 时间观察
 *
 */


#include "../utilities/time.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TimeWatcher
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		~TimeWatcher()
		{
			Reset();
		}

		/**
		 *
		 * 重置
		 *
		 */
		void Reset()
		{
			_count = 0;

			_max = 0.0;
			_min = 0.0;
			_avg = 0.0;
			_all = 0.0;
			_rate = 0.0;
			_elapsed = 0.0;
		}

		/**
		 *
		 * 开始观察
		 *
		 */
		void Start()
		{
			_head = Time::TimePoint();
		}

		/**
		 *
		 * 停止观察
		 *
		 */
		void Stop()
		{
			_all = Time::Microseconds(Time::TimePoint() - _head);

			_avg = _all / _count;
			_rate = _count / _all;
		}

		/**
		 *
		 * 开始间隔观察
		 *
		 */
		void ElapsedStart()
		{
			_last = Time::TimePoint();
		}

		/**
		 *
		 * 停止间隔观察
		 *
		 */
		void ElapsedStop()
		{
			++_count;

			_elapsed = Time::Microseconds(Time::TimePoint() - _last);

			if (_max == 0.0 || _max < _elapsed)
			{
				_max = _elapsed;
			}

			if (_min == 0.0 || _min > _elapsed)
			{
				_min = _elapsed;
			}
		}

		/**
		 *
		 * 最大观察时间
		 *
		 * @return 最大观察时间
		 *
		 */
		std::double_t Max() const
		{
			return _max;
		}

		/**
		 *
		 * 最小观察时间
		 *
		 * @return 最小观察时间
		 *
		 */
		std::double_t Min() const
		{
			return _min;
		}

		/**
		 *
		 * 平均观察时间
		 *
		 * @return 平均观察时间
		 *
		 */
		std::double_t Avg() const
		{
			return _avg;
		}

		/**
		 *
		 * 总观察时间
		 *
		 * @return 总观察时间
		 *
		 */
		std::double_t All() const
		{
			return _all;
		}

		/**
		 *
		 * 观察速率
		 *
		 * @return 观察速率
		 *
		 */
		std::double_t Rate() const
		{
			return _rate;
		}

		/**
		 *
		 * 观察间隔
		 *
		 * @return 观察间隔
		 *
		 */
		std::double_t Elapsed() const
		{
			return _elapsed;
		}

		/**
		 *
		 * 总观察次数
		 *
		 * @return 总观察次数
		 *
		 */
		std::size_t Count() const
		{
			return _count;
		}

	protected:
		std::size_t _count{ 0 };

		std::double_t _max{ 0.0 };
		std::double_t _min{ 0.0 };
		std::double_t _avg{ 0.0 };
		std::double_t _all{ 0.0 };
		std::double_t _rate{ 0.0 };
		std::double_t _elapsed{ 0.0 };

		ClockTimePoint _head{ Time::TimePoint() };
		ClockTimePoint _last{ Time::TimePoint() };
	};
}


#endif // __TINY_TOOLKIT__DEBUG__TIME_WATCHER__H__
