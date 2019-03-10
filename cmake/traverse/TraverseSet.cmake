#
# 包含多个设置, TRAVERSE_SET(project name1 name2 ...)
#
MACRO(TRAVERSE_SET)

	IF(${ARGC} LESS 2)

		MESSAGE(FATAL_ERROR "Has and is greater than two parameter")

	ENDIF()

	FOREACH(param ${ARGN})

		IF (NOT (${param} STREQUAL ${ARGV0}))

			SET(${ARGV0} "${${ARGV0}} ${param}")

		ENDIF()

	ENDFOREACH()

ENDMACRO(TRAVERSE_SET)
