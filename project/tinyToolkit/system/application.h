#ifndef __TINY_TOOLKIT__SYSTEM__APPLICATION__H__
#define __TINY_TOOLKIT__SYSTEM__APPLICATION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 程序信息
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Application
	{
	public:
		/**
		 *
		 * 进程是否存在
		 *
		 * @return 进程状态
		 *
		 */
		static bool Exist();

		/**
		 *
		 * 开启守护进程
		 *
		 * @return 是否开启成功
		 *
		 */
		static bool Deamon();

		/**
		 *
		 * 编译时间
		 *
		 * @return 编译时间
		 *
		 */
		static std::time_t CompileTime();

		/**
		 *
		 * 程序完整路径
		 *
		 * @return 程序完整路径
		 *
		 */
		static const std::string & Path();

		/**
		 *
		 * 程序名
		 *
		 * @return 程序名
		 *
		 */
		static const std::string & Name();

		/**
		 *
		 * 程序前缀
		 *
		 * @return 程序前缀
		 *
		 */
		static const std::string & Steam();

		/**
		 *
		 * 程序扩展名
		 *
		 * @return 程序扩展名
		 *
		 */
		static const std::string & Extension();

		/**
		 *
		 * 程序目录
		 *
		 * @return 程序目录
		 *
		 */
		static const std::string & Directory();

		/**
		 *
		 * 编译时间字符串
		 *
		 * @return 编译时间字符串
		 *
		 */
		static const std::string & CompileTimeString();
	};
}


#endif // __TINY_TOOLKIT__SYSTEM__APPLICATION__H__
