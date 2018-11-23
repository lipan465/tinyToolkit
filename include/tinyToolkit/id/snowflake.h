#ifndef __TINY_TOOLKIT__ID__SNOWFLAKE__H__
#define __TINY_TOOLKIT__ID__SNOWFLAKE__H__


/**
 *
 *  作者: hm
 *
 *  说明: twitter分布式唯一全局id
 *
 */


#include "../utilities/time.h"
#include "../utilities/string.h"
#include "../utilities/singleton.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Snowflake
	{
		/**
		 *
		 * 无效位                        时间戳                            数据标识  机器ID      序列号
		 * 0 - 0000000000 0000000000 0000000000 0000000000 0000000000 0 - 00000 - 00000 - 000000000000
		 *
		 */
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		Snowflake()
		{
			_sequenceBits = 12;
			_workerIDBits = 5;
			_dataCenterIDBits = 5;

			_workerIDMax = ~(-1 * (1 << _workerIDBits));
			_sequenceMask = ~(-1 * (1 << _sequenceBits));
			_dataCenterIDMax = ~(-1 * (1 << _dataCenterIDBits));

			_workerIDShift = _sequenceBits;
			_dataCenterIDShift = _sequenceBits + _workerIDBits;
			_timeStampLeftShift = _sequenceBits + _workerIDBits + _dataCenterIDBits;
		}

		/**
		 *
		 * 获取全局唯一id
		 *
		 * @param dataCenterID 数据标识
		 * @param workerID 机器ID
		 *
		 * @return 全局唯一id
		 *
		 */
		static uint64_t Get(int32_t dataCenterID = 0, int32_t workerID = 0)
		{
			return Singleton<Snowflake>::Instance().Create(dataCenterID, workerID);
		}

	protected:
		/**
		 *
		 * 创建全局唯一id
		 *
		 * @param dataCenterID 数据标识
		 * @param workerID 机器ID
		 *
		 * @return 全局唯一id
		 *
		 */
		uint64_t Create(int32_t dataCenterID, int32_t workerID)
		{
			if (workerID < 0 || workerID > _workerIDMax)
			{
				throw std::runtime_error(String::Format("workerID can't be greater than {} or less than 0", _workerIDMax));
			}

			if (dataCenterID < 0 || dataCenterID > _dataCenterIDMax)
			{
				throw std::runtime_error(String::Format("dataCenterID can't be greater than {} or less than 0", _dataCenterIDMax));
			}

			_workerID = workerID;
			_dataCenterID = dataCenterID;

			return NextID();
		}

		/**
		 *
		 * 获取下一个id
		 *
		 * @return id
		 *
		 */
		uint64_t NextID()
		{
			auto timeStamp = Time::Milliseconds();

			if (timeStamp < _lastTimeStamp)
			{
				throw std::runtime_error("Clock moved backwards, Refusing to generate id");
			}

			if (_lastTimeStamp == timeStamp)
			{
				_sequence = (_sequence + 1) & _sequenceMask;

				if (_sequence == 0)  // 毫秒内序列溢出
				{
					timeStamp = TilNextMillis(_lastTimeStamp);
				}
			}
			else
			{
				_sequence = 0;
			}

			_lastTimeStamp = timeStamp;

			return static_cast<uint64_t >
			(
				((timeStamp - _baseTimeStamp) << _timeStampLeftShift) |
				(_dataCenterID << _dataCenterIDShift) |
				(_workerID << _workerIDShift) |
				_sequence
			);
		}

		/**
		 *
		 * 阻塞到下一个毫秒, 直到获得新的时间戳
		 *
		 * @param lastTimestamp 上次生成ID的时间截
		 *
		 * @return 当前时间戳
		 *
		 */
		std::time_t TilNextMillis(std::time_t lastTimestamp)
		{
			auto timeStamp = Time::Milliseconds();

			while (timeStamp <= lastTimestamp)
			{
				timeStamp = Time::Milliseconds();
			}

			return timeStamp;
		}

	protected:
		int32_t _sequence{ 0 };
		int32_t _workerID{ 0 };
		int32_t _dataCenterID{ 0 };

		int32_t _sequenceBits{ 0 };
		int32_t _workerIDBits{ 0 };
		int32_t _dataCenterIDBits{ 0 };

		int32_t _workerIDMax{ 0 };
		int32_t _dataCenterIDMax{ 0 };

		int32_t _workerIDShift{ 0 };
		int32_t _dataCenterIDShift{ 0 };
		int32_t _timeStampLeftShift{ 0 };

		int32_t _sequenceMask{ 0 };

		std::time_t _baseTimeStamp{ Time::Milliseconds() };
		std::time_t _lastTimeStamp{ 0 };
	};
}


#endif // __TINY_TOOLKIT__ID__SNOWFLAKE__H__
