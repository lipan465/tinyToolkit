#ifndef __TINY_TOOLKIT__NET__CACHE__H__
#define __TINY_TOOLKIT__NET__CACHE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 缓存
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <cstring>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <cstring>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstring>
#
#endif


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API Cache
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param size 大小
			 *
			 */
			explicit Cache(std::size_t size);

			/**
			 *
			 * 析构函数
			 *
			 */
			~Cache();

			/**
			 *
			 * 清空
			 *
			 */
			void Clear();

			/**
			 *
			 * 移动
			 *
			 * @param length 长度
			 *
			 */
			void Move(std::size_t length);

			/**
			 *
			 * 整理
			 *
			 * @param length 长度
			 *
			 */
			void Organize(std::size_t length);

			/**
			 *
			 * 位移
			 *
			 * @param length 长度
			 *
			 */
			void DisplacementRead(std::size_t length);

			/**
			 *
			 * 位移
			 *
			 * @param length 长度
			 *
			 */
			void DisplacementWrite(std::size_t length);

			/**
			 *
			 * 是否已满
			 *
			 * @return 是否已满
			 *
			 */
			bool Full() const;

			/**
			 *
			 * 是否为空
			 *
			 * @return 是否为空
			 *
			 */
			bool Empty() const;

			/**
			 *
			 * 数据
			 *
			 * @return 数据
			 *
			 */
			char * Buffer();

			/**
			 *
			 * 大小
			 *
			 * @return 大小
			 *
			 */
			std::size_t Size() const;

			/**
			 *
			 * 剩余长度
			 *
			 * @return 剩余长度
			 *
			 */
			std::size_t OverLength() const;

			/**
			 *
			 * 空闲长度
			 *
			 * @return 空闲长度
			 *
			 */
			std::size_t FreeLength() const;

			/**
			 *
			 * 内容长度
			 *
			 * @return 内容长度
			 *
			 */
			std::size_t ContentLength() const;

		private:
			char * _buffer{ nullptr };

			std::size_t _size{ 0 };
			std::size_t _wPos{ 0 };
			std::size_t _rPos{ 0 };
		};
	}
}


#endif // __TINY_TOOLKIT__NET__CACHE__H__
