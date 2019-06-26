/**
 *
 *  作者: hm
 *
 *  说明: twitter分布式唯一全局id
 *
 */


#include "snowflake.h"

#include "../utilities/time.h"
#include "../utilities/string.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	Snowflake::Snowflake()
	{
		_sequenceBits = 12;
		_workerIDBits = 5;
		_dataCenterIDBits = 5;

		_workerIDMax = ~(-1 * (1 << _workerIDBits));
		_sequenceMask = ~(-1 * (1 << _sequenceBits));
		_dataCenterIDMax = ~(-1 * (1 << _dataCenterIDBits));

		/// 这里需要用稳定时间, 防止系统时间变化
		_baseTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
		_lastTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

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
	uint64_t Snowflake::Get(int32_t dataCenterID, int32_t workerID)
	{
		return Singleton<Snowflake>::Instance().Create(dataCenterID, workerID);
	}

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
	uint64_t Snowflake::Create(int32_t dataCenterID, int32_t workerID)
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
	uint64_t Snowflake::NextID()
	{
		/// 这里需要用稳定时间, 防止系统时间变化
		auto timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

		if (timeStamp < _lastTimeStamp)
		{
			throw std::runtime_error("Clock moved backwards, Refusing to generate id");
		}

		if (_lastTimeStamp == timeStamp)
		{
			_sequence = (_sequence + 1) & _sequenceMask;

			if (_sequence == 0)  /// 毫秒内序列溢出
			{
				timeStamp = TilNextMillis(_lastTimeStamp);
			}
		}
		else
		{
			_sequence = 0;
		}

		_lastTimeStamp = timeStamp;

		return static_cast<uint64_t>
		(
			((_lastTimeStamp - _baseTimeStamp) << _timeStampLeftShift) |
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
	std::time_t Snowflake::TilNextMillis(std::time_t lastTimestamp)
	{
		/// 这里需要用稳定时间, 防止系统时间变化
		auto timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

		while (timeStamp <= lastTimestamp)
		{
			/// 这里需要用稳定时间, 防止系统时间变化
			timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
		}

		return timeStamp;
	}

}
