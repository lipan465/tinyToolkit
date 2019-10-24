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
		 * 移动构造函数
		 *
		 * @param rhs 右值对象
		 *
		 */
		NonCopyable(NonCopyable && rhs) noexcept = delete;

		/**
		 *
		 * 拷贝构造函数
		 *
		 * @param lhs 左值对象
		 *
		 */
		NonCopyable(NonCopyable const & lhs) = delete;

		/**
		 *
		 * 重载=操作
		 *
		 * @param rhs 右值对象
		 *
		 * @return 对象
		 *
		 */
		NonCopyable & operator=(NonCopyable && rhs) noexcept = delete;

		/**
		 *
		 * 重载=操作
		 *
		 * @param lhs 左值对象
		 *
		 * @return 对象
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
