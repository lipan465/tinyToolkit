/**
 *
 *  作者: hm
 *
 *  说明: 日志字符串队列节点
 *
 */


#include "stringQueueSink.h"

#include "../utilities/operator.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param name 节点名称
	 *
	 */
	StringQueueLogSink::StringQueueLogSink(std::string name) : ILogSink(std::move(name))
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	StringQueueLogSink::~StringQueueLogSink()
	{
		Close();
	}

	/**
	 *
	 * 关闭日志
	 *
	 */
	void StringQueueLogSink::Close()
	{
		Flush();
	}

	/**
	 *
	 * 刷新日志
	 *
	 */
	void StringQueueLogSink::Flush()
	{

	}

	/**
	 *
	 * 重新打开日志
	 *
	 */
	void StringQueueLogSink::Reopen()
	{
		Operator::Clear(_queue);
	}

	/**
	 *
	 * 写入日志
	 *
	 * @param event 日志事件
	 *
	 */
	void StringQueueLogSink::Write(const LogEvent & event)
	{
		if (Filter() && Filter()->Decide(event))
		{
			return;
		}

		_queue.push(Layout() ? Layout()->Format(event) : event.message);

		if (_autoFlush)
		{
			Flush();
		}
	}

	/**
	 *
	 * 队列是否为空
	 *
	 * @return 队列是否为空
	 *
	 */
	bool StringQueueLogSink::Empty()
	{
		return _queue.empty();
	}

	/**
	 *
	 * 队列大小
	 *
	 * @return 队列大小
	 *
	 */
	std::size_t StringQueueLogSink::Size() const
	{
		return _queue.size();
	}

	/**
	 *
	 * 队列
	 *
	 * @return 队列
	 *
	 */
	std::queue<std::string> & StringQueueLogSink::Queue()
	{
		return _queue;
	}

	/**
	 *
	 * 队列
	 *
	 * @return 队列
	 *
	 */
	const std::queue<std::string> & StringQueueLogSink::Queue() const
	{
		return _queue;
	}
}
