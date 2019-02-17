#ifndef __TINY_TOOLKIT__LOGIC__FACTORY__H__
#define __TINY_TOOLKIT__LOGIC__FACTORY__H__


/**
 *
 *  作者: hm
 *
 *  说明: 模块工厂
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API LogicModuleFactory
	{
	public:
		/**
		 *
		 * 创建
		 *
		 * @param path 模块路径
		 * @param name 模块名称
		 *
		 */
		bool Create(const std::string & path, const std::string & name);
	};
}


#endif // __TINY_TOOLKIT__LOGIC__FACTORY__H__
