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
echo -e "\033[1m\033[36m[macOS] Start build project\033[0m"
echo -e ""


####################################################################################################


#
# 进入脚本目录
#
cd ${RelativeDirectory}


#
# 拉取fmt
#
git clone https://github.com/fmtlib/fmt.git fmt_macOS


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
mkdir -p fmt_macOS/build && cd fmt_macOS/build


#
# 生成make
#
cmake	.. 	\
		\
		-G "Xcode" \
		\
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_VERBOSE_MAKEFILE=ON \
		\
		-DCMAKE_C_FLAGS=-fPIC \
		-DCMAKE_CXX_FLAGS=-fPIC


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
mkdir -p tinyToolkit_macOS && cd tinyToolkit_macOS


#
# 生成make
#
cmake	../../ \
		\
		-G "Xcode" \
		\
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_VERBOSE_MAKEFILE=ON


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
echo -e "\033[1m\033[36m[macOS] End build project\033[0m"
echo -e ""


####################################################################################################
