/**
 *
 *  作者: hm
 *
 *  说明: 文件处理
 *
 */


#include "file.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <stdexcept>
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 析构函数
		 *
		 */
		File::~File()
		{
			Close();
		}

		/**
		 *
		 * 关闭文件
		 *
		 */
		void File::Close()
		{
			if (!IsOpen())
			{
				return;
			}

			_size = 0;

			_file.flush();
			_file.close();
		}

		/**
		 *
		 * 刷新内容
		 *
		 */
		void File::Flush()
		{
			if (!IsOpen())
			{
				return;
			}

			_file.flush();
		}

		/**
		 *
		 * 写入内容
		 *
		 * @param content 内容
		 *
		 */
		void File::Write(const std::string & content)
		{
			if (!IsOpen())
			{
				return;
			}

			_file << content;

			_size += content.size();
		}

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
		bool File::Open(const std::string & path, bool truncate)
		{
			if (IsOpen())
			{
				throw std::runtime_error("Handle is already in use");
			}

			_path = path;

			_file.open(_path, truncate ? std::ios::binary : std::ios::binary | std::ios::app);

			if (!IsOpen())
			{
				return false;
			}

			_size += static_cast<std::size_t>(_file.tellp());

			return true;
		}

		/**
		 *
		 * 是否已经打开
		 *
		 * @return 是否已经打开
		 *
		 */
		bool File::IsOpen()
		{
			return _file.is_open();
		}

		/**
		 *
		 * 重新打开
		 *
		 * @return 是否打开成功
		 *
		 */
		bool File::Reopen()
		{
			Close();

			return Open(_path, true);
		}

		/**
		 *
		 * 大小
		 *
		 * @return 大小
		 *
		 */
		std::size_t File::Size()
		{
			return _size;
		}

		/**
		 *
		 * 路径
		 *
		 * @return 路径
		 *
		 */
		const std::string & File::Path()
		{
			return _path;
		}
	}
}
