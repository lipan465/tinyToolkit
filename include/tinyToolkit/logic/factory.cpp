/**
 *
 *  作者: hm
 *
 *  说明: 模块工厂
 *
 */


#include "factory.h"

#include "../utilities/string.h"
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
	 * @return 是否创建成功
	 *
	 */
	bool LogicModuleFactory::Create(const std::string & path, const std::string & name)
	{
		if (!tinyToolkit::Filesystem::CreateDirectories(path))
		{
			return false;
		}

		return CreateMain(path, name) && CreateModule(path, name);
	}

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
	bool LogicModuleFactory::CreateMain(const std::string & path, const std::string & name)
	{
		std::string headerDir{ };
		std::string sourceDir{ };

		if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
		{
			headerDir = path + "main.h";
			sourceDir = path + "main.cpp";
		}
		else
		{
			headerDir = path + TINY_TOOLKIT_FOLDER_SEP + "main.h";
			sourceDir = path + TINY_TOOLKIT_FOLDER_SEP + "main.cpp";
		}

		if (tinyToolkit::Filesystem::Exists(headerDir) ||
			tinyToolkit::Filesystem::Exists(sourceDir))
		{
			return false;
		}

		{
			std::vector<std::string> vec
			{
				"#ifndef __" + tinyToolkit::String::Upper(name) + "__MAIN__H__",
				"#define __" + tinyToolkit::String::Upper(name) + "__MAIN__H__",
				"",
				"",
				"/**",
				" *",
				" *  作者: ",
				" *",
				" *  说明: 主函数",
				" *",
				" */",
				"",
				"",
				"#include \"module.h\"",
				"",
				"",
				"#endif // __" + tinyToolkit::String::Upper(name) + "__MAIN__H__"
			};

			tinyToolkit::Filesystem::WriteFile(headerDir, vec.begin(), vec.end());
		}

		{
			std::vector<std::string> vec
			{
				"/**",
				" *",
				" *  作者: ",
				" *",
				" *  说明: 主函数",
				" *",
				" */",
				"",
				"",
				"#include \"main.h\"",
				"",
				"",
				"static iModule * sModules = nullptr;",
				"",
				"",
				"class Factory" + name,
				"{",
				"public:",
				"\tFactory" + name + "(ILogicModule * module)",
				"\t{",
				"\t\tILogicModule * module" + name + " = new " + name + ";",
				"\t\t",
				"\t\tmodule" + name + "->SetName(name);",
				"\t\tmodule" + name + "->SetNext(module);",
				"\t\t",
				"\t\tmodule = module" + name,
				"\t}",
				"};",
				"",
				"",
				"Factory" + name + " sFactory(sModules);",
				"",
				"",

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

				"extern \"C\" __declspec(dllexport) ILogicModule * __cdecl GetModules()",
				"{",
				"\treturn sModules;",
				"}"

#else

				"extern \"C\" ILogicModule * GetModules()",
				"{",
				"\treturn sModules;",
				"}"

#endif
			};

			tinyToolkit::Filesystem::WriteFile(sourceDir, vec.begin(), vec.end());
		}

		return true;
	}

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
	bool LogicModuleFactory::CreateModule(const std::string & path, const std::string & name)
	{
		std::string headerDir{ };
		std::string sourceDir{ };

		if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
		{
			headerDir = path + "module.h";
			sourceDir = path + "module.cpp";
		}
		else
		{
			headerDir = path + TINY_TOOLKIT_FOLDER_SEP + "module.h";
			sourceDir = path + TINY_TOOLKIT_FOLDER_SEP + "module.cpp";
		}

		if (tinyToolkit::Filesystem::Exists(headerDir) ||
			tinyToolkit::Filesystem::Exists(sourceDir))
		{
			return false;
		}

		{
			std::vector<std::string> vec
			{
				"#ifndef __" + tinyToolkit::String::Upper(name) + "__MODULE__H__",
				"#define __" + tinyToolkit::String::Upper(name) + "__MODULE__H__",
				"",
				"",
				"/**",
				" *",
				" *  作者: ",
				" *",
				" *  说明: 主函数",
				" *",
				" */",
				"",
				"",
				"#include <tinyToolkit/tinyToolkit.h>",
				"",
				"",
				"class " + name + " : public ILogicModule",
				"{",
				"\t~" + name + "() override = default;",
				"",
				"\tbool Launch() override;",
				"",
				"\tbool Release() override;",
				"",
				"\tbool Initialize() override;",
				"}",
				"",
				"",
				"#endif // __" + tinyToolkit::String::Upper(name) + "__MODULE__H__"
			};

			tinyToolkit::Filesystem::WriteFile(headerDir, vec.begin(), vec.end());
		}

		{

		}

		return true;
	}
}
