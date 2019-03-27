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


#
# 根据项目名包含多个头文件目录, TRAVERSE_INCLUDE_FOR_PROGRAM_NAME(name1 name2 ...)
#
MACRO(TRAVERSE_INCLUDE_FOR_PROGRAM_NAME)

	IF(${ARGC} LESS 1)

		MESSAGE(FATAL_ERROR "Has and is greater than one parameter")

	ENDIF()

	FOREACH(param ${ARGN})

		TARGET_INCLUDE_DIRECTORIES(${PROGRAM_NAME}	PUBLIC	${param})

	ENDFOREACH()

ENDMACRO(TRAVERSE_INCLUDE_FOR_PROGRAM_NAME)