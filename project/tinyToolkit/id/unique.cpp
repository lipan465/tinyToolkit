/**
 *
 *  作者: hm
 *
 *  说明: 唯一全局id
 *
 */


#include "unique.h"

#include "../utilities/time.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	UniqueID::UniqueID()
	{
		/// 这里需要用稳定时间, 防止系统时间变化
		_id.key.count = 0;
		_id.key.times = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
	}

	/**
	 *
	 * 获取全局唯一id
	 *
	 * @return id
	 *
	 */
	uint64_t UniqueID::Get()
	{
		return Singleton<UniqueID>::Instance().Create();
	}

	/**
	 *
	 * 创建全局唯一id
	 *
	 * @return id
	 *
	 */
	uint64_t UniqueID::Create()
	{
		/// 这里需要用稳定时间, 防止系统时间变化
		auto timeStamp = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());

		if (timeStamp != _id.key.times)
		{
			_id.key.count = 0;
			_id.key.times = timeStamp;
		}
		else
		{
			++_id.key.count;
		}

		return _id.value;
	}
}
