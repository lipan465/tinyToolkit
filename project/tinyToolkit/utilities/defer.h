#ifndef __TINY_TOOLKIT__UTILITIES__DEFER__H__
#define __TINY_TOOLKIT__UTILITIES__DEFER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 延时操作
 *
 */


#include "copyable.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API DeferHelper : public NonCopyable
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
		 * 析构函数
		 *
		 */
		~DeferHelper() override;

	private:
		std::function<void ()> _func;
	};
}


#define TINY_TOOLKIT_DEFER(event) tinyToolkit::DeferHelper TINY_TOOLKIT_JOIN_VALUE(_anonymous, __LINE__)([&](){ event; })


#endif // __TINY_TOOLKIT__UTILITIES__DEFER__H__
