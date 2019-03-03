/**
 *
 *  作者: hm
 *
 *  说明: 延时操作
 *
 */


#include "defer.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	DeferHelper::DeferHelper(std::function<void ()> && func) : _func(std::move(func))
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	DeferHelper::~DeferHelper()
	{
		if (_func)
		{
			_func();
		}
	}
}
