#ifndef __TINY_TOOLKIT__LOGIC__MANAGER__H__
#define __TINY_TOOLKIT__LOGIC__MANAGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 模块管理器
 *
 */


#include "module.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API LogicModuleManager
	{
	public:
		/**
		 *
		 * 启动
		 *
		 */
		void Launch();

		/**
		 *
		 * 释放
		 *
		 */
		void Release();

		/**
		 *
		 * 加载模块
		 *
		 * @param path 模块路径
		 *
		 * @return 是否加载成功
		 *
		 */
		bool LoadModule(const char * path);

		/**
		 *
		 * 查找模块
		 *
		 * @param name 名称
		 *
		 * @return 模块
		 *
		 */
		ILogicModule * Find(const std::string & name);

	private:
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		HINSTANCE _handle{ };

#else

		void * _handle{ nullptr };

#endif

		std::unordered_map<std::string, ILogicModule *> _manager;
	};
}


#endif // __TINY_TOOLKIT__LOGIC__MANAGER__H__
