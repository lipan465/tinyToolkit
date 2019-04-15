#!/usr/bin/env bash


####################################################################################################


#
# 脚本路径
#
shellDirectory=$(cd `dirname $0`; pwd)


#
# 项目路径
#
projectDirectory=${shellDirectory}/..


#
# 编译路径
#
buildDirectory=${projectDirectory}/build_macOS


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m[macOS] Start build project\033[0m"
echo -e ""


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m---------- fmt ----------\033[0m"
echo -e ""


####################################################################################################


#
# 创建并进入目录
#
mkdir -p ${buildDirectory}/fmt && cd ${buildDirectory}/fmt


#
# 生成make
#
cmake	${projectDirectory}/3rd/fmt \
		\
		-G "Unix Makefiles" \
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
cd ${shellDirectory}


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m---------- tinyToolkit ----------\033[0m"
echo -e ""


####################################################################################################


#
# 创建并进入目录
#
mkdir -p ${buildDirectory}/tinyToolkit && cd ${buildDirectory}/tinyToolkit


#
# 生成make
#
cmake	${projectDirectory} \
		\
		-G "Unix Makefiles" \
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
cd ${shellDirectory}


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m[macOS] End build project\033[0m"
echo -e ""


####################################################################################################
