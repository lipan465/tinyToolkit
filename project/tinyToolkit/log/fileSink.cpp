/**
 *
 *  作者: hm
 *
 *  说明: 日志文件节点
 *
 */


#include "fileSink.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param name 节点名称
	 * @param path 日志路径
	 * @param truncate 是否覆盖日志
	 *
	 */
	FileLogSink::FileLogSink(std::string name, std::string path, bool truncate) : ILogSink(std::move(name)),
																				  _path(std::move(path))
	{
		if (!_file.Open(_path, truncate))
		{
			throw std::logic_error("Open log file failed : " + _file.Path());
		}
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	FileLogSink::~FileLogSink()
	{
		Close();
	}

	/**
	 *
	 * 关闭日志
	 *
	 */
	void FileLogSink::Close()
	{
		_file.Close();
	}

	/**
	 *
	 * 刷新日志
	 *
	 */
	void FileLogSink::Flush()
	{
		_file.Flush();
	}

	/**
	 *
	 * 重新打开日志
	 *
	 */
	void FileLogSink::Reopen()
	{
		if (!_file.Reopen())
		{
			throw std::logic_error("ReOpen log file failed : " + _file.Path());
		}
	}

	/**
	 *
	 * 写入日志
	 *
	 * @param event 日志事件
	 *
	 */
	void FileLogSink::Write(const LogEvent & event)
	{
		if (Filter() && Filter()->Decide(event))
		{
			return;
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
	bool FileLogSink::IsOpen()
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
	std::size_t FileLogSink::Size() const
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
	const std::string & FileLogSink::Path() const
	{
		return _file.Path();
	}
}
