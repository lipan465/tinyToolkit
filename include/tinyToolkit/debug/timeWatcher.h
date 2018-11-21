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
		 * 重置
		 *
		 */
		void Reset()
		{
			_max = 0;
			_min = 0;
			_all = 0;
			_count = 0;
			_elapsed = 0;
		}

		/**
		 *
		 * 开始观察
		 *
		 */
		void Start()
		{
			_last = Time::TimePoint();
		}

		/**
		 *
		 * 停止观察
		 *
		 */
		void Stop()
		{
			++_count;

			_elapsed = Time::Microseconds(Time::TimePoint() - _last);

			_all += _elapsed;

			if (_max == 0 || _max < _elapsed)
			{
				_max = _elapsed;
			}

			if (_min == 0 || _min > _elapsed)
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
		std::time_t Max() const
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
		std::time_t Min() const
		{
			return _min;
		}

		/**
		 *
		 * 总观察时间
		 *
		 * @return 总观察时间
		 *
		 */
		std::time_t All() const
		{
			return _all;
		}

		/**
		 *
		 * 观察间隔
		 *
		 * @return 观察间隔
		 *
		 */
		std::time_t Elapsed() const
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
		std::time_t _max{ 0 };
		std::time_t _min{ 0 };
		std::time_t _all{ 0 };
		std::time_t _elapsed{ 0 };

		std::size_t _count{ 0 };

		ClockTimePoint _last{ Time::TimePoint() };
	};
}


#endif // __TINY_TOOLKIT__DEBUG__TIME_WATCHER__H__
