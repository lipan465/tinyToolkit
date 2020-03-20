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
		 * 清空
		 *
		 */
		void Endpoint::Clear()
		{
			port = 0;
			host = "0.0.0.0";
		}

		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool Endpoint::IsValid()
		{
			return port != 0 && !host.empty();
		}
	}
}
