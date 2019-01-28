#
# 添加cmake自带的模块
#
INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCXXCompilerFlag)


#
# 设置通用编译选项
#
TRAVERSE_SET(COMPILER_GNU_FLAGS
		# 将编译时的调试信息保存到本地文件中
		-g
		# 优选代码空间
		-Os
		# gprof调试选项
		-pg
		# 支持dlopen, dlsym, dlclose, dlerror显示加载动态库
		-ldl
		# 告诉编译器产生与位置无关代码, 则产生的代码中, 没有绝对地址, 全部使用相对地址. 故而代码可以被加载器加载到内存的任意位置, 都可以正确的执行
		-fPIC
		# 通知链接器将所有符号添加到动态符号表中
		-rdynamic
		# 链接线程库
		-pthread
		# 显示所有的静态内存分配
#		-fmem-report
		# 统计编译消耗的时间并显示报告
#		-ftime-report
		# 把所有简单的函数内联进调用者. 编译器会探索式地决定哪些函数足够简单, 值得做这种内联
		-finline-functions
		# 会打开一些很有用的警告选项
		-Wall
		# 把警告当作错误
		-Werror
		# 打印一些额外的警告信息
		-Wextra
		# 当一个没有定义的符号出现在#if中时, 给出警告
		-Wundef
		# 当一个局部变量遮盖住了另一个局部变量, 或者全局变量, 给出警告
		-Wshadow
		# 范围溢出, 给出警告
		-Woverflow
		# 无论是声明为inline或者是指定了-finline-functions选项, 如果某函数不能内联, 给出警告
#		-Winline
		# 当强制转化丢掉了类型修饰符, 给出警告
		-Wcast-qual
		# 某个指针类型强制转换导致目标所需的地址对齐增加, 给出警告
		-Wcast-align
		# 出现错误的时候停止编译
		-Wfatal-errors
		# 对函数指针或者void *类型的指针进行算术操作, 给出警告
		-Wpointer-arith
		# 如果在同一个可见域内某定义多次声明, 给出警告
		-Wredundant-decls
		# 如果编译器探测到永远不会执行到的代码, 给出警告
		-Wunreachable-code
		# 如果返回临时引用, 给出警告
		-Wreturn-local-addr
		# 如果函数的声明隐藏住了基类的虚函数, 给出警告
		-Woverloaded-virtual
		# 设置了但未使用的变量, 给出警告
		-Wunused-but-set-variable
		# 无法保证完整初始化类, 给出警告
		-Wsizeof-pointer-memaccess)


#
# 设置通用编译选项
#
#
TRAVERSE_SET(COMPILER_CLANG_FLAGS
		# 将编译时的调试信息保存到本地文件中
		-g
		# 优选代码空间
		-Os
		# gprof调试选项
		-pg
		# 告诉编译器产生与位置无关代码, 则产生的代码中, 没有绝对地址, 全部使用相对地址. 故而代码可以被加载器加载到内存的任意位置, 都可以正确的执行
		-fPIC
		# 链接线程库
		-pthread
		# 显示所有的静态内存分配
#		-fmem-report
		# 统计编译消耗的时间并显示报告
#		-ftime-report
		# 把所有简单的函数内联进调用者. 编译器会探索式地决定哪些函数足够简单, 值得做这种内联
		-finline-functions
		# 会打开一些很有用的警告选项
		-Wall
		# 把警告当作错误
		-Werror
		# 打印一些额外的警告信息
		-Wextra
		# 当一个没有定义的符号出现在#if中时, 给出警告
		-Wundef
		# 当一个局部变量遮盖住了另一个局部变量, 或者全局变量, 给出警告
		-Wshadow
		# 范围溢出, 给出警告
		-Woverflow
		# 无论是声明为inline或者是指定了-finline-functions选项, 如果某函数不能内联, 给出警告
#		-Winline
		# 当强制转化丢掉了类型修饰符, 给出警告
		-Wcast-qual
		# 某个指针类型强制转换导致目标所需的地址对齐增加, 给出警告
		-Wcast-align
		# 出现错误的时候停止编译
		-Wfatal-errors
		# 对函数指针或者void *类型的指针进行算术操作, 给出警告
		-Wpointer-arith
		# 如果在同一个可见域内某定义多次声明, 给出警告
		-Wredundant-decls
		# 如果编译器探测到永远不会执行到的代码, 给出警告
		-Wunreachable-code
		# 如果函数的声明隐藏住了基类的虚函数, 给出警告
		-Woverloaded-virtual
		# 如果返回临时引用, 给出警告
		-Wreturn-stack-address
		# 无法保证完整初始化类, 给出警告
		-Wsizeof-pointer-memaccess)


