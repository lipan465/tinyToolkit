/**
 *
 *  作者: hm
 *
 *  说明: 地址
 *
 */


#include "address.h"


namespace tinyToolkit
{
	/**
	 *
	 * 是否有效
	 *
	 * @return 是否有效
	 *
	 */
	bool NetAddress::IsValid()
	{
		return port != 0 && !host.empty();
	}
}
