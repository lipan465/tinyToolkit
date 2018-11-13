#ifndef __TINY_TOOLKIT__ID__UNIQUE__H__
#define __TINY_TOOLKIT__ID__UNIQUE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 唯一全局id
 *
 */


#include "../utilities/time.h"
#include "../utilities/singleton.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API UniqueID
	{
		union ONLY_ID
		{
			struct
			{
				uint32_t count;
				uint32_t times;
			}key;

			uint64_t value;
		};

	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		UniqueID()
		{
			_id.key.count = 0;
			_id.key.times = static_cast<uint32_t>(tinyToolkit::Time::Seconds());
		}

		/**
		 *
		 * 获取全局唯一id
		 *
		 * @return id
		 *
		 */
		static uint64_t Get()
		{
			return tinyToolkit::Singleton<UniqueID>::Instance().Create();
		}

	protected:
		/**
		 *
		 * 创建全局唯一id
		 *
		 * @return id
		 *
		 */
		uint64_t Create()
		{
			auto timeStamp = static_cast<uint32_t>(tinyToolkit::Time::Seconds());

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

	protected:
		ONLY_ID _id{ };
	};
}


#endif // __TINY_TOOLKIT__ID__UNIQUE__H__
