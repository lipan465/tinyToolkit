/**
 *
 *  作者: hm
 *
 *  说明: 端点
 *
 */


#include "endpoint.h"


namespace tinyToolkit
{
	namespace net
	{
		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool Endpoint::IsValid() const
		{
			return port != 0 && !host.empty();
		}
	}
}