#
# 设置gcc asan选项
#
TRAVERSE_SET(COMPILER_GNU_ASAN_FLAG
		-static-libasan
		-static-libubsan
		-fsanitize=leak
		-fsanitize=vptr
		-fsanitize=null
		-fsanitize=shift
		-fsanitize=bounds
		-fsanitize=return
		-fsanitize=address
		-fsanitize=alignment
		-fsanitize=undefined
		-fsanitize=vla-bound
		-fsanitize=object-size
		-fsanitize=unreachable
		-fsanitize=bounds-strict
		-fsanitize=float-cast-overflow
		-fsanitize=float-divide-by-zero
		-fsanitize=integer-divide-by-zero
		-fsanitize=signed-integer-overflow
		-fstack-protector
		-fno-strict-aliasing
		-fno-omit-frame-pointer
		-fno-delete-null-pointer-checks)


#
# 设置clang asan选项
#
TRAVERSE_SET(COMPILER_CLANG_ASAN_FLAG
		-fsanitize=address
		-fno-omit-frame-pointer)


#
# 检测C编译的特性
#
CHECK_C_COMPILER_FLAG("-std=c89" C_COMPILER_SUPPORTS_C89)
CHECK_C_COMPILER_FLAG("-std=c99" C_COMPILER_SUPPORTS_C99)
CHECK_C_COMPILER_FLAG("-std=c11" C_COMPILER_SUPPORTS_C11)

IF(C_COMPILER_SUPPORTS_C11)

	SET(CMAKE_C_FLAGS "-std=c11")

ELSEIF(C_COMPILER_SUPPORTS_C99)

	SET(CMAKE_C_FLAGS "-std=c99")

ELSEIF(C_COMPILER_SUPPORTS_C89)

	SET(CMAKE_C_FLAGS "-std=c89")

ELSE()

	MESSAGE(FATAL_ERROR "The C compiler ${CMAKE_C_COMPILER} does not support more than C89 features.")

ENDIF()


#
# 检测C++编译器支持的特性
#
CHECK_CXX_COMPILER_FLAG("-std=c++0x" CXX_COMPILER_SUPPORTS_CXX0X)
CHECK_CXX_COMPILER_FLAG("-std=c++11" CXX_COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++14" CXX_COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++17" CXX_COMPILER_SUPPORTS_CXX17)

IF(CXX_COMPILER_SUPPORTS_CXX17)

	SET(CMAKE_CXX_FLAGS "-std=c++17")

	ADD_DEFINITIONS(-DTINY_TOOLKIT_CXX_SUPPORT=17)

ELSEIF(CXX_COMPILER_SUPPORTS_CXX14)

	SET(CMAKE_CXX_FLAGS "-std=c++14")

	ADD_DEFINITIONS(-DTINY_TOOLKIT_CXX_SUPPORT=14)

ELSEIF(CXX_COMPILER_SUPPORTS_CXX11)

	SET(CMAKE_CXX_FLAGS "-std=c++11")

	ADD_DEFINITIONS(-DTINY_TOOLKIT_CXX_SUPPORT=11)

ELSEIF(CXX_COMPILER_SUPPORTS_CXX0X)

	SET(CMAKE_CXX_FLAGS "-std=c++0x")

	ADD_DEFINITIONS(-DTINY_TOOLKIT_CXX_SUPPORT=0)

ELSE()

	MESSAGE(FATAL_ERROR "The CXX compiler ${CMAKE_CXX_COMPILER} does not support more than C++11 features.")

ENDIF()


#
# 根据编译器来选择编译选项
#
IF(CMAKE_CXX_COMPILER_ID MATCHES GNU)

	#
	# 设置编译选项
	#
	TRAVERSE_SET(CMAKE_C_FLAGS   ${COMPILER_GNU_FLAGS})
	TRAVERSE_SET(CMAKE_CXX_FLAGS ${COMPILER_GNU_FLAGS})

	#
	# 谁值调试选项
	#
	IF(${CMAKE_BUILD_TYPE} MATCHES Debug)

		TRAVERSE_SET(CMAKE_C_FLAGS   ${COMPILER_GNU_ASAN_FLAG})
		TRAVERSE_SET(CMAKE_CXX_FLAGS ${COMPILER_GNU_ASAN_FLAG})

	ENDIF()

ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES Clang)

	#
	# 设置编译选项
	#
	TRAVERSE_SET(CMAKE_C_FLAGS   ${COMPILER_CLANG_FLAGS})
	TRAVERSE_SET(CMAKE_CXX_FLAGS ${COMPILER_CLANG_FLAGS})

	#
	# 谁值调试选项
	#
	IF(${CMAKE_BUILD_TYPE} MATCHES Debug)

		TRAVERSE_SET(CMAKE_C_FLAGS   ${COMPILER_CLANG_ASAN_FLAG})
		TRAVERSE_SET(CMAKE_CXX_FLAGS ${COMPILER_CLANG_ASAN_FLAG})

	ENDIF()

ENDIF()
