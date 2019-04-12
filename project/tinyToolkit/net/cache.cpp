/**
 *
 *  作者: hm
 *
 *  说明: 通讯缓存
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
		_value = new char[size];
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	NetCache::~NetCache()
	{
		delete[] _value;

		_value = nullptr;
	}

	/**
	 *
	 * 清空
	 *
	 */
	void NetCache::Clear()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_rPos = 0;
		_wPos = 0;

		memset(_value, 0, _size);
	}

	/**
	 *
	 * 减少长度
	 *
	 * @param size 待减少长度
	 *
	 * @return 是否减少成功
	 *
	 */
	bool NetCache::Reduced(std::size_t size)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if (_rPos + size > _wPos)
		{
			return false;
		}
		else
		{
			_rPos += size;

			return true;
		}
	}

	/**
	 *
	 * 压入数据
	 *
	 * @param data 待压入数据指针
	 * @param size 待压入数据长度
	 *
	 * @return 是否压入成功
	 *
	 */
	bool NetCache::Push(const void * data, std::size_t size)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if (_size - Length() < size)
		{
			return false;
		}

		if (_size - _wPos < size)
		{
			if (_rPos == _wPos)
			{
				_rPos = 0;
				_wPos = 0;
			}
			else
			{
				memcpy(_value, _value + _rPos, Length());
			}
		}

		memcpy(_value + _wPos, data, size);

		_wPos += size;

		_value[_wPos] = '\0';

		return true;
	}

	/**
	 *
	 * 数据长度
	 *
	 * @return 数据长度
	 *
	 */
	std::size_t NetCache::Length() const
	{
		return _wPos - _rPos;
	}

	/**
	 *
	 * 数据
	 *
	 * @return 数据
	 *
	 */
	const char * NetCache::Value() const
	{
		return _value + _rPos;
	}
}