#ifndef __TINY_TOOLKIT__UTILITIES__COPYABLE__H__
#define __TINY_TOOLKIT__UTILITIES__COPYABLE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 复制操作
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NonCopyable
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param rhs 实例化对象
		 *
		 */
		NonCopyable(NonCopyable && rhs) noexcept = delete;

		/**
		 *
		 * 构造函数
		 *
		 * @param lhs 实例化对象
		 *
		 */
		NonCopyable(NonCopyable const & lhs) = delete;

		/**
		 *
		 * 重载=操作
		 *
		 * @param rhs 实例化对象
		 *
		 * @return 实例化对象
		 *
		 */
		NonCopyable & operator=(NonCopyable && rhs) noexcept = delete;

		/**
		 *
		 * 重载=操作
		 *
		 * @param lhs 实例化对象
		 *
		 * @return 实例化对象
		 *
		 */
		NonCopyable & operator=(NonCopyable const & lhs) = delete;

	protected:
		/**
		 *
		 * 构造函数
		 *
		 */
		NonCopyable() = default;

		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~NonCopyable() = default;
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__COPYABLE__H__
