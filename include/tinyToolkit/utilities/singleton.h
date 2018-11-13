#ifndef __TINY_TOOLKIT__UTILITIES__SINGLETON__H__
#define __TINY_TOOLKIT__UTILITIES__SINGLETON__H__


/**
 *
 *  作者: hm
 *
 *  说明: 单例模式
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	template <class TypeT>
	class TINY_TOOLKIT_API Singleton
	{
	public:
		/**
		 *
		 * 获取单例
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param args 待初始化数据
		 *
		 * @return 单例对象引用
		 *
		 */
		template <class... Args>
		static TypeT & Instance(Args &&... args)
		{
			std::call_once(_onceFlag, [&](){ _instance = new TypeT(std::forward<Args>(args)...); });

			return *_instance;
		}

	protected:
		/**
		 *
		 * 构造函数
		 *
		 */
		Singleton() = default;

		/**
		*
		* 构造函数
		*
		* @param rhs 实例化对象
		*
		*/
		Singleton(const Singleton &) = delete;

		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~Singleton() = default;

		/**
		*
		* 函数重载
		*
		* @param rhs 待赋值实例化
		*
		* @return 实例化对象
		*
		*/
		Singleton & operator=(const Singleton &) = delete;

	protected:
		class GarbageCollector
		{
		public:
			/**
			*
			* 析构函数
			*
			*/
			~GarbageCollector()
			{
				if (Singleton<TypeT>::_instance)
				{
					delete Singleton<TypeT>::_instance;

					Singleton<TypeT>::_instance = nullptr;
				}
			}
		};

	protected:
		static TypeT * _instance;

		static std::once_flag _onceFlag;

		static GarbageCollector _garbageCollector;
	};

	template <class TypeT>
	TypeT * Singleton<TypeT>::_instance = nullptr;

	template <class TypeT>
	std::once_flag Singleton<TypeT>::_onceFlag;

	template <class TypeT>
	typename Singleton<TypeT>::GarbageCollector Singleton<TypeT>::_garbageCollector;
}


#endif // __TINY_TOOLKIT__UTILITIES__SINGLETON__H__
