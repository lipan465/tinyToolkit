#ifndef __TINY_TOOLKIT__LOG__FILE__H__
#define __TINY_TOOLKIT__LOG__FILE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 文件处理
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <fstream>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <fstream>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <fstream>
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API File
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			~File();

			/**
			 *
			 * 关闭文件
			 *
			 */
			void Close();

			/**
			 *
			 * 刷新内容
			 *
			 */
			void Flush();

			/**
			 *
			 * 写入内容
			 *
			 * @param content 内容
			 *
			 */
			void Write(const std::string & content);

			/**
			 *
			 * 打开文件
			 *
			 * @param path 文件路径
			 * @param truncate 是否截断
			 *
			 * @return 是否打开成功
			 *
			 */
			bool Open(const std::string & path, bool truncate = false);

			/**
			 *
			 * 是否已经打开
			 *
			 * @return 是否已经打开
			 *
			 */
			bool IsOpen() const;

			/**
			 *
			 * 重新打开
			 *
			 * @return 是否打开成功
			 *
			 */
			bool Reopen();

			/**
			 *
			 * 大小
			 *
			 * @return 大小
			 *
			 */
			std::size_t Size() const;

			/**
			 *
			 * 路径
			 *
			 * @return 路径
			 *
			 */
			const std::string & Path() const;

		private:
			std::size_t _size{ 0 };

			std::string _path{ };

			std::ofstream _file{ };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__FILE__H__
