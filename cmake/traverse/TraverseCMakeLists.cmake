#
# 遍历指定目录添加项目, TRAVERSE_DIRECTORY_ADD_PROJECT(path)
#
MACRO(TRAVERSE_DIRECTORY_ADD_PROJECT)

	#
	# 检测参数个数
	#
	IF(NOT ${ARGC} EQUAL 1)

		MESSAGE(FATAL_ERROR "There is one and only one parameter")

	ENDIF()

	MESSAGE(STATUS "----- : " ${CMAKE_CURRENT_LIST_DIR})

	#
	# 遍历指定目录获取cmake文件
	#
	FILE(GLOB_RECURSE CMAKE_FILES "${ARGV0}/CMakeLists.txt")

	#
	# 遍历cmake文件
	#
	FOREACH(source ${CMAKE_FILES})

		MESSAGE(STATUS "cmake : " ${source})

		#
		#
		#
		STRING(LENGTH ${ARGV0}/ cmake_prefix_length)

		STRING(SUBSTRING ${source} ${cmake_prefix_length} -1 cmake_relative_path)

		STRING(FIND ${cmake_relative_path} "/" cmake_relative_pos)

		IF(NOT (${cmake_relative_pos} EQUAL -1))

			STRING(FIND ${cmake_relative_path} "/CMakeLists.txt" cmake_suffix_pos)

			IF(${cmake_relative_pos} STREQUAL ${cmake_suffix_pos})

				STRING(SUBSTRING ${cmake_relative_path} 0 ${cmake_relative_pos} cmake_sub_directory)

				ADD_SUBDIRECTORY(${cmake_sub_directory})

			ENDIF()

		ENDIF()

	ENDFOREACH()

ENDMACRO(TRAVERSE_DIRECTORY_ADD_PROJECT)


#
# 遍历当前目录添加项目, TRAVERSE_CURRENT_DIRECTORY_ADD_PROJECT()
#
MACRO(TRAVERSE_CURRENT_DIRECTORY_ADD_PROJECT)

	#
	# 检测参数个数
	#
	IF(NOT ${ARGC} EQUAL 0)

		MESSAGE(FATAL_ERROR "There is not need parameter")

	ENDIF()

	#
	# 遍历指定目录添加项目
	#
	TRAVERSE_DIRECTORY_ADD_PROJECT(${CMAKE_CURRENT_LIST_DIR})

ENDMACRO(TRAVERSE_CURRENT_DIRECTORY_ADD_PROJECT)