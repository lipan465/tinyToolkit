/**
 *
 *  作者: hm
 *
 *  说明: 日志回卷文件节点
 *
 */


#include "rotatingFileSink.h"

#include "../utilities/string.h"
#include "../utilities/filesystem.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param name 节点名称
	 * @param path 日志路径
	 * @param limit 日志大小限制
	 * @param count 日志个数限制
	 *
	 */
	RotatingFileLogSink::RotatingFileLogSink(std::string name, std::string path, std::size_t limit, std::size_t count) : ILogSink(std::move(name)),
																														 _count(count),
																														 _limit(limit),
																														 _path(std::move(path))
	{
		if (!_file.Open(CalculatePath(_path)))
		{
			throw std::logic_error("Open rotating log file failed : " + _file.Path());
		}
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	RotatingFileLogSink::~RotatingFileLogSink()
	{
		Close();
	}

	/**
	 *
	 * 关闭日志
	 *
	 */
	void RotatingFileLogSink::Close()
	{
		_file.Close();
	}

	/**
	 *
	 * 刷新日志
	 *
	 */
	void RotatingFileLogSink::Flush()
	{
		_file.Flush();
	}

	/**
	 *
	 * 重新打开日志
	 *
	 */
	void RotatingFileLogSink::Reopen()
	{
		if (!_file.Reopen())
		{
			throw std::logic_error("ReOpen rotating log file failed : " + _file.Path());
		}
	}

	/**
	 *
	 * 写入日志
	 *
	 * @param event 日志事件
	 *
	 */
	void RotatingFileLogSink::Write(const LogEvent & event)
	{
		if (Filter() && Filter()->Decide(event))
		{
			return;
		}

		if (_file.Size() > _limit)
		{
			_file.Close();

			Rotating();

			if (!_file.Open(CalculatePath(_path)))
			{
				throw std::logic_error("Open rotating log file failed : " + _file.Path());
			}
		}

		_file.Write(Layout() ? Layout()->Format(event) : event.message);

		if (_autoFlush)
		{
			Flush();
		}
	}

	/**
	 *
	 * 是否已经打开
	 *
	 * @return 是否已经打开
	 *
	 */
	bool RotatingFileLogSink::IsOpen()
	{
		return _file.IsOpen();
	}

	/**
	 *
	 * 日志大小
	 *
	 * @return 日志大小
	 *
	 */
	std::size_t RotatingFileLogSink::Size() const
	{
		return _file.Size();
	}

	/**
	 *
	 * 日志路径
	 *
	 * @return 日志路径
	 *
	 */
	const std::string & RotatingFileLogSink::Path() const
	{
		return _file.Path();
	}

	/**
	 *
	 * 轮转日志
	 *
	 */
	void RotatingFileLogSink::Rotating()
	{
		for (std::size_t i = _count; i > 0; --i)
		{
			std::string srcPath = CalculatePath(_path, i - 1);
			std::string dstPath = CalculatePath(_path, i);

			if (Filesystem::Exists(dstPath))
			{
				Filesystem::Remove(dstPath);
			}

			if (Filesystem::Exists(srcPath))
			{
				Filesystem::Rename(srcPath, dstPath);
			}
		}
	}

	/**
	 *
	 * 推导日志路径
	 *
	 * @param path 日志路径
	 * @param index 索引
	 *
	 * @return 日志路径
	 *
	 */
	std::string RotatingFileLogSink::CalculatePath(const std::string & path, std::size_t index)
	{
		if (index == 0)
		{
			return path;
		}

		std::size_t pos = path.rfind('.');

		if (pos == std::string::npos)  /// name_1
		{
			return String::Format("{}_{}", path, index);
		}
		else if (pos == 0)  /// 1.log
		{
			return String::Format("{}{}", index, path);
		}
		else  /// name_1.log
		{
			return String::Format("{}_{}{}", path.substr(0, pos), index, path.substr(pos));
		}
	}
}
