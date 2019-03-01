/**
 *
 *  作者: hm
 *
 *  说明: 日志输出流节点
 *
 */


#include "ostreamSink.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param name 节点名称
	 * @param stream 输出流
	 *
	 */
	OStreamLogSink::OStreamLogSink(std::string name, std::shared_ptr<std::ostream> stream) : ILogSink(std::move(name)),
																							 _stream(std::move(stream))
	{

	};

	/**
	 *
	 * 析构函数
	 *
	 */
	OStreamLogSink::~OStreamLogSink()
	{
		Close();
	}

	/**
	 *
	 * 关闭日志
	 *
	 */
	void OStreamLogSink::Close()
	{
		Flush();
	}

	/**
	 *
	 * 刷新日志
	 *
	 */
	void OStreamLogSink::Flush()
	{
		if (_stream)
		{
			_stream->flush();
		}
	}

	/**
	 *
	 * 重新打开日志
	 *
	 */
	void OStreamLogSink::Reopen()
	{
		if (_stream)
		{
			_stream->clear();  /// 清除错误标志
		}
	}

	/**
	 *
	 * 写入日志
	 *
	 * @param event 日志事件
	 *
	 */
	void OStreamLogSink::Write(const LogEvent & event)
	{
		if (Filter() && Filter()->Decide(event))
		{
			return;
		}

		if (_stream)
		{
			*_stream << (Layout() ? Layout()->Format(event) : event.message);
		}

		if (_autoFlush)
		{
			Flush();
		}
	}

	/**
	 *
	 * 输出流
	 *
	 * @return 输出流
	 *
	 */
	const std::shared_ptr<std::ostream> & OStreamLogSink::Stream() const
	{
		return _stream;
	}
}
