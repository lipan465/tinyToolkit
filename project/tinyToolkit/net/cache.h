#ifndef __TINY_TOOLKIT__NET__BUFFER__H__
#define __TINY_TOOLKIT__NET__BUFFER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯缓存
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetCache
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param size 缓存大小
		 *
		 */
		explicit NetCache(std::size_t size);

		/**
		 *
		 * 析构函数
		 *
		 */
		~NetCache();

		/**
		 *
		 * 减少长度
		 *
		 * @param size 待减少长度
		 *
		 * @return 是否减少成功
		 *
		 */
		bool Reduced(std::size_t size);

		/**
		 *
		 * 压入数据
		 *
		 * @param value 待压入数据
		 * @param size 待压入数据长度
		 *
		 * @return 是否压入成功
		 *
		 */
		bool Push(const void * value, std::size_t size);

		/**
		 *
		 * 数据长度
		 *
		 * @return 数据长度
		 *
		 */
		std::size_t Length();

		/**
		 *
		 * 数据长度
		 *
		 * @return 数据长度
		 *
		 */
		std::size_t Length() const;

		/**
		 *
		 * 数据
		 *
		 * @return 数据
		 *
		 */
		const char * Value();

		/**
		 *
		 * 数据
		 *
		 * @return 数据
		 *
		 */
		const char * Value() const;

	private:
		char * _value{ nullptr };

		std::mutex _mutex{ };

		std::size_t _size{ 0 };
		std::size_t _wPos{ 0 };
		std::size_t _rPos{ 0 };
	};
}


#endif // __TINY_TOOLKIT__NET__BUFFER__H__
