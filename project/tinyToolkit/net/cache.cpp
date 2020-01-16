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
	/**
	 *
	 * 构造函数
	 *
	 * @param size 缓存大小
	 *
	 */
	NetCache::NetCache(std::size_t size) : _size(size)
	{
		_value = new char[size + 1];
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	NetCache::~NetCache()
	{
		if (_value)
		{
			delete[] _value;

			_value = nullptr;
		}
	}

	/**
	 *
	 * 清空
	 *
	 */
	void NetCache::Clear()
	{
		_rPos = 0;
		_wPos = 0;

		memset(_value, 0, _size);
	}

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
	bool NetCache::Append(const void * buffer, std::size_t length)
	{
		if (length == 0)
		{
			return false;
		}

		if ((_size - Length()) < length)
		{
			return false;
		}

		if ((_size - _wPos) < length)
		{
			if (_rPos == _wPos)
			{
				_wPos = 0;
				_rPos = 0;
			}
			else
			{
				memmove(_value, _value + _rPos, Length());

				_wPos = Length();
				_rPos = 0;
			}
		}

		memcpy(_value + _wPos, buffer, length);

		_wPos += length;

		_value[_wPos] = '\0';

		return true;
	}

	/**
	 *
	 * 位移数据
	 *
	 * @param length 待位移长度
	 *
	 * @return 是否位移成功
	 *
	 */
	bool NetCache::Displacement(std::size_t length)
	{
		if ((_rPos + length) > _wPos)
		{
			return false;
		}
		else
		{
			_rPos += length;

			return true;
		}
	}

	/**
	 *
	 * 数据长度
	 *
	 * @return 数据长度
	 *
	 */
	std::size_t NetCache::Length()
	{
		return _wPos - _rPos;
	}

	/**
	 *
	 * 数据内容
	 *
	 * @return 数据内容
	 *
	 */
	const char * NetCache::Value()
	{
		return _value + _rPos;
	}
}
