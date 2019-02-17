/**
 *
 *  作者: hm
 *
 *  说明: 模块工厂
 *
 */


#include "factory.h"

#include "../debug/trace.h"
#include "../utilities/filesystem.h"


namespace tinyToolkit
{
	/**
	 *
	 * 创建
	 *
	 * @param path 模块路径
	 * @param name 模块名称
	 *
	 */
	bool LogicModuleFactory::Create(const std::string & path, const std::string & name)
	{
		std::string mainHeader{ };
		std::string mainSource{ };
		std::string moduleHeader{ };
		std::string moduleSource{ };

		if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
		{
			mainHeader = path + "main.h";
			mainSource = path + "main.cpp";
			moduleHeader = path + "module.h";
			moduleSource = path + "module.cpp";
		}
		else
		{
			mainHeader = path + TINY_TOOLKIT_FOLDER_SEP + "main.h";
			mainSource = path + TINY_TOOLKIT_FOLDER_SEP + "main.cpp";
			moduleHeader = path + TINY_TOOLKIT_FOLDER_SEP + "module.h";
			moduleSource = path + TINY_TOOLKIT_FOLDER_SEP + "module.cpp";
		}

		if (!tinyToolkit::Filesystem::Exists(path) ||
			!tinyToolkit::Filesystem::Exists(mainHeader) ||
			!tinyToolkit::Filesystem::Exists(mainSource) ||
			!tinyToolkit::Filesystem::Exists(moduleHeader) ||
			!tinyToolkit::Filesystem::Exists(moduleSource))
		{
			TINY_TOOLKIT_DEBUG("Module already exists")

			return false;
		}



		return true;
	}
}
