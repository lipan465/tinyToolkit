#ifndef __TINY_TOOLKIT__NET__SOCKET__H__
#define __TINY_TOOLKIT__NET__SOCKET__H__


/**
 *
 *  作者: hm
 *
 *  说明: 通讯设置
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Socket
	{
	public:
		/**
		 *
		 * 设置非堵塞
		 *
		 * @param socket 句柄
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool SetNonBlocking(int32_t socket);

		/**
		 *
		 * 设置Nagle算法
		 *
		 * @param socket 句柄
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool SetNodelay(int32_t socket);

		/**
		 *
		 * 设置延迟确认
		 *
		 * @param socket 句柄
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool SetQuickAck(int32_t socket);

		/**
		 *
		 * 设置端口复用
		 *
		 * @param socket 句柄
		 *
		 * @return 是否设置成功
		 *
		 */
		static bool SetReuseAddress(int32_t socket);
	};
}


#endif // __TINY_TOOLKIT__NET__SOCKET__H__
