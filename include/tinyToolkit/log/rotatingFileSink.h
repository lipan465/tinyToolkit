#ifndef __TINY_TOOLKIT__LOG__ROTATING_FILE_SINK__H__
#define __TINY_TOOLKIT__LOG__ROTATING_FILE_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志回卷文件节点
 *
 */


#include "sink.h"
#include "file.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API RotatingLogFileSink : public ILogSink
	{
	public:
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
		explicit RotatingLogFileSink(std::string name, std::string path, std::size_t limit = TINY_TOOLKIT_GB, std::size_t count = 10) : ILogSink(std::move(name)),
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
		~RotatingLogFileSink() override
		{
			Close();
		}

		/**
		 *
		 * 关闭日志
		 *
		 */
		void Close() override
		{
			_file.Close();
		}

		/**
		 *
		 * 刷新日志
		 *
		 */
		void Flush() override
		{
			_file.Flush();
		}

		/**
		 *
		 * 重新打开日志
		 *
		 */
		void Reopen() override
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
		void Write(const LogEvent & event) override
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
		bool IsOpen()
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
		std::size_t Size() const
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
		const std::string & Path() const
		{
			return _file.Path();
		}

	protected:
		/**
		 *
		 * 轮转日志
		 *
		 */
		void Rotating()
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
		std::string CalculatePath(const std::string & path, std::size_t index = 0)
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

	protected:
		LogFile _file{ };

		std::size_t _count{ 10 };
		std::size_t _limit{ TINY_TOOLKIT_GB };

		std::string _path{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__ROTATING_FILE_SINK__H__
