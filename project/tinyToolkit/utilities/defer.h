#ifndef __TINY_TOOLKIT__UTILITIES__DEFER__H__
#define __TINY_TOOLKIT__UTILITIES__DEFER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 延时操作
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class DeferHelper
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		explicit DeferHelper(std::function<void ()> && func);

		/**
		 *
		 * 移动构造函数
		 *
		 * @param rhs 右值对象
		 *
		 */
		DeferHelper(DeferHelper && rhs) = delete;

		/**
		 *
		 * 拷贝构造函数
		 *
		 * @param lhs 左值对象
		 *
		 */
		DeferHelper(const DeferHelper & lhs) = delete;

		/**
		 *
		 * 析构函数
		 *
		 */
		~DeferHelper();

		/**
		 *
		 * =操作符重载
		 *
		 * @param rhs 右值对象
		 *
		 */
		void operator=(DeferHelper && rhs) = delete;

		/**
		 *
		 * =操作符重载
		 *
		 * @param lhs 左值对象
		 *
		 */
		void operator=(const DeferHelper & lhs) = delete;

	private:
		std::function<void ()> _func;
	};
}


#define TINY_TOOLKIT_DEFER(event) tinyToolkit::DeferHelper TINY_TOOLKIT_JOIN_VALUE(_anonymous, __LINE__)([&](){ event; })


#endif // __TINY_TOOLKIT__UTILITIES__DEFER__H__
