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
		 * @return 是否创建成功
		 *
		 */
		bool Create(const std::string & path, const std::string & name);

	protected:
		/**
		 *
		 * 创建主文件
		 *
		 * @param path 模块路径
		 * @param name 模块名称
		 *
		 * @return 是否创建成功
		 *
		 */
		bool CreateMain(const std::string & path, const std::string & name);

		/**
		 *
		 * 创建模块文件
		 *
		 * @param path 模块路径
		 * @param name 模块名称
		 *
		 * @return 是否创建成功
		 *
		 */
		bool CreateModule(const std::string & path, const std::string & name);
	};
}


#endif // __TINY_TOOLKIT__LOGIC__FACTORY__H__
