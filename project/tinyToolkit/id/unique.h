#ifndef __TINY_TOOLKIT__ID__UNIQUE__H__
#define __TINY_TOOLKIT__ID__UNIQUE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 唯一全局id
 *
 */


#include "../utilities/singleton.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API UniqueID
	{
		union ONLY_ID
		{
			struct
			{
				uint32_t count{ 0 };
				uint32_t times{ 0 };
			}key;

			uint64_t value{ 0 };
		};

		friend Singleton<UniqueID>;

	public:
		/**
		 *
		 * 获取全局唯一id
		 *
		 * @return id
		 *
		 */
		static uint64_t Get();

	protected:
		/**
		 *
		 * 构造函数
		 *
		 */
		UniqueID();

		/**
		 *
		 * 创建全局唯一id
		 *
		 * @return id
		 *
		 */
		uint64_t Create();

	protected:
		ONLY_ID _id{ };
	};
}


#endif // __TINY_TOOLKIT__ID__UNIQUE__H__
