/**
 *
 *  作者: hm
 *
 *  说明: 缓存
 *
 */


#include "cache.h"


namespace tinyToolkit
{
	namespace net
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param size 大小
		 *
		 */
		Cache::Cache(std::size_t size) : _size(size)
		{
			_buffer = new char[_size + 1];
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		Cache::~Cache()
		{
			if (_buffer == nullptr)
			{
				return;
			}

			delete[] _buffer;

			_buffer = nullptr;
		}

		/**
		 *
		 * 清空
		 *
		 */
		void Cache::Clear()
		{
			_rPos = 0;
			_wPos = 0;

			_buffer[_wPos] = '\0';
		}

		/**
		 *
		 * 移动
		 *
		 * @param length 长度
		 *
		 */
		void Cache::Move(std::size_t length)
		{
			memmove(_buffer, _buffer + _rPos, length);

			_wPos = length;
			_rPos = 0;

			_buffer[_wPos] = '\0';
		}

		/**
		 *
		 * 整理
		 *
		 * @param length 长度
		 *
		 */
		void Cache::Organize(std::size_t length)
		{
			if (length == 0 || length < OverLength())
			{
				return;
			}

			if (Empty())
			{
				Clear();
			}
			else
			{
				Move(ContentLength());
			}
		}

		/**
		 *
		 * 位移
		 *
		 * @param length 长度
		 *
		 */
		void Cache::DisplacementRead(std::size_t length)
		{
			if (length == 0)
			{
				return;
			}

			if (length > ContentLength())
			{
				length = ContentLength();
			}

			_rPos += length;

			if (Empty())
			{
				Clear();
			}
		}

		/**
		 *
		 * 位移
		 *
		 * @param length 长度
		 *
		 */
		void Cache::DisplacementWrite(std::size_t length)
		{
			if (length == 0)
			{
				return;
			}

			if (length > OverLength())
			{
				length = OverLength();
			}

			_wPos += length;

			_buffer[_wPos] = '\0';
		}

		/**
		 *
		 * 是否已满
		 *
		 * @return 是否已满
		 *
		 */
		bool Cache::Full() const
		{
			return _size == ContentLength();
		}

		/**
		 *
		 * 是否为空
		 *
		 * @return 是否为空
		 *
		 */
		bool Cache::Empty() const
		{
			return _rPos == _wPos;
		}

		/**
		 *
		 * 数据
		 *
		 * @return 数据
		 *
		 */
		char * Cache::Buffer()
		{
			return _buffer + _rPos;
		}

		/**
		 *
		 * 大小
		 *
		 * @return 大小
		 *
		 */
		std::size_t Cache::Size() const
		{
			return _size;
		}

		/**
		 *
		 * 剩余长度
		 *
		 * @return 剩余长度
		 *
		 */
		std::size_t Cache::OverLength() const
		{
			return _size - _wPos;
		}

		/**
		 *
		 * 空闲长度
		 *
		 * @return 空闲长度
		 *
		 */
		std::size_t Cache::FreeLength() const
		{
			return _size - ContentLength();
		}

		/**
		 *
		 * 内容长度
		 *
		 * @return 内容长度
		 *
		 */
		std::size_t Cache::ContentLength() const
		{
			return _wPos - _rPos;
		}
	}
}
