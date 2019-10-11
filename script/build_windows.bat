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
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\example\container
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\example\crypto
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\example\debug
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\example\id
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\example\log
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\example\net
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\example\pool
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\example\system
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\example\test
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\example\timer
copy %buildDirectory%\fmt\Release\fmt.lib %buildDirectory%\tinyToolkit\project\tinyToolkit


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
