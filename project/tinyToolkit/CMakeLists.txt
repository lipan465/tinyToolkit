#
# 项目名
#
SET(PROGRAM_NAME tinyToolkit)


#
# 库文件的生成目录
#
IF(WIN32)

	#
	# 设置库文件生成目录
	#
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

ELSE()

	#
	# 设置库文件生成目录
	#
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ENDIF()


#
# 遍历递归当前目录获取源文件
#
RECURSE_CURRENT_DIRECTORY_SOURCE_FILE(SOURCE_FILES)


#
# 链接源文件, 生成库文件
#
ADD_LIBRARY(${PROGRAM_NAME} SHARED ${SOURCE_FILES})


#
# 定义当前目录下源文件分组
#
IF(WIN32 OR APPLE)

	#
	# 遍历递归当前目录定义源文件分组
	#
	RECURSE_CURRENT_DIRECTORY_SOURCE_FILE_GROUP()

ENDIF()


#
# 链接库文件
#
IF(WIN32)

	#
	# fmt支持
	#
	IF(CMAKE_BUILD_TYPE STREQUAL Debug)

		#
		# 根据项目名链接多个动态库
		#
		TRAVERSE_LIBRARY_FOR_PROGRAM_NAME(${PROJECT_SOURCE_DIR}/project/library/fmt/windows/debug/fmtd.lib)

	ELSE()

		#
		# 根据项目名链接多个动态库
		#
		TRAVERSE_LIBRARY_FOR_PROGRAM_NAME(${PROJECT_SOURCE_DIR}/project/library/fmt/windows/release/fmt.lib)

	ENDIF()

ELSEIF(APPLE)

	#
	# fmt支持
	#
	IF(CMAKE_BUILD_TYPE STREQUAL Debug)

		#
		# 根据项目名链接多个动态库
		#
		TRAVERSE_LIBRARY_FOR_PROGRAM_NAME(${PROJECT_SOURCE_DIR}/project/library/fmt/apple/debug/libfmtd.a)

	ELSE()

		#
		# 根据项目名链接多个动态库
		#
		TRAVERSE_LIBRARY_FOR_PROGRAM_NAME(${PROJECT_SOURCE_DIR}/project/library/fmt/apple/release/libfmt.a)

	ENDIF()

ELSE()

	#
	# fmt支持
	#
	IF(CMAKE_BUILD_TYPE STREQUAL Debug)

		#
		# 根据项目名链接多个动态库
		#
		TRAVERSE_LIBRARY_FOR_PROGRAM_NAME(${PROJECT_SOURCE_DIR}/project/library/fmt/linux/debug/libfmtd.a)

	ELSE()

		#
		# 根据项目名链接多个动态库
		#
		TRAVERSE_LIBRARY_FOR_PROGRAM_NAME(${PROJECT_SOURCE_DIR}/project/library/fmt/linux/release/libfmt.a)

	ENDIF()

ENDIF()


#
# 安装库文件
#
INSTALL(TARGETS tinyToolkit
		LIBRARY DESTINATION ${LIBRARY_INSTALL_PREFIX}
		ARCHIVE DESTINATION ${LIBRARY_INSTALL_PREFIX})


#
# 遍历递归指定目录安装头文件
#
TRAVERSE_INSTALL_INCLUDE(id							${INCLUDE_INSTALL_PREFIX}/id)
TRAVERSE_INSTALL_INCLUDE(log						${INCLUDE_INSTALL_PREFIX}/log)
TRAVERSE_INSTALL_INCLUDE(net						${INCLUDE_INSTALL_PREFIX}/net)
TRAVERSE_INSTALL_INCLUDE(test						${INCLUDE_INSTALL_PREFIX}/test)
TRAVERSE_INSTALL_INCLUDE(pool						${INCLUDE_INSTALL_PREFIX}/pool)
TRAVERSE_INSTALL_INCLUDE(timer						${INCLUDE_INSTALL_PREFIX}/timer)
TRAVERSE_INSTALL_INCLUDE(debug						${INCLUDE_INSTALL_PREFIX}/debug)
TRAVERSE_INSTALL_INCLUDE(logic						${INCLUDE_INSTALL_PREFIX}/logic)
TRAVERSE_INSTALL_INCLUDE(common						${INCLUDE_INSTALL_PREFIX}/common)
TRAVERSE_INSTALL_INCLUDE(crypto						${INCLUDE_INSTALL_PREFIX}/crypto)
TRAVERSE_INSTALL_INCLUDE(system						${INCLUDE_INSTALL_PREFIX}/system)
TRAVERSE_INSTALL_INCLUDE(utilities					${INCLUDE_INSTALL_PREFIX}/utilities)
TRAVERSE_INSTALL_INCLUDE(3rd/fmt					${INCLUDE_INSTALL_PREFIX}/3rd/fmt)
TRAVERSE_INSTALL_INCLUDE(${CMAKE_CURRENT_LIST_DIR}	${INCLUDE_INSTALL_PREFIX})