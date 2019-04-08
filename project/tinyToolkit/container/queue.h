#ifndef __TINY_TOOLKIT__CONTAINER__QUEUE__H__
#define __TINY_TOOLKIT__CONTAINER__QUEUE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 队列
 *
 */


#include "../utilities/copyable.h"
#include "../utilities/operator.h"


namespace tinyToolkit
{
	template<typename TypeT>
	class TINY_TOOLKIT_API LockQueue : public NonCopyable
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		LockQueue() = default;

		/**
		 *
		 * 析构函数
		 *
		 */
		~LockQueue() override
		{
			Operator::Clear(_queue);
		}

		/**
		 *
		 * 压入数据
		 *
		 * @param value 待压入数据
		 *
		 */
		void Push(TypeT && value)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_queue.push(std::move(value));

			_condition.notify_one();
		}

		/**
		 *
		 * 压入数据
		 *
		 * @param value 待压入数据
		 *
		 */
		void Push(const TypeT & value)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_queue.push(std::forward<TypeT>(value));

			_condition.notify_one();
		}

		/**
		 *
		 * 弹出数据
		 *
		 */
		void Pop()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_queue.pop();
		}

		/**
		 *
		 * 弹出数据
		 *
		 * @param value 存储的数据
		 *
		 */
		void Pop(TypeT & value)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			value = std::move(_queue.front());

			_queue.pop();
		}

		/**
		 *
		 * 等待直到有数据弹出
		 *
		 */
		void WaitAndPop()
		{
			std::unique_lock<std::mutex> lock(_mutex);

			_condition.wait
			(
				lock,

				[this]
				{
					return !_queue.empty();
				}
			);

			_queue.pop();
		}

		/**
		 *
		 * 等待直到有数据弹出
		 *
		 * @param value 存储的数据
		 *
		 */
		void WaiAndPop(TypeT & value)
		{
			std::unique_lock<std::mutex> lock(_mutex);

			_condition.wait
			(
				lock,

				[this]
				{
					return !_queue.empty();
				}
			);

			value = std::move(_queue.front());

			_queue.pop();
		}

		/**
		 *
		 * 尝试弹出数据
		 *
		 * @return 是否弹出成功
		 *
		 */
		bool TryPop()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			if (_queue.empty())
			{
				return false;
			}

			_queue.pop();

			return true;
		}

		/**
		 *
		 * 尝试弹出数据
		 *
		 * @param value 存储的数据
		 *
		 * @return 是否弹出成功
		 *
		 */
		bool TryPop(TypeT & value)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			if (_queue.empty())
			{
				return false;
			}

			value = std::move(_queue.front());

			_queue.pop();

			return true;
		}

		/**
		 *
		 * 是否为空
		 *
		 * @return 是否为空
		 *
		 */
		bool Empty() const
		{
			std::lock_guard<std::mutex> lock(_mutex);

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
			std::lock_guard<std::mutex> lock(_mutex);

			return _queue.size();
		}

		/**
		 *
		 * 尾部数据
		 *
		 * @return 尾部数据
		 *
		 */
		const TypeT & Back() const
		{
			std::lock_guard<std::mutex> lock(_mutex);

			return _queue.back();
		}

		/**
		 *
		 * 头部数据
		 *
		 * @return 头部数据
		 *
		 */
		const TypeT & Front() const
		{
			std::lock_guard<std::mutex> lock(_mutex);

			return _queue.front();
		}

	private:
		mutable std::mutex _mutex{ };

		std::queue<TypeT> _queue{ };

		std::condition_variable _condition{ };
	};
}


#endif // __TINY_TOOLKIT__CONTAINER__QUEUE__H__
