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
				"class Factory" + name,
				"{",
				"public:",
				"\t/**",
				"\t *",
				"\t * 构造函数",
				"\t *",
				"\t * @param module 模块",
				"\t *",
				"\t */",
				"\texplicit Factory" + name + "(tinyToolkit::ILogicModule ** module)",
				"\t{",
				"\t\ttinyToolkit::ILogicModule * newModule = new " + name + ";",
				"",
				"\t\tnewModule->SetName(\"" + name + "\");",
				"\t\tnewModule->SetNext(*module);",
				"",
				"\t\t*module = newModule;",
				"\t}",
				"};",
				"",
				"",
				"/**",
				" *",
				" * 模块对象",
				" *",
				" */",
				"static tinyToolkit::ILogicModule * sModules = nullptr;",
				"",
				"",
				"/**",
				" *",
				" * 工厂对象",
				" *",
				" */",
				"static Factory" + name + " sFactory(&sModules);",
				"",
				"",

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

				"/**",
				" *",
				" * 获取模块",
				" *",
				" * @return 模块",
				" *",
				" */",
				"extern \"C\" __declspec(dllexport) tinyToolkit::ILogicModule * __cdecl GetModules()",
				"{",
				"\treturn sModules;",
				"}"

#else

				"/**",
				" *",
				" * 获取模块",
				" *",
				" * @return 模块",
				" *",
				" */",
				"extern \"C\" tinyToolkit::ILogicModule * GetModules()",
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
				" *  说明: 模块",
				" *",
				" */",
				"",
				"",
				"#include \"../include/tinyToolkit/tinyToolkit.h\"",
				"",
				"",
				"class " + name + " : public tinyToolkit::ILogicModule",
				"{",
				"\t/**",
				"\t *",
				"\t * 析构函数",
				"\t *",
				"\t */",
				"\t~" + name + "() override = default;",
				"",
				"\t/**",
				"\t *",
				"\t * 启动",
				"\t *",
				"\t * @return 是否启动成功",
				"\t *",
				"\t */",
				"\tbool Launch() override;",
				"",
				"\t/**",
				"\t *",
				"\t * 释放",
				"\t *",
				"\t * @return 是否释放成功",
				"\t *",
				"\t */",
				"\tbool Release() override;",
				"",
				"\t/**",
				"\t *",
				"\t * 初始化",
				"\t *",
				"\t * @return 是否初始化成功",
				"\t *",
				"\t */",
				"\tbool Initialize() override;",
				"};",
				"",
				"",
				"#endif // __" + tinyToolkit::String::Upper(name) + "__MODULE__H__"
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
				" *  说明: 模块",
				" *",
				" */",
				"",
				"",
				"#include \"module.h\"",
				"",
				"",
				"/**",
				" *",
				" * 启动",
				" *",
				" * @return 是否启动成功",
				" *",
				" */",
				"bool " + name + "::Launch()",
				"{",
				"\treturn true;",
				"}",
				"",
				"/**",
				" *",
				" * 释放",
				" *",
				" * @return 是否释放成功",
				" *",
				" */",
				"bool " + name + "::Release()",
				"{",
				"\treturn true;",
				"}",
				"",
				"/**",
				" *",
				" * 初始化",
				" *",
				" * @return 是否初始化成功",
				" *",
				" */",
				"bool " + name + "::Initialize()",
				"{",
				"\treturn true;",
				"}",
				"",
			};

			tinyToolkit::Filesystem::WriteFile(sourceDir, vec.begin(), vec.end());
		}

		return true;
	}
}
