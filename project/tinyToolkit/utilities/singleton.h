#ifndef __TINY_TOOLKIT__UTILITIES__SINGLETON__H__
#define __TINY_TOOLKIT__UTILITIES__SINGLETON__H__


/**
 *
 *  作者: hm
 *
 *  说明: 单例模式
 *
 */


#include "copyable.h"


namespace tinyToolkit
{
	template <typename TypeT>
	class TINY_TOOLKIT_API Singleton : public NonCopyable
	{
	public:
		/**
		 *
		 * 获取单例
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param args 参数
		 *
		 * @return 单例对象引用
		 *
		 */
		template <typename ... Args>
		static TypeT & Instance(Args &&... args)
		{
			std::call_once(_onceFlag, [&](){ _instance.reset(new TypeT(std::forward<Args>(args)...)); });

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
		 * 析构函数
		 *
		 */
		~Singleton() override = default;

	protected:
		static std::once_flag _onceFlag;

		static std::unique_ptr<TypeT> _instance;
	};

	template <typename TypeT>
	std::once_flag Singleton<TypeT>::_onceFlag;

	template <typename TypeT>
	std::unique_ptr<TypeT> Singleton<TypeT>::_instance;
}


#endif // __TINY_TOOLKIT__UTILITIES__SINGLETON__H__
