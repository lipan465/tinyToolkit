#ifndef __TINY_TOOLKIT__LOG__FILE__H__
#define __TINY_TOOLKIT__LOG__FILE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志文件处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API LogFile
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		~LogFile();

		/**
		 *
		 * 是否已经打开文件
		 *
		 * @return 是否已经打开文件
		 *
		 */
		bool IsOpen();

		/**
		 *
		 * 关闭文件
		 *
		 */
		void Close();

		/**
		 *
		 * 刷新缓冲区
		 *
		 */
		void Flush();

		/**
		 *
		 * 打开文件
		 *
		 * @param path 文件路径
		 * @param truncate 是否覆盖文件
		 *
		 * @return 是否打开成功
		 *
		 */
		bool Open(const std::string & path, bool truncate = false);

		/**
		 *
		 * 重新打开文件
		 *
		 * @return 是否打开成功
		 *
		 */
		bool Reopen();

		/**
		 *
		 * 写入文件
		 *
		 * @param value 待写入数据
		 *
		 */
		void Write(const std::string & value);

		/**
		 *
		 * 文件大小
		 *
		 * @return 文件大小
		 *
		 */
		std::size_t Size() const;

		/**
		 *
		 * 文件路径
		 *
		 * @return 文件路径
		 *
		 */
		const std::string & Path() const;

	protected:
		std::size_t _size{ 0 };

		std::string _path{ };

		std::ofstream _file{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__FILE__H__
