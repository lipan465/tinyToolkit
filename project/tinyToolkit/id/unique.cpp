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
		_id.key.count = 0;
		_id.key.times = static_cast<uint32_t>(Time::Seconds());
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
		auto timeStamp = static_cast<uint32_t>(Time::Seconds());

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
