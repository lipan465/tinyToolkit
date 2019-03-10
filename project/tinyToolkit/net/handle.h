#ifndef __TINY_TOOLKIT__NET__HANDLE__H__
#define __TINY_TOOLKIT__NET__HANDLE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯句柄
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	typedef union
	{
		void * handle;

		TINY_TOOLKIT_SOCKET_TYPE socket;
	}NetHandle;
}


#endif // __TINY_TOOLKIT__NET__HANDLE__H__
