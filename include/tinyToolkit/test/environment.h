#ifndef __TINY_TOOLKIT__TEST__ENVIRONMENT__H__
#define __TINY_TOOLKIT__TEST__ENVIRONMENT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 测试环境
 *
 */

#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TestEnvironment
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~TestEnvironment() = default;

		/**
		 *
		 * 设置环境
		 *
		 */
		virtual void SetUp();

		/**
		 *
		 * 销毁环境
		 *
		 */
		virtual void TearDown();
	};
}


#endif // __TINY_TOOLKIT__TEST__ENVIRONMENT__H__
