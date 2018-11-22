#ifndef __TINY_TOOLKIT__LOG__DETAIL__H__
#define __TINY_TOOLKIT__LOG__DETAIL__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志类型定义
 *
 */


#include "../system/os.h"
#include "../utilities/time.h"
#include "../utilities/lock.h"
#include "../utilities/string.h"
#include "../system/application.h"
#include "../utilities/singleton.h"
#include "../utilities/container.h"
#include "../utilities/filesystem.h"


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
		DEBUG,
		INFO,
		NOTICE,
		WARNING,
		ERROR,
		CRITICAL,
		ALERT,
		FATAL,
		EMERG,
	};
}


#endif // __TINY_TOOLKIT__LOG__DETAIL__H__
