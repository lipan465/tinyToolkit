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
		explicit StringQueueLogSink(std::string name);

		/**
		 *
		 * 析构函数
		 *
		 */
		~StringQueueLogSink() override;

		/**
		 *
		 * 关闭日志
		 *
		 */
		void Close() override;

		/**
		 *
		 * 刷新日志
		 *
		 */
		void Flush() override;

		/**
		 *
		 * 重新打开日志
		 *
		 */
		void Reopen() override;

		/**
		 *
		 * 写入日志
		 *
		 * @param event 日志事件
		 *
		 */
		void Write(const LogEvent & event) override;

		/**
		 *
		 * 队列是否为空
		 *
		 * @return 队列是否为空
		 *
		 */
		bool Empty();

		/**
		 *
		 * 队列大小
		 *
		 * @return 队列大小
		 *
		 */
		std::size_t Size() const;

		/**
		 *
		 * 队列
		 *
		 * @return 队列
		 *
		 */
		std::queue<std::string> & Queue();

		/**
		 *
		 * 队列
		 *
		 * @return 队列
		 *
		 */
		const std::queue<std::string> & Queue() const;

	protected:
		std::queue<std::string> _queue{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__STRING_QUEUE_SINK__H__
