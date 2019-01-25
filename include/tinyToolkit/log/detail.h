#ifndef __TINY_TOOLKIT__LOG__DETAIL__H__
#define __TINY_TOOLKIT__LOG__DETAIL__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志类型定义
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	/**
	 *
	 * 日志操作
	 *
	 */
	enum class LOG_OPTION_TYPE : uint8_t
	{
		WRITE,
		FLUSH,
		TERMINATE,
	};

	/**
	 *
	 * 日志优先级
	 *
	 */
	enum class LOG_PRIORITY_TYPE : uint8_t
	{
		DEBUGS,
		INFO,
		NOTICE,
		WARNING,
		ERRORS,
		CRITICAL,
		ALERT,
		FATAL,
		EMERG,
	};
}


#endif // __TINY_TOOLKIT__LOG__DETAIL__H__
