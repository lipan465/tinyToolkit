#ifndef __TINY_TOOLKIT__LOG__FILE__H__
#define __TINY_TOOLKIT__LOG__FILE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志事件
 *
 */


#include "detail.h"


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
		~LogFile()
		{
			Close();
		}

		/**
		 *
		 * 是否已经打开文件
		 *
		 * @return 是否已经打开文件
		 *
		 */
		bool IsOpen()
		{
			return _file.is_open();
		}

		/**
		 *
		 * 关闭文件
		 *
		 */
		void Close()
		{
			if (IsOpen())
			{
				_size = 0;

				_file.flush();
				_file.close();
			}
		}

		/**
		 *
		 * 刷新缓冲区
		 *
		 */
		void Flush()
		{
			if (IsOpen())
			{
				_file.flush();
			}
		}

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
		bool Open(const std::string & path, bool truncate = false)
		{
			if (IsOpen())
			{
				return false;
			}

			_path = path;

			_file.open(_path, truncate ? std::ios::binary : std::ios::binary | std::ios::app);

			if (IsOpen())
			{
				_size += _file.tellp();

				return true;
			}
			else
			{
				return false;
			}
		}

		/**
		 *
		 * 重新打开文件
		 *
		 * @return 是否打开成功
		 *
		 */
		bool Reopen()
		{
			Close();

			return Open(_path, true);
		}

		/**
		 *
		 * 写入文件
		 *
		 * @param value 待写入数据
		 *
		 */
		void Write(const std::string & value)
		{
			if (IsOpen())
			{
				_file << value;

				_size += value.size();
			}
		}

		/**
		 *
		 * 文件大小
		 *
		 * @return 文件大小
		 *
		 */
		std::size_t Size() const
		{
			return _size;
		}

		/**
		 *
		 * 文件路径
		 *
		 * @return 文件路径
		 *
		 */
		const std::string & Path() const
		{
			return _path;
		}

	protected:
		std::size_t _size{ 0 };

		std::string _path{ };

		std::ofstream _file{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__FILE__H__
