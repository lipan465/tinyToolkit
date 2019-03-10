#
# 遍历添加指定目录下级项目, TRAVERSE_ADD_SUBDIRECTORY(path)
#
MACRO(TRAVERSE_ADD_SUBDIRECTORY)

	IF(NOT ${ARGC} EQUAL 1)

		MESSAGE(FATAL_ERROR "There is one and only one parameter")

	ENDIF()

	FILE(GLOB_RECURSE cmakeLists "${ARGV0}/CMakeLists.txt")

	FOREACH(cmake ${cmakeLists})

		STRING(LENGTH ${ARGV0}/ cmake_prefix_length)

		STRING(SUBSTRING ${cmake} ${cmake_prefix_length} -1 cmake_relative_path)

		STRING(FIND ${cmake_relative_path} "/" cmake_relative_pos)

		IF(NOT (${cmake_relative_pos} EQUAL -1))

			STRING(FIND ${cmake_relative_path} "/CMakeLists.txt" cmake_suffix_pos)

			IF(${cmake_relative_pos} STREQUAL ${cmake_suffix_pos})

				STRING(SUBSTRING ${cmake_relative_path} 0 ${cmake_relative_pos} cmake_sub_directory)

				ADD_SUBDIRECTORY(${cmake_sub_directory})

			ENDIF()

		ENDIF()

	ENDFOREACH()

ENDMACRO(TRAVERSE_ADD_SUBDIRECTORY)


#
# 遍历添加当前目录下级项目, TRAVERSE_CURRENT_ADD_SUBDIRECTORY()
#
MACRO(TRAVERSE_CURRENT_ADD_SUBDIRECTORY)

	TRAVERSE_ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR})

ENDMACRO(TRAVERSE_CURRENT_ADD_SUBDIRECTORY)