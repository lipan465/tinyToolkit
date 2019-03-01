#
# cmake支持的最低版本
#
CMAKE_MINIMUM_REQUIRED(VERSION 3.8 FATAL_ERROR)


#
# 项目名
#
PROJECT(tinyToolkit VERSION 0.0.1)


#
# cmake模块
#
SET(CMAKE_MODULE_PATH	${CMAKE_SOURCE_DIR}/cmake/check
						${CMAKE_SOURCE_DIR}/cmake/traverse)


#
# 添加cmake模块
#
INCLUDE(TraverseSet)
INCLUDE(TraverseSource)
INCLUDE(TraverseLibrary)
INCLUDE(TraverseInstall)
INCLUDE(TraverseInclude)
INCLUDE(TraverseCMakeLists)


#
# 添加cmake模块
#
INCLUDE(CheckSystem)
INCLUDE(CheckPlatform)
INCLUDE(CheckCompiler)
INCLUDE(CheckBuildType)
INCLUDE(CheckGlobalType)
INCLUDE(CheckInstallPrefix)


#
# 添加头文件路径
#
TRAVERSE_INCLUDE(
		${PROJECT_SOURCE_DIR}/
		${PROJECT_SOURCE_DIR}/project)


#
# 添加下级项目
#
TRAVERSE_CURRENT_ADD_SUBDIRECTORY()
