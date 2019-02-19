#ifndef __TINY_TOOLKIT__NET__EVENT__H__
#define __TINY_TOOLKIT__NET__EVENT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯事件
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	enum class NET_EVENT_TYPE : uint8_t
	{
		ACCEPT,
		CONNECT,
		TRANSMIT,
	};

	class TINY_TOOLKIT_API NetEvent
	{
	public:
		int32_t _socket{ -1 };

		NET_EVENT_TYPE _type{ NET_EVENT_TYPE::TRANSMIT };

		std::function<void(const NetEvent *, const void *)> _callback;
	};

	class TINY_TOOLKIT_API NetEventPackage
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param host 主机地址
		 * @param port 主机端口
		 * @param value 待发送数据
		 * @param size 待发送数据长度
		 *
		 */
		NetEventPackage(const char * host, uint16_t port, const void * value, std::size_t size) : _port(port),
																								  _host(host),
																								  _value(std::string(reinterpret_cast<const char *>(value), size))
		{

		}

	public:
		uint16_t _port{ 0 };

		std::string _host{ };
		std::string _value{ };
	};
}


#endif // __TINY_TOOLKIT__NET__EVENT__H__
