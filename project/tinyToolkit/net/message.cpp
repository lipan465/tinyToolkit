/**
 *
 *  作者: hm
 *
 *  说明: 通讯信息
 *
 */


#include "message.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param data 待发送数据指针
	 * @param size 待发送数据长度
	 *
	 */
	NetMessage::NetMessage(const void * data, std::size_t size) : _size(size)
	{
		_data = new char[_size + 1];

		memcpy(_data, data, _size);

		_data[_size] = '\0';
	}

	/**
	 *
	 * 拷贝构造函数
	 *
	 * @param rhs 右值对象
	 *
	 */
	NetMessage::NetMessage(tinyToolkit::NetMessage && rhs) noexcept : _data(rhs._data), _size(rhs._size)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	NetMessage::~NetMessage()
	{
		delete[] _data;

		_data = nullptr;
	}
}
