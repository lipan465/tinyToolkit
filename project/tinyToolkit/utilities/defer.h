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


/**
 *
 * 延时回调函数
 *
 * @param func 函数
 *
 */
void DeferCallback(std::function<void()> * func);


#define TINY_TOOLKIT_DEFER \
	\
	std::function<void()> \
	\
	TINY_TOOLKIT_JOIN_VALUE(_anonymous, __LINE__) \
	\
	__attribute__((cleanup(DeferCallback), unused)) =


#endif // __TINY_TOOLKIT__UTILITIES__DEFER__H__
