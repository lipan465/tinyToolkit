#ifndef __TINY_TOOLKIT__NET__CACHE__H__
#define __TINY_TOOLKIT__NET__CACHE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 缓存
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
		 * 清空
		 *
		 */
		void Clear();

		/**
		 *
		 * 添加数据
		 *
		 * @param buffer 待添加数据缓冲区指针
		 * @param length 待添加数据缓冲区长度
		 *
		 * @return 是否添加成功
		 *
		 */
		bool Append(const void * buffer, std::size_t length);

		/**
		 *
		 * 位移数据
		 *
		 * @param length 待位移长度
		 *
		 * @return 是否位移成功
		 *
		 */
		bool Displacement(std::size_t length);

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
		 * 数据内容
		 *
		 * @return 数据内容
		 *
		 */
		const char * Value();

	private:
		char * _value{ nullptr };

		std::size_t _size{ 0 };
		std::size_t _wPos{ 0 };
		std::size_t _rPos{ 0 };
	};
}


#endif // __TINY_TOOLKIT__NET__CACHE__H__
