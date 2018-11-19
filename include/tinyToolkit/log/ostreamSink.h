#ifndef __TINY_TOOLKIT__LOG__O_STREAM_SINK__H__
#define __TINY_TOOLKIT__LOG__O_STREAM_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 输出流日志节点
 *
 */


#include "sink.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API OStreamSink : public ILogSink
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		OStreamSink() = default;

		/**
		 *
		 * 构造函数
		 *
		 * @param stream 输出流
		 *
		 */
		explicit OStreamSink(std::shared_ptr<std::ostream> stream) : _stream(std::move(stream))
		{

		};

		/**
		 *
		 * 析构函数
		 *
		 */
		~OStreamSink() override
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
			Flush();
		}

		/**
		 *
		 * 刷新日志
		 *
		 */
		void Flush() override
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
		void Reopen() override
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
		void Write(const LogEvent & event) override
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
		 * 设置输出流
		 *
		 * @param stream 输出流
		 *
		 */
		void SetStream(const std::shared_ptr<std::ostream> & stream)
		{
			_stream = stream;
		}

		/**
		 *
		 * 输出流
		 *
		 * @return 输出流
		 *
		 */
		const std::shared_ptr<std::ostream> & Stream() const
		{
			return _stream;
		}

	protected:
		std::shared_ptr<std::ostream> _stream{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__O_STREAM_SINK__H__
