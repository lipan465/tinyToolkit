#ifndef __TINY_TOOLKIT__ID__SNOWFLAKE__H__
#define __TINY_TOOLKIT__ID__SNOWFLAKE__H__


/**
 *
 *  作者: hm
 *
 *  说明: twitter分布式唯一全局id
 *
 */


#include "../utilities/singleton.h"


namespace tinyToolkit
{
	/**
	 *
	 * 无效位                    时间戳                       数据标识      机器ID         序列号
	 *   0 - 000 0000 0000 0000 0000 0000 0000 0000 0000 - 0000 0000 - 0000 0000 - 0000 0000 0000
	 *
	 */
	class TINY_TOOLKIT_API Snowflake
	{
		friend Singleton<Snowflake>;

	public:
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
		static uint64_t Get(int32_t dataCenterID = 0, int32_t workerID = 0);

	protected:
		/**
		 *
		 * 构造函数
		 *
		 */
		Snowflake();

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
		uint64_t Create(int32_t dataCenterID, int32_t workerID);

		/**
		 *
		 * 获取下一个id
		 *
		 * @return id
		 *
		 */
		uint64_t NextID();

		/**
		 *
		 * 阻塞到下一个毫秒, 直到获得新的时间戳
		 *
		 * @param lastTimestamp 上次生成ID的时间截
		 *
		 * @return 当前时间戳
		 *
		 */
		std::time_t TilNextMillis(std::time_t lastTimestamp);

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

		std::time_t _baseTimeStamp{ 0 };
		std::time_t _lastTimeStamp{ 0 };
	};
}


#endif // __TINY_TOOLKIT__ID__SNOWFLAKE__H__
