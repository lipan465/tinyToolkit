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

	typedef struct TINY_TOOLKIT_API EventValue
	{
		int32_t socket{ -1 };

		EVENT_TYPE type{ EVENT_TYPE::TRANSMIT };

		std::function<void(const EventValue *, const struct epoll_event &)> callback;
	}EventValue;
}


#endif // __TINY_TOOLKIT__NET__EVENT__H__
