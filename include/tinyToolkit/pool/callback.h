#ifndef __TINY_TOOLKIT__POOL__CALLBACK__H__
#define __TINY_TOOLKIT__POOL__CALLBACK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 回调池
 *
 */


#include "../utilities/time.h"
#include "../utilities/container.h"


namespace tinyToolkit
{
	template<typename ReturnTypeT, typename ...Args>
	class TINY_TOOLKIT_API CallBackPool
	{
		typedef ReturnTypeT (*FunctionTypeT)(Args...);

		class Callback
		{
		public:
			Callback(std::size_t id, std::function<ReturnTypeT(Args...)> function) : _id(id), _function(std::move(function))
			{

			}

			std::size_t ID() const
			{
				return _id;
			}

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
		std::size_t Size()
		{
			return _pool.size();
		}

		std::size_t Register(FunctionTypeT function)
		{
			return Register(std::function<ReturnTypeT(Args...)>(function));
		}

		std::size_t Register(std::function<ReturnTypeT(Args...)> function)
		{
			auto callback = Callback(++_id, std::move(function));

			_pool.insert(std::make_pair(callback.ID(), callback));

			return callback.ID();
		}

		void UnRegister()
		{
			_id.store(0);

			tinyToolkit::Container::Clear(_pool);
		}

		void UnRegister(std::size_t id)
		{
			_pool.erase(id);
		}

		void Call(Args &&... args)
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

		void operator () (Args &&... args)
		{
			Call(std::forward<Args>(args)...);
		}

		void operator -= (std::size_t id)
		{
			UnRegister(id);
		}

		std::size_t operator += (FunctionTypeT function)
		{
			return Register(function);
		}

		std::size_t operator += (std::function<ReturnTypeT(Args...)> function)
		{
			return Register(function);
		}

	protected:
		std::atomic<std::size_t> _id{ 0 };

		std::unordered_map<std::size_t, Callback> _pool;
	};
}


#endif // __TINY_TOOLKIT__POOL__CALLBACK__H__
