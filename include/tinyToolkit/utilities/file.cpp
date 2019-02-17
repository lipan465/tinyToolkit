/**
 *
 *  作者: hm
 *
 *  说明: 文件操作
 *
 */


#include "file.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param path 待打开文件路径
	 * @param truncate 是否覆盖文件
	 *
	 */
	LockFile::LockFile(std::string path, bool truncate)
	{
		Open(std::move(path), truncate);
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	LockFile::~LockFile()
	{
		Close();
	}

	/**
	 *
	 * 打开文件
	 *
	 * @param path 待打开文件路径
	 * @param truncate 是否覆盖文件
	 *
	 * @return 是否打开成功
	 *
	 */
	bool LockFile::Open(std::string path, bool truncate)
	{
		if (IsOpen())
		{
			return false;
		}

		_path = std::move(path);

		return Create() && Lock() && Execl(truncate);
	}

	/**
	 *
	 * 关闭文件
	 *
	 * @return 是否关闭成功
	 *
	 */
	bool LockFile::Close()
	{
		if (_fd > 0)
		{
			if (::close(_fd) == -1)
			{
				return false;
			}

			_fd = -1;
		}

		return true;
	}

	/**
	 *
	 * 是否已经打开文件
	 *
	 * @return 是否已经打开文件
	 *
	 */
	bool LockFile::IsOpen()
	{
		return _fd > 0;
	}

	/**
	 *
	 * 写入文件
	 *
	 * @param value 待写入数据
	 *
	 * @return 是否写入成功
	 *
	 */
	bool LockFile::Write(const char * value)
	{
		assert(value);

		return Write(value, strlen(value));
	}

	/**
	 *
	 * 写入文件
	 *
	 * @param value 待写入数据
	 *
	 * @return 是否写入成功
	 *
	 */
	bool LockFile::Write(const std::string & value)
	{
		return Write(value, value.size());
	}

	/**
	 *
	 * 写入文件
	 *
	 * @param value 待写入数据
	 * @param size 待写入数据长度
	 *
	 * @return 是否写入成功
	 *
	 */
	bool LockFile::Write(const char * value, std::size_t size)
	{
		assert(value);

		if (!IsOpen())
		{
			return false;
		}

		while (size > 0)
		{
			ssize_t len = ::write(_fd, value, size);

			if (len == -1)
			{
				return false;
			}
			else
			{
				_len += len;

				size -= len;
			}
		}

		return true;
	}

	/**
	 *
	 * 写入文件
	 *
	 * @param value 待写入数据
	 * @param size 待写入数据长度
	 *
	 * @return 是否写入成功
	 *
	 */
	bool LockFile::Write(const std::string & value, std::size_t size)
	{
		return Write(value.c_str(), size);
	}

	/**
	 *
	 * 函数重载
	 *
	 * @param value 待写入数据
	 *
	 * @return 实例化对象
	 *
	 */
	LockFile & LockFile::operator << (const char * value)
	{
		Write(value);

		return *this;
	}

	/**
	 *
	 * 函数重载
	 *
	 * @param data 待写入数据
	 *
	 * @return 实例化对象
	 *
	 */
	LockFile & LockFile::operator << (const std::string & value)
	{
		Write(value);

		return *this;
	}

	/**
	 *
	 * 文件大小
	 *
	 * @return 文件大小
	 *
	 */
	std::size_t LockFile::Size() const
	{
		return _len;
	}

	/**
	 *
	 * 文件路径
	 *
	 * @return 文件路径
	 *
	 */
	const std::string & LockFile::Path() const
	{
		return _path;
	}

	/**
	 *
	 * 锁定
	 *
	 * @return 是否锁定成功
	 *
	 */
	bool LockFile::Lock()
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

		Close();

		return false;

	#else

		struct flock lock
		{
			.l_type = F_WRLCK,  /// 加锁的类型: 只读锁(F_RDLCK), 读写锁(F_WRLCK), 或是解锁(F_UNLCK)
			.l_whence = SEEK_SET,  /// 加锁部分的开始位置
			.l_start = 0,  /// 加锁部分的开始位置
			.l_len = 0,  /// 加锁的长度
			.l_pid = 0,  /// pid
		};

		if (fcntl(_fd, F_SETLK, &lock) == -1)  /// 尝试在整个文件上设置锁定
		{
			Close();

			if (errno == EACCES || errno == EAGAIN)
			{
				/// 代表这个文件已经被别的进程锁定
			}

			return false;
		}

		return true;

	#endif
	}

	/**
	 *
	 * 创建文件
	 *
	 * @return 是否创建成功
	 *
	 */
	bool LockFile::Create()
	{
		_fd = open(_path.c_str(), O_WRONLY | O_CREAT, 0644);

		return IsOpen();
	}

	/**
	 *
	 * 设置描诉符
	 *
	 * @param truncate 是否清空文件
	 *
	 * @return 是否设置成功
	 *
	 */
	bool LockFile::Execl(bool truncate)
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

		Close();

		return false;

	#else

		if (truncate)
		{
			if (ftruncate(_fd, 0) == -1)  /// 清空文件
			{
				Close();

				return false;
			}
		}

		int32_t val = fcntl(_fd, F_GETFD, 0);  /// 获得文件描述符标记

		if (val == -1)
		{
			Close();

			return false;
		}

		/**
		 *
		 * 对描述符设置了FD_CLOEXEC
		 *
		 * 使用execl执行的程序里, 此描述符被关闭, 不能再使用它
		 * 使用fork调用的子进程中, 此描述符并不关闭, 仍可使用
		 *
		 */
		val |= FD_CLOEXEC;

		if (fcntl(_fd, F_SETFD, val) == -1)  /// 设置文件描述符标记
		{
			Close();

			return false;
		}

		return true;

	#endif
	}
}
