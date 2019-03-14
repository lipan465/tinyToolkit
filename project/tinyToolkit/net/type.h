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
		IO,
		SEND,
		ACCEPT,
		CONNECT,
		RECEIVE,
		INVALID,
	};
}


#endif // __TINY_TOOLKIT__NET__TYPE__H__
