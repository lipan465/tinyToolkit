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
		 * 析构函数
		 *
		 */
		~LogicModuleManager();

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
		bool _isLoad{ false };

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		std::unordered_map<std::string, HINSTANCE> _handleManager{ };

#else

		std::unordered_map<std::string, void *> _handleManager{ };

#endif

		std::unordered_map<std::string, ILogicModule *> _moduleManager{ };
	};
}


#endif // __TINY_TOOLKIT__LOGIC__MANAGER__H__
