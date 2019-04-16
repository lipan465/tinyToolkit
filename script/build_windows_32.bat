@echo off


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


::
:: 脚本路径
::
set shellDirectory=%~dp0


::
:: 项目路径
::
set projectDirectory=%shellDirectory%\..


::
:: 编译路径
::
set buildDirectory=%projectDirectory%\build_windows_32


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


::
:: 输出信息
::
echo;
echo [windows_32] Start build project
echo;


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


::
:: 输出信息
::
echo;
echo ---------- fmt ----------
echo;


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


::
:: 创建目录
::
mkdir %buildDirectory%\fmt


::
:: 进入目录
::
pushd %buildDirectory%\fmt


::
:: 生成make
::
cmake	%projectDirectory%\3rd\fmt ^
		^
		-G "Visual Studio 15 2017" ^
		^
		-DCMAKE_BUILD_TYPE=Release ^
		-DCMAKE_VERBOSE_MAKEFILE=ON ^
		^
		-DCMAKE_C_FLAGS=-fPIC ^
		-DCMAKE_CXX_FLAGS=-fPIC


::
:: 编译
::
cmake --build . --config Releas


::
:: 返回上级目录
::
popd


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


::
:: 输出信息
::
echo;
echo ---------- tinyToolkit ----------
echo;


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


::
:: 创建目录
::
mkdir %buildDirectory%\tinyToolkit


::
:: 进入目录
::
pushd %buildDirectory%\tinyToolkit


::
:: 生成make
::
cmake	%projectDirectory% ^
		^
		-G "Visual Studio 15 2017" ^
		^
		-DCMAKE_BUILD_TYPE=Release ^
		-DCMAKE_VERBOSE_MAKEFILE=ON


::
:: 编译
::
cmake --build  . --config Release


::
:: 返回上级目录
::
popd


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


::
:: 输出信息
::
echo;
echo [windows_32] End build project
echo;


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
