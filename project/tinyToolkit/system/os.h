#ifndef __TINY_TOOLKIT__SYSTEM__OS__H__
#define __TINY_TOOLKIT__SYSTEM__OS__H__


/**
 *
 *  作者: hm
 *
 *  说明: 进程信息
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API OS
	{
	public:
		/**
		 *
		 * 是否是大端字节序
		 *
		 * @return 是否是大端字节序
		 *
		 */
		static bool IsBigEndian();

		/**
		 *
		 * 是否是小端字节序
		 *
		 * @return 是否是小端字节序
		 *
		 */
		static bool IsLittleEndian();

		/**
		 *
		 * 获取处理器个数
		 *
		 * @return 处理器个数
		 *
		 */
		static int64_t ProcessorCount();

		/**
		 *
		 * 获取线程id
		 *
		 * @return 线程id
		 *
		 */
		static uint64_t ThreadID();

		/**
		 *
		 * 获取当前进程id
		 *
		 * @return pid
		 *
		 */
		static uint64_t ProcessID();

		/**
		 *
		 * 获取用户名
		 *
		 * @return 用户名
		 *
		 */
		static std::string UserName();

		/**
		 *
		 * 获取计算机名称
		 *
		 * @return 计算机名称
		 *
		 */
		static std::string ComputerName();

		/**
		 *
		 * 获取最后一个错误信息
		 *
		 * @return 最后一个错误信息
		 *
		 */
		static std::string LastErrorMessage();
	};
}


#endif // __TINY_TOOLKIT__SYSTEM__OS__H__
