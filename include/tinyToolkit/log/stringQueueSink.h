#ifndef __TINY_TOOLKIT__LOG__STRING_QUEUE_SINK__H__
#define __TINY_TOOLKIT__LOG__STRING_QUEUE_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志字符串队列节点
 *
 */


#include "sink.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API StringQueueLogSink : public ILogSink
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param name 节点名称
		 *
		 */
		explicit StringQueueLogSink(std::string name) : ILogSink(std::move(name))
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~StringQueueLogSink() override
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

		}

		/**
		 *
		 * 重新打开日志
		 *
		 */
		void Reopen() override
		{
			ContainerOperator::Clear(_queue);
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
		bool Empty()
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
		std::size_t Size() const
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
		std::queue<std::string> & Queue()
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
		const std::queue<std::string> & Queue() const
		{
			return _queue;
		}

	protected:
		std::queue<std::string> _queue;
	};
}


#endif // __TINY_TOOLKIT__LOG__STRING_QUEUE_SINK__H__
