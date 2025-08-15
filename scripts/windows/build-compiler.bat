@echo off
REM CHTL编译器构建脚本 - Windows平台
REM 构建编译器并打包官方模块

setlocal enabledelayedexpansion

REM 颜色定义（Windows 10支持）
set "GREEN=[92m"
set "RED=[91m"
set "YELLOW=[93m"
set "NC=[0m"

REM 打印信息
:info
echo %GREEN%[INFO]%NC% %~1
goto :eof

:error
echo %RED%[ERROR]%NC% %~1 >&2
exit /b 1

:warning
echo %YELLOW%[WARN]%NC% %~1
goto :eof

REM 主流程开始
echo ===================================
echo CHTL编译器构建脚本 - Windows平台
echo ===================================
echo.

REM 切换到项目根目录
cd /d "%~dp0\..\.."

REM 检查环境
call :info "检查构建环境..."

REM 检查Java
where java >nul 2>&1
if %errorlevel% neq 0 (
    call :error "未找到Java，请安装Java 17或更高版本"
    exit /b 1
)

REM 检查Java版本
for /f "tokens=3" %%i in ('java -version 2^>^&1 ^| findstr /i "version"') do (
    set JAVA_VERSION_STR=%%i
)
set JAVA_VERSION_STR=%JAVA_VERSION_STR:"=%
for /f "delims=. tokens=1" %%i in ("%JAVA_VERSION_STR%") do (
    set JAVA_VERSION=%%i
)

if %JAVA_VERSION% lss 17 (
    call :error "需要Java 17或更高版本，当前版本：%JAVA_VERSION%"
    exit /b 1
)

REM 检查Maven
where mvn >nul 2>&1
if %errorlevel% neq 0 (
    call :error "未找到Maven，请安装Maven 3.6+"
    exit /b 1
)

call :info "环境检查通过"

REM 清理构建目录
call :info "清理构建目录..."
if exist target rmdir /s /q target
if exist build rmdir /s /q build
if exist dist rmdir /s /q dist

REM 编译项目
call :info "编译CHTL编译器..."
call mvn clean package -DskipTests
if %errorlevel% neq 0 (
    call :error "编译失败"
    exit /b 1
)
call :info "编译成功"

REM 准备模块
call :info "准备官方模块..."

REM 创建临时模块目录
set "TEMP_MODULE_DIR=build\modules"
mkdir "%TEMP_MODULE_DIR%" 2>nul

REM 复制官方模块
if exist "src\main\java\com\chtl\module" (
    xcopy /s /e /y "src\main\java\com\chtl\module\*" "%TEMP_MODULE_DIR%\" >nul
)

REM 编译Chtholly模块（如果存在）
if exist "%TEMP_MODULE_DIR%\Chtholly" (
    call :info "构建Chtholly模块..."
    
    REM 编译CJMOD部分
    if exist "%TEMP_MODULE_DIR%\Chtholly\CJMOD\src" (
        pushd "%TEMP_MODULE_DIR%\Chtholly\CJMOD"
        
        REM 创建build目录
        mkdir build 2>nul
        
        REM 编译Java文件
        dir /s /b src\*.java > java_files.txt
        javac -cp "..\..\..\..\..\target\chtl-compiler.jar" -d build @java_files.txt
        del java_files.txt
        
        REM 打包成JAR
        jar cf chtholly-cjmod.jar -C build .
        
        popd
    )
)

call :info "模块准备完成"

REM 创建发布包
call :info "创建发布包..."

REM 创建发布目录
set "DIST_DIR=dist\chtl-compiler-windows"
mkdir "%DIST_DIR%\bin" 2>nul
mkdir "%DIST_DIR%\lib" 2>nul
mkdir "%DIST_DIR%\modules" 2>nul
mkdir "%DIST_DIR%\config" 2>nul
mkdir "%DIST_DIR%\docs" 2>nul

REM 复制编译器JAR
copy target\chtl-compiler*.jar "%DIST_DIR%\lib\chtl-compiler.jar" >nul

REM 复制依赖
if exist "target\lib" (
    xcopy /y "target\lib\*.jar" "%DIST_DIR%\lib\" >nul
)

REM 复制模块
if exist "build\modules" (
    xcopy /s /e /y "build\modules\*" "%DIST_DIR%\modules\" >nul
)

REM 创建启动脚本
echo @echo off > "%DIST_DIR%\bin\chtl.bat"
echo REM CHTL编译器启动脚本 >> "%DIST_DIR%\bin\chtl.bat"
echo. >> "%DIST_DIR%\bin\chtl.bat"
echo setlocal >> "%DIST_DIR%\bin\chtl.bat"
echo set "SCRIPT_DIR=%%~dp0" >> "%DIST_DIR%\bin\chtl.bat"
echo set "BASE_DIR=%%SCRIPT_DIR%%\.." >> "%DIST_DIR%\bin\chtl.bat"
echo. >> "%DIST_DIR%\bin\chtl.bat"
echo REM 设置类路径 >> "%DIST_DIR%\bin\chtl.bat"
echo set "CLASSPATH=%%BASE_DIR%%\lib\*" >> "%DIST_DIR%\bin\chtl.bat"
echo. >> "%DIST_DIR%\bin\chtl.bat"
echo REM 设置模块路径 >> "%DIST_DIR%\bin\chtl.bat"
echo set "CHTL_MODULE_PATH=%%BASE_DIR%%\modules" >> "%DIST_DIR%\bin\chtl.bat"
echo. >> "%DIST_DIR%\bin\chtl.bat"
echo REM 运行编译器 >> "%DIST_DIR%\bin\chtl.bat"
echo java -cp "%%CLASSPATH%%" com.chtl.cli.CHTLCLI %%* >> "%DIST_DIR%\bin\chtl.bat"

