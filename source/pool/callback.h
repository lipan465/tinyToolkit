#ifndef __TINY_TOOLKIT__POOL__CALLBACK__H__
#define __TINY_TOOLKIT__POOL__CALLBACK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 回调池
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <functional>
#  include <unordered_map>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <unordered_map>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <functional>
#
#endif


namespace tinyToolkit
{
	namespace pool
	{
		template<typename KeyTypeT, typename ReturnTypeT, typename ...Args>
		class TINY_TOOLKIT_API CallbackPool
		{
		public:
			/**
			 *
			 * 调用
			 *
			 * @param args 参数
			 *
			 */
			void Call(Args &&... args)
			{
				for (auto &iter : _pool)
				{
					if (!iter.second)
					{
						continue;
					}

					iter.second(std::forward<Args>(args)...);
				}
			}

			/**
			 *
			 * 释放
			 *
			 */
			void Release()
			{
				_pool.clear();
			}

			/**
			 *
			 * 注册
			 *
			 * @param key 键值
			 * @param function 函数
			 *
			 * @return 是否注册成功
			 *
			 */
			bool Register(const KeyTypeT & key, std::function<ReturnTypeT(Args...)> function)
			{
				if (_pool.find(key) != _pool.end())
				{
					return false;
				}

				_pool.insert(std::make_pair(key, function));

				return true;
			}

			/**
			 *
			 * 注销
			 *
			 * @param key 键值
			 *
			 */
			bool UnRegister(const KeyTypeT & key)
			{
				auto find = _pool.find(key);

				if (find == _pool.end())
				{
					return false;
				}

				_pool.erase(find);

				return true;
			}

			/**
			 *
			 * 个数
			 *
			 * @return 个数
			 *
			 */
			std::size_t Size()
			{
				return _pool.size();
			}

		private:
			std::unordered_map<KeyTypeT, std::function<ReturnTypeT(Args...)>> _pool{ };
		};
	}
}


#endif // __TINY_TOOLKIT__POOL__CALLBACK__H__
