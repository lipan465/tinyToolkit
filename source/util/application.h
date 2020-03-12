#ifndef __TINY_TOOLKIT__UTIL__APPLICATION__H__
#define __TINY_TOOLKIT__UTIL__APPLICATION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 应用处理
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <ctime>
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <ctime>
#  include <string>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		class TINY_TOOLKIT_API Application
		{
		public:
			/**
			 *
			 * 是否存在
			 *
			 * @return 是否存在
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
			 * 线程序列号
			 *
			 * @return 线程序列号
			 *
			 */
			static uint64_t ThreadID();

			/**
			 *
			 * 进程序列号
			 *
			 * @return 进程序列号
			 *
			 */
			static uint64_t ProcessID();

			/**
			 *
			 * 编译时间
			 *
			 * @param date 日期
			 * @param time 时间
			 *
			 * @return 编译时间
			 *
			 */
			static std::time_t CompileTime(const char * date = __DATE__, const char * time = __TIME__);

			/**
			 *
			 * 路径
			 *
			 * @return 路径
			 *
			 */
			static std::string Path();

			/**
			 *
			 * 名称
			 *
			 * @return 名称
			 *
			 */
			static std::string Name();

			/**
			 *
			 * 前缀
			 *
			 * @return 前缀
			 *
			 */
			static std::string Steam();

			/**
			 *
			 * 扩展名
			 *
			 * @return 扩展名
			 *
			 */
			static std::string Extension();

			/**
			 *
			 * 父级目录
			 *
			 * @return 父级目录
			 *
			 */
			static std::string ParentDirectory();

			/**
			 *
			 * 编译时间字符串
			 *
			 * @param date 日期
			 * @param time 时间
			 *
			 * @return 编译时间
			 *
			 */
			static std::string CompileTimeString(const char * date = __DATE__, const char * time = __TIME__);
		};
	}
}


#endif // __TINY_TOOLKIT__UTIL__APPLICATION__H__
