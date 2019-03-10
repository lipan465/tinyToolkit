#ifndef __TINY_TOOLKIT__NET__TYPE__H__
#define __TINY_TOOLKIT__NET__TYPE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯类型
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	enum class NET_EVENT_TYPE : uint8_t
	{
		SEND,
		ACCEPT,
		CONNECT,
		RECEIVE,
		TRANSMIT,
	};
}


#endif // __TINY_TOOLKIT__NET__TYPE__H__
