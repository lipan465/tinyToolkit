#!/usr/bin/env bash


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m[macOS] Start build project\033[0m"
echo -e ""


####################################################################################################


#
# 创建目录
#
mkdir -p macOS && cd macOS


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
# 返回上级目录
#
cd ..


#
# 编译
#
cmake --build macOS --config Release -- -j 4


####################################################################################################


#
# 输出信息
#
echo -e ""
echo -e "\033[1m\033[36m[macOS] End build project\033[0m"
echo -e ""


####################################################################################################
