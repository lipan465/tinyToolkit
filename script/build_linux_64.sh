#!/usr/bin/env bash


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m[linux_64] Start build project\033[0m"
echo -e ""


####################################################################################################


#
# 创建目录
#
mkdir -p linux_64 && cd linux_64


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
		-DCMAKE_CXX_COMPILER=g++8.1.0


#
# 返回上级目录
#
cd ..


#
# 编译
#
cmake --build linux_64 --config Release -- -j 4


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m[linux_64] End build project\033[0m"
echo -e ""


####################################################################################################
