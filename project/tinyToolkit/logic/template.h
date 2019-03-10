#ifndef __TINY_TOOLKIT__LOGIC__TEMPLATE__H__
#define __TINY_TOOLKIT__LOGIC__TEMPLATE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 逻辑模板
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API LogicTemplate
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
		static bool Create(const std::string & path, const std::string & name);

	protected:
		/**
		 *
		 * 创建cmake文件
		 *
		 * @param path 模块路径
		 * @param name 模块名称
		 *
		 * @return 是否创建成功
		 *
		 */
		static bool CreateCmake(const std::string & path, const std::string & name);

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
		static bool CreateMain(const std::string & path, const std::string & name);

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
		static bool CreateModule(const std::string & path, const std::string & name);
	};
}


#endif // __TINY_TOOLKIT__LOGIC__TEMPLATE__H__
