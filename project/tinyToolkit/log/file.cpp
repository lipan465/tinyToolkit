/**
 *
 *  作者: hm
 *
 *  说明: 日志文件处理
 *
 */


#include "file.h"


namespace tinyToolkit
{
	/**
	 *
	 * 析构函数
	 *
	 */
	LogFile::~LogFile()
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
	bool LogFile::IsOpen()
	{
		return _file.is_open();
	}

	/**
	 *
	 * 关闭文件
	 *
	 */
	void LogFile::Close()
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
	void LogFile::Flush()
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
	bool LogFile::Open(const std::string & path, bool truncate)
	{
		if (IsOpen())
		{
			return false;
		}

		_path = path;

		_file.open(_path, truncate ? std::ios::binary : std::ios::binary | std::ios::app);

		if (IsOpen())
		{
			_size += static_cast<std::size_t>(_file.tellp());

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
	bool LogFile::Reopen()
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
	void LogFile::Write(const std::string & value)
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
	std::size_t LogFile::Size() const
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
	const std::string & LogFile::Path() const
	{
		return _path;
	}
}
