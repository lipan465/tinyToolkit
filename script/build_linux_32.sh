#!/usr/bin/env bash


####################################################################################################


#
# 绝对路径
#
RelativeDirectory=$(cd `dirname $0`; pwd)


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m[linux_32] Start build project\033[0m"
echo -e ""


####################################################################################################


#
# 进入脚本目录
#
cd ${RelativeDirectory}


#
# 拉取fmt
#
git clone https://github.com/fmtlib/fmt.git fmt_linux_32


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m---------- fmt ----------\033[0m"
echo -e ""


####################################################################################################


#
# 进入脚本目录
#
cd ${RelativeDirectory}


#
# 创建并进入目录
#
mkdir -p fmt_linux_32/build && cd fmt_linux_32/build


#
# 生成make
#
cmake	.. 	\
		\
		-G "Unix Makefiles" \
		\
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_VERBOSE_MAKEFILE=ON \
		\
		-DCMAKE_C_COMPILER=gcc8.1.0 \
		-DCMAKE_CXX_COMPILER=g++8.1.0 \
		\
		-DCMAKE_C_FLAGS="-m32 -fPIC" \
		-DCMAKE_CXX_FLAGS="-m32 -fPIC" \
		-DCMAKE_SHARED_LINKER_FLAGS="-m32 -fPIC"


#
# 编译安装
#
make -j4 && make install


#
# 进入脚本目录
#
cd ${RelativeDirectory}


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m---------- tinyToolkit ----------\033[0m"
echo -e ""


####################################################################################################


#
# 进入脚本目录
#
cd ${RelativeDirectory}


#
# 创建并进入目录
#
mkdir -p tinyToolkit_linux_32 && cd tinyToolkit_linux_32


#
# 生成make
#
cmake	../../ \
		\
		-G "Unix Makefiles" \
		\
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_VERBOSE_MAKEFILE=ON \
		\
		-DCMAKE_C_COMPILER=gcc8.1.0 \
		-DCMAKE_CXX_COMPILER=g++8.1.0 \
		\
		-DCMAKE_C_FLAGS="-m32 -fPIC" \
		-DCMAKE_CXX_FLAGS="-m32 -fPIC" \
		-DCMAKE_SHARED_LINKER_FLAGS="-m32 -fPIC"


#
# 编译安装
#
make -j4 && make install


#
# 进入脚本目录
#
cd ${RelativeDirectory}


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m[linux_32] End build project\033[0m"
echo -e ""


####################################################################################################
