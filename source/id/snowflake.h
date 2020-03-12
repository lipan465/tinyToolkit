#ifndef __TINY_TOOLKIT__ID__SNOWFLAKE__H__
#define __TINY_TOOLKIT__ID__SNOWFLAKE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 分布式唯一标识码
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <ctime>
#  include <cstdint>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <ctime>
#  include <cstdint>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <ctime>
#  include <cstdint>
#
#endif


namespace tinyToolkit
{
	namespace id
	{
		/**
		 *
		 * 无效位                    时间戳                       用户序号      设备序号         序列号
		 *   0 - 000 0000 0000 0000 0000 0000 0000 0000 0000 - 0000 0000 - 0000 0000 - 0000 0000 0000
		 *
		 */
		class TINY_TOOLKIT_API Snowflake
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param userID 用户序号
			 * @param deviceID 设备序号
			 *
			 */
			Snowflake(int32_t userID, int32_t deviceID);

			/**
			 *
			 * 生成
			 *
			 * @return 唯一标识码
			 *
			 */
			uint64_t Generate();

		private:
			int32_t _userID{ 0 };
			int32_t _deviceID{ 0 };
			int32_t _sequence{ 0 };

			int32_t _userIDBits{ 0 };
			int32_t _deviceIDBits{ 0 };
			int32_t _sequenceBits{ 0 };

			int32_t _userIDMax{ 0 };
			int32_t _deviceIDMax{ 0 };

			int32_t _userIDShift{ 0 };
			int32_t _deviceIDShift{ 0 };
			int32_t _timeStampLeftShift{ 0 };

			int32_t _sequenceMask{ 0 };

			std::time_t _baseTimeStamp{ 0 };
			std::time_t _lastTimeStamp{ 0 };
		};
	}
}


#endif // __TINY_TOOLKIT__ID__SNOWFLAKE__H__
