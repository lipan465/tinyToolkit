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
set buildDirectory=%projectDirectory%\build_windows


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


::
:: 输出信息
::
echo;
echo [windows] Start build project
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
		-DFMT_DOC=OFF ^
		-DFMT_TEST=OFF ^
		^
		-DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_VERBOSE_MAKEFILE=ON


::
:: 编译
::
cmake --build . --config release


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
		-DCMAKE_BUILD_TYPE=Release ^
		-DCMAKE_VERBOSE_MAKEFILE=ON


::
:: 复制文件
::
copy ..\fmt\Release\fmt.lib example\container
copy ..\fmt\Release\fmt.lib example\crypto
copy ..\fmt\Release\fmt.lib example\debug
copy ..\fmt\Release\fmt.lib example\id
copy ..\fmt\Release\fmt.lib example\log
copy ..\fmt\Release\fmt.lib example\net
copy ..\fmt\Release\fmt.lib example\pool
copy ..\fmt\Release\fmt.lib example\system
copy ..\fmt\Release\fmt.lib example\test
copy ..\fmt\Release\fmt.lib example\timer
copy ..\fmt\Release\fmt.lib project\tinyToolkit


::
:: 编译
::
cmake --build . --config release


::
:: 返回上级目录
::
popd


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


::
:: 输出信息
::
echo;
echo [windows] End build project
echo;


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