REM 创建cjmod命令脚本
echo @echo off > "%DIST_DIR%\bin\cjmod.bat"
echo REM CJMOD管理工具启动脚本 >> "%DIST_DIR%\bin\cjmod.bat"
echo. >> "%DIST_DIR%\bin\cjmod.bat"
echo setlocal >> "%DIST_DIR%\bin\cjmod.bat"
echo set "SCRIPT_DIR=%%~dp0" >> "%DIST_DIR%\bin\cjmod.bat"
echo set "BASE_DIR=%%SCRIPT_DIR%%\.." >> "%DIST_DIR%\bin\cjmod.bat"
echo. >> "%DIST_DIR%\bin\cjmod.bat"
echo REM 设置类路径 >> "%DIST_DIR%\bin\cjmod.bat"
echo set "CLASSPATH=%%BASE_DIR%%\lib\*" >> "%DIST_DIR%\bin\cjmod.bat"
echo. >> "%DIST_DIR%\bin\cjmod.bat"
echo REM 设置模块路径 >> "%DIST_DIR%\bin\cjmod.bat"
echo set "CHTL_MODULE_PATH=%%BASE_DIR%%\modules" >> "%DIST_DIR%\bin\cjmod.bat"
echo. >> "%DIST_DIR%\bin\cjmod.bat"
echo REM 运行CJMOD CLI >> "%DIST_DIR%\bin\cjmod.bat"
echo java -cp "%%CLASSPATH%%" com.chtl.cli.CJmodCLI %%* >> "%DIST_DIR%\bin\cjmod.bat"

REM 复制文档
copy README.md "%DIST_DIR%\docs\" >nul
copy CHTL语法文档.md "%DIST_DIR%\docs\" >nul

REM 创建配置文件
(
echo # CHTL编译器配置文件
echo.
echo # 默认输出目录
echo output.dir=output
echo.
echo # 编译选项
echo compile.optimize=true
echo compile.sourcemap=true
echo compile.minify=false
echo.
echo # 模块路径
echo module.path=${CHTL_HOME}/modules
echo.
echo # 日志级别
echo log.level=INFO
) > "%DIST_DIR%\config\chtl.conf"

call :info "发布包创建成功"

REM 打包发布文件
call :info "打包发布文件..."

cd dist

REM 创建ZIP包
powershell -Command "Compress-Archive -Path 'chtl-compiler-windows' -DestinationPath 'chtl-compiler-windows.zip' -Force"

REM 创建安装脚本
(
echo @echo off
echo REM CHTL编译器安装脚本
echo.
echo set INSTALL_DIR=C:\Program Files\CHTL
echo.
echo echo 安装CHTL编译器到 %%INSTALL_DIR%% ...
echo.
echo REM 检查管理员权限
echo net session ^>nul 2^>^&1
echo if %%errorlevel%% neq 0 ^(
echo     echo 请以管理员身份运行此脚本
echo     pause
echo     exit /b 1
echo ^)
echo.
echo REM 解压文件
echo echo 解压文件...
echo powershell -Command "Expand-Archive -Path 'chtl-compiler-windows.zip' -DestinationPath '.' -Force"
echo.
echo REM 创建安装目录
echo if not exist "%%INSTALL_DIR%%" mkdir "%%INSTALL_DIR%%"
echo.
echo REM 复制文件
echo xcopy /s /e /y "chtl-compiler-windows\*" "%%INSTALL_DIR%%\" ^>nul
echo.
echo REM 添加到PATH
echo echo 添加到系统PATH...
echo setx /M PATH "%%PATH%%;%%INSTALL_DIR%%\bin"
echo.
echo echo 安装完成！
echo echo 请重新打开命令提示符，然后运行 'chtl --version' 验证安装
echo pause
) > install.bat

cd ..

call :info "打包完成：dist\chtl-compiler-windows.zip"

REM 显示构建信息
call :info "构建信息："
echo   平台：Windows
echo   Java版本：%JAVA_VERSION%
echo   构建时间：%date% %time%

if exist "dist\chtl-compiler-windows.zip" (
    for %%F in ("dist\chtl-compiler-windows.zip") do echo   包大小：%%~zF 字节
)

echo.
call :info "构建完成！"
echo.
echo 使用方法：
echo   1. 解压：解压 dist\chtl-compiler-windows.zip
echo   2. 安装：以管理员身份运行 dist\install.bat
echo   3. 使用：chtl input.chtl -o output\

endlocal
exit /b 0