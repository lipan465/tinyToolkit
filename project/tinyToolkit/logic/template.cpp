/**
 *
 *  作者: hm
 *
 *  说明: 逻辑模板
 *
 */


#include "template.h"

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
	bool LogicTemplate::Create(const std::string & path, const std::string & name)
	{
		if (!tinyToolkit::Filesystem::CreateDirectories(path))
		{
			return false;
		}

		return CreateMain(path, name) && CreateModule(path, name) && CreateCmake(path, name);
	}


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
	bool LogicTemplate::CreateCmake(const std::string & path, const std::string & name)
	{
		std::string cmakePath{ };

		if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
		{
			cmakePath = path + "CMakeLists.txt";
		}
		else
		{
			cmakePath = path + TINY_TOOLKIT_FOLDER_SEP + "CMakeLists.txt";
		}

		if (tinyToolkit::Filesystem::Exists(cmakePath))
		{
			return false;
		}

		{
			std::vector<std::string> vec
			{
				"#",
				"# 项目名",
				"#",
				"SET(PROGRAM_NAME " + tinyToolkit::String::Lower(name.substr(0, 1)) + name.substr(1) + ")",
				"",
				"",
				"#",
				"# 库文件的生成目录",
				"#",
				"SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)",
				"",
				"",
				"#",
				"# 获取当前目录下源文件",
				"#",
				"TRAVERSE_CURRENT_SOURCE_FILE(SOURCE_FILES)",
				"",
				"",
				"#",
				"# 链接源文件, 生成库文件",
				"#",
				"ADD_LIBRARY(${PROGRAM_NAME} SHARED ${SOURCE_FILES})",
				"",
				"",
				"#",
				"# 链接库文件",
				"#",
				"TRAVERSE_LIBRARY_FOR_PROGRAM_NAME(tinyToolkit)"
			};

			tinyToolkit::Filesystem::WriteFile(cmakePath, vec.begin(), vec.end());
		}

		return true;
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
	bool LogicTemplate::CreateMain(const std::string & path, const std::string & name)
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
				"class Factory" + tinyToolkit::String::Upper(name.substr(0, 1)) + name.substr(1),
				"{",
				"public:",
				"\t/**",
				"\t *",
				"\t * 构造函数",
				"\t *",
				"\t * @param module 模块",
				"\t *",
				"\t */",
				"\texplicit Factory" + tinyToolkit::String::Upper(name.substr(0, 1)) + name.substr(1) + "(tinyToolkit::ILogicModule ** module)",
				"\t{",
				"\t\ttinyToolkit::ILogicModule * newModule = new " + tinyToolkit::String::Upper(name.substr(0, 1)) + name.substr(1) + ";",
				"",
				"\t\tnewModule->SetName(\"" + tinyToolkit::String::Upper(name.substr(0, 1)) + name.substr(1) + "\");",
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
				"static Factory" + tinyToolkit::String::Upper(name.substr(0, 1)) + name.substr(1) + " sFactory(&sModules);",
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
	bool LogicTemplate::CreateModule(const std::string & path, const std::string & name)
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
				"#include <tinyToolkit/tinyToolkit.h>",
				"",
				"",
				"class " + tinyToolkit::String::Upper(name.substr(0, 1)) + name.substr(1) + " : public tinyToolkit::ILogicModule",
				"{",
				"\t/**",
				"\t *",
				"\t * 析构函数",
				"\t *",
				"\t */",
				"\t~" + tinyToolkit::String::Upper(name.substr(0, 1)) + name.substr(1) + "() override = default;",
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
				"bool " + tinyToolkit::String::Upper(name.substr(0, 1)) + name.substr(1) + "::Launch()",
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
				"bool " + tinyToolkit::String::Upper(name.substr(0, 1)) + name.substr(1) + "::Release()",
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
				"bool " + tinyToolkit::String::Upper(name.substr(0, 1)) + name.substr(1) + "::Initialize()",
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