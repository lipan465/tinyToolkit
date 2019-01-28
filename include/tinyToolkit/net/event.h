#ifndef __TINY_TOOLKIT__NET__EVENT__H__
#define __TINY_TOOLKIT__NET__EVENT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 事件
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	enum class EVENT_TYPE : uint8_t
	{
		ACCEPT,
		CONNECT,
		TRANSMIT,
	};

	class TINY_TOOLKIT_API EventValue
	{
	public:
		int32_t socket{ -1 };

		EVENT_TYPE type{ EVENT_TYPE::TRANSMIT };

		std::function<void(const EventValue *, const struct epoll_event &)> callback;
	};

	class TINY_TOOLKIT_API EventPackage
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
		EventPackage(const char * host_, uint16_t port_, const void * value_, std::size_t size_) : port(port_),
																								   host(host_),
																								   value(std::string(reinterpret_cast<const char *>(value_), size_))
		{

		}

	public:
		uint16_t port{ 0 };

		std::string host{ };
		std::string value{ };
	};
}


#endif // __TINY_TOOLKIT__NET__EVENT__H__
