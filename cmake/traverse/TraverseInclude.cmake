#
# 包含多个头文件目录, TRAVERSE_INCLUDE(name1 name2 ...)
#
MACRO(TRAVERSE_INCLUDE)

	IF(${ARGC} LESS 1)

		MESSAGE(FATAL_ERROR "Has and is greater than one parameter")

	ENDIF()

	FOREACH(param ${ARGN})

		INCLUDE_DIRECTORIES(${param})

	ENDFOREACH()

ENDMACRO(TRAVERSE_INCLUDE)