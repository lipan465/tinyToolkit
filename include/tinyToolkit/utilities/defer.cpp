/**
 *
 *  作者: hm
 *
 *  说明: 延时操作
 *
 */


#include "defer.h"


/**
 *
 * 延时回调函数
 *
 * @param func 函数
 *
 */
void DeferCallback(std::function<void()> * func)
{
	(*func)();
}