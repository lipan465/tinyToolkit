/**
 *
 *  作者: hm
 *
 *  说明: 分布式唯一标识码
 *
 */


#include "snowflake.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <chrono>
#  include <string>
#  include <stdexcept>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <chrono>
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <chrono>
#  include <stdexcept>
#
#endif


namespace tinyToolkit
{
	namespace id
	{
		/**
		 *
		 * 下个时间戳
		 *
		 * @param lastTimestamp 上次时间截
		 *
		 * @return 当前时间戳
		 *
		 */
		std::time_t NextMilliseconds(std::time_t lastTimestamp)
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


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 构造函数
		 *
		 * @param userID 用户序号
		 * @param deviceID 设备序号
		 *
		 */
		Snowflake::Snowflake(int32_t userID, int32_t deviceID) : _userID(userID),
		                                                         _deviceID(deviceID)
		{
			_userIDBits = 8;
			_deviceIDBits = 8;
			_sequenceBits = 12;

			_userIDMax = ~(-1 * (1 << _userIDBits));
			_deviceIDMax = ~(-1 * (1 << _deviceIDBits));
			_sequenceMask = ~(-1 * (1 << _sequenceBits));

			/// 这里需要用稳定时间, 防止系统时间变化
			_baseTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
			_lastTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

			_userIDShift = _sequenceBits + _deviceIDBits;
			_deviceIDShift = _sequenceBits;
			_timeStampLeftShift = _sequenceBits + _deviceIDBits + _userIDBits;

			if (_userID < 0 || _userID > _userIDMax)
			{
				throw std::runtime_error
				(
					"userID can't be greater than " + std::to_string(_userIDMax) + " or less than 0"
				);
			}

			if (_deviceID < 0 || _deviceID > _deviceIDMax)
			{
				throw std::runtime_error
				(
					"deviceID can't be greater than " + std::to_string(_deviceIDMax) + " or less than 0"
				);
			}
		}

		/**
		 *
		 * 生成
		 *
		 * @return 唯一标识码
		 *
		 */
		uint64_t Snowflake::Generate()
		{
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
					timeStamp = NextMilliseconds(_lastTimeStamp);
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
				(_userID << _userIDShift) |
				(_deviceID << _deviceIDShift) |
				_sequence
			);
		}
	}
}
