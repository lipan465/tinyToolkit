/**
 *
 *  作者: hm
 *
 *  说明: 通讯信息
 *
 */


#include "message.h"

#include "../utilities/net.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param ip 远端地址
	 * @param port 远端端口
	 * @param data 待发送数据
	 * @param size 待发送数据长度
	 *
	 */
	NetMessage::NetMessage(const char * ip, uint16_t port, const void * data, std::size_t size) : _ip(Net::AsHostByte(ip)), _port(port), _size(size)
	{
		_data = new char[size + 1];

		memcpy(_data, data, size);

		_data[size] = '\0';
	}

	/**
	 *
	 * 拷贝构造函数
	 *
	 * @param rhs 右值对象
	 *
	 */
	NetMessage::NetMessage(tinyToolkit::NetMessage && rhs) noexcept : _data(rhs._data), _ip(rhs._ip), _port(rhs._port), _size(rhs._size)
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
