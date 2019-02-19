#ifndef __TINY_TOOLKIT__POOL__CALLBACK__H__
#define __TINY_TOOLKIT__POOL__CALLBACK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 回调池
 *
 */


#include "../utilities/operator.h"


namespace tinyToolkit
{
	template<typename ReturnTypeT, typename ...Args>
	class TINY_TOOLKIT_API CallBackPool
	{
		class TINY_TOOLKIT_API Callback
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param id 序列号
			 * @param function 函数
			 *
			 */
			Callback(std::size_t id, std::function<ReturnTypeT(Args...)> function) : _id(id), _function(std::move(function))
			{

			}

			/**
			 *
			 * 序列号
			 *
			 * @return 序列号
			 *
			 */
			std::size_t ID() const
			{
				return _id;
			}

			/**
			 *
			 * 重构()操作
			 *
			 * @param args 参数
			 *
			 */
			void operator () (Args &&... args)
			{
				if (_function)
				{
					_function(std::forward<Args>(args)...);
				}
			}

		protected:
			std::size_t _id{ 0 };

			std::function<ReturnTypeT(Args...)> _function{ };
		};

	public:
		/**
		 *
		 * 回调函数个数
		 *
		 * @return 回调函数个数
		 *
		 */
		std::size_t Size()
		{
			return _pool.size();
		}

		/**
		 *
		 * 注册
		 *
		 * @param function 函数
		 *
		 * @return 序列号
		 *
		 */
		std::size_t Register(std::function<ReturnTypeT(Args...)> function)
		{
			auto callback = Callback(++_id, std::move(function));

			_pool.insert(std::make_pair(callback.ID(), callback));

			return callback.ID();
		}

		/**
		 *
		 * 注销
		 *
		 */
		void UnRegister()
		{
			_id.store(0);

			Operator::Clear(_pool);
		}

		/**
		 *
		 * 注销
		 *
		 * @param id 序列号
		 *
		 */
		void UnRegister(std::size_t id)
		{
			_pool.erase(id);
		}

		/**
		 *
		 * 调用
		 *
		 * @param args 参数
		 *
		 */
		void Call(Args ... args)
		{
			for (auto &callback : _pool)
			{
				try
				{
					callback.second(std::forward<Args>(args)...);
				}
				catch (std::bad_function_call & e)
				{
					/// 调用函数出错
				}
			}
		}

		/**
		 *
		 * 重构()操作
		 *
		 * @param args 参数
		 *
		 */
		void operator()(Args ... args)
		{
			Call(std::forward<Args>(args)...);
		}

		/**
		 *
		 * 重构-=操作
		 *
		 * @param id 序列号
		 *
		 */
		void operator-=(std::size_t id)
		{
			UnRegister(id);
		}

		/**
		 *
		 * 重构+=操作
		 *
		 * @param function 函数
		 *
		 */
		std::size_t operator+=(std::function<ReturnTypeT(Args...)> function)
		{
			return Register(std::move(function));
		}

	protected:
		std::atomic<std::size_t> _id{ 0 };

		std::unordered_map<std::size_t, Callback> _pool{ };
	};
}


#endif // __TINY_TOOLKIT__POOL__CALLBACK__H__
