@echo off
REM 统一模块打包脚本 - Windows平台
REM 可以打包包含CMOD和CJMOD的完整模块

setlocal enabledelayedexpansion

REM 颜色定义
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

REM 显示使用说明
if "%1"=="" goto :usage
if "%1"=="-h" goto :usage
if "%1"=="--help" goto :usage
if "%1"=="/?" goto :usage

REM 解析命令行参数
set "MODULE_PATH=%1"
set "OUTPUT_DIR=."
set "MODULE_NAME="
set "MODULE_VERSION="
set "INCLUDE_SOURCE=true"
set "MINIFY=true"
set "SIGN_MODULE=false"

shift
:parse_args
if "%1"=="" goto :end_parse
if "%1"=="-o" (
    set "OUTPUT_DIR=%2"
    shift
    shift
    goto :parse_args
)
if "%1"=="--output" (
    set "OUTPUT_DIR=%2"
    shift
    shift
    goto :parse_args
)
if "%1"=="-n" (
    set "MODULE_NAME=%2"
    shift
    shift
    goto :parse_args
)
if "%1"=="--name" (
    set "MODULE_NAME=%2"
    shift
    shift
    goto :parse_args
)
if "%1"=="-v" (
    set "MODULE_VERSION=%2"
    shift
    shift
    goto :parse_args
)
if "%1"=="--version" (
    set "MODULE_VERSION=%2"
    shift
    shift
    goto :parse_args
)
if "%1"=="--no-source" (
    set "INCLUDE_SOURCE=false"
    shift
    goto :parse_args
)
if "%1"=="--no-minify" (
    set "MINIFY=false"
    shift
    goto :parse_args
)
if "%1"=="--sign" (
    set "SIGN_MODULE=true"
    shift
    goto :parse_args
)
call :error "未知参数: %1"
exit /b 1

:end_parse

REM 验证参数
if not exist "%MODULE_PATH%" (
    call :error "模块路径不存在: %MODULE_PATH%"
    exit /b 1
)

REM 获取模块名
if "%MODULE_NAME%"=="" (
    for %%F in ("%MODULE_PATH%") do set "MODULE_NAME=%%~nxF"
)

if "%MODULE_VERSION%"=="" (
    set "MODULE_VERSION=1.0.0"
)

echo ===================================
echo 统一模块打包工具 - Windows平台
echo ===================================
echo.

call :info "模块: %MODULE_NAME% v%MODULE_VERSION%"

REM 检查模块结构
call :info "检查模块结构..."

set "HAS_CMOD=false"
set "HAS_CJMOD=false"

REM 检查CMOD
if exist "%MODULE_PATH%\CMOD" (
    set "HAS_CMOD=true"
    set "CMOD_DIR=CMOD"
    call :info "发现CMOD模块"
) else if exist "%MODULE_PATH%\cmod" (
    set "HAS_CMOD=true"
    set "CMOD_DIR=cmod"
    call :info "发现CMOD模块"
)

REM 检查CJMOD
if exist "%MODULE_PATH%\CJMOD" (
    set "HAS_CJMOD=true"
    set "CJMOD_DIR=CJMOD"
    call :info "发现CJMOD模块"
) else if exist "%MODULE_PATH%\cjmod" (
    set "HAS_CJMOD=true"
    set "CJMOD_DIR=cjmod"
    call :info "发现CJMOD模块"
)

if "%HAS_CMOD%"=="false" if "%HAS_CJMOD%"=="false" (
    call :error "没有找到CMOD或CJMOD目录"
    exit /b 1
)

REM 打包CMOD
if "%HAS_CMOD%"=="true" (
    call :info "打包CMOD部分..."
    
    REM 使用CMOD专用打包脚本
    set "SCRIPT_DIR=%~dp0"
    if exist "!SCRIPT_DIR!package-cmod.bat" (
        set "CMOD_ARGS=-o "%OUTPUT_DIR%" -n "%MODULE_NAME%-cmod" -v "%MODULE_VERSION%""
        if "%MINIFY%"=="false" set "CMOD_ARGS=!CMOD_ARGS! --no-minify"
        
        call "!SCRIPT_DIR!package-cmod.bat" "%MODULE_PATH%\%CMOD_DIR%" !CMOD_ARGS!
    ) else (
        call :warning "找不到package-cmod.bat脚本，跳过CMOD打包"
    )
)

REM 打包CJMOD
if "%HAS_CJMOD%"=="true" (
    call :info "打包CJMOD部分..."
    
    REM 使用CJMOD专用打包脚本
    set "SCRIPT_DIR=%~dp0"
    if exist "!SCRIPT_DIR!package-cjmod.bat" (
        set "CJMOD_ARGS=-o "%OUTPUT_DIR%" -n "%MODULE_NAME%-cjmod" -v "%MODULE_VERSION%""
        if "%INCLUDE_SOURCE%"=="false" set "CJMOD_ARGS=!CJMOD_ARGS! --no-source"
        if "%SIGN_MODULE%"=="true" set "CJMOD_ARGS=!CJMOD_ARGS! --sign"
        
        call "!SCRIPT_DIR!package-cjmod.bat" "%MODULE_PATH%\%CJMOD_DIR%" !CJMOD_ARGS!
    ) else (
        call :warning "找不到package-cjmod.bat脚本，跳过CJMOD打包"
    )
)

REM 创建统一包
call :info "创建统一模块包..."

REM 创建输出目录
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

REM 包文件名
set "PACKAGE_FILE=%OUTPUT_DIR%\%MODULE_NAME%-%MODULE_VERSION%-unified.zip"

REM 创建临时目录
set "TEMP_DIR=%TEMP%\unified_%RANDOM%"
mkdir "%TEMP_DIR%"

REM 复制整个模块结构
xcopy /s /e /y "%MODULE_PATH%\*" "%TEMP_DIR%\" >nul

REM 如果存在单独的CMOD和CJMOD包，也包含它们
if exist "%OUTPUT_DIR%\%MODULE_NAME%-cmod-%MODULE_VERSION%.cmod" (
    mkdir "%TEMP_DIR%\packages" 2>nul
    copy "%OUTPUT_DIR%\%MODULE_NAME%-cmod-%MODULE_VERSION%.cmod" "%TEMP_DIR%\packages\" >nul
)

if exist "%OUTPUT_DIR%\%MODULE_NAME%-cjmod-%MODULE_VERSION%.cjmod" (
    mkdir "%TEMP_DIR%\packages" 2>nul
    copy "%OUTPUT_DIR%\%MODULE_NAME%-cjmod-%MODULE_VERSION%.cjmod" "%TEMP_DIR%\packages\" >nul
)

REM 创建README
(
echo # %MODULE_NAME% 模块
echo.
echo 版本: %MODULE_VERSION%
echo.
echo ## 包含内容
echo.
) > "%TEMP_DIR%\README.md"

if "%HAS_CMOD%"=="true" (
    echo - CMOD: 提供CHTL模板和样式>> "%TEMP_DIR%\README.md"
)

if "%HAS_CJMOD%"=="true" (
    echo - CJMOD: 提供CHTL JS扩展功能>> "%TEMP_DIR%\README.md"
)

(
echo.
echo ## 安装方法
echo.
echo ### 方法1：使用统一包
echo 将整个目录复制到项目的module文件夹中。
echo.
echo ### 方法2：分别安装
) >> "%TEMP_DIR%\README.md"

if "%HAS_CMOD%"=="true" (
    echo - CMOD: `cjmod install packages/%MODULE_NAME%-cmod-%MODULE_VERSION%.cmod`>> "%TEMP_DIR%\README.md"
)

if "%HAS_CJMOD%"=="true" (
    echo - CJMOD: `cjmod install packages/%MODULE_NAME%-cjmod-%MODULE_VERSION%.cjmod`>> "%TEMP_DIR%\README.md"
)

REM 创建元信息
mkdir "%TEMP_DIR%\META-INF"
(
echo Manifest-Version: 1.0
echo Module-Type: Unified
echo Module-Name: %MODULE_NAME%
echo Module-Version: %MODULE_VERSION%
echo Has-CMOD: %HAS_CMOD%
echo Has-CJMOD: %HAS_CJMOD%
echo Created-By: Unified Package Tool
echo Build-Time: %date% %time%
) > "%TEMP_DIR%\META-INF\MANIFEST.MF"

REM 创建ZIP包
pushd "%TEMP_DIR%"
powershell -Command "Compress-Archive -Path '*' -DestinationPath '%PACKAGE_FILE%' -Force"
popd

REM 清理临时目录
rmdir /s /q "%TEMP_DIR%"

call :info "统一包创建成功: %PACKAGE_FILE%"

REM 显示包信息
call :info "打包结果:"
echo.

if "%HAS_CMOD%"=="true" if exist "%OUTPUT_DIR%\%MODULE_NAME%-cmod-%MODULE_VERSION%.cmod" (
    echo   CMOD包: %MODULE_NAME%-cmod-%MODULE_VERSION%.cmod
    for %%F in ("%OUTPUT_DIR%\%MODULE_NAME%-cmod-%MODULE_VERSION%.cmod") do echo     大小: %%~zF 字节
)

if "%HAS_CJMOD%"=="true" if exist "%OUTPUT_DIR%\%MODULE_NAME%-cjmod-%MODULE_VERSION%.cjmod" (
    echo   CJMOD包: %MODULE_NAME%-cjmod-%MODULE_VERSION%.cjmod
    for %%F in ("%OUTPUT_DIR%\%MODULE_NAME%-cjmod-%MODULE_VERSION%.cjmod") do echo     大小: %%~zF 字节
)

if exist "%PACKAGE_FILE%" (
    echo   统一包: %MODULE_NAME%-%MODULE_VERSION%-unified.zip
    for %%F in ("%PACKAGE_FILE%") do echo     大小: %%~zF 字节
)

echo.
call :info "打包完成！"

endlocal
exit /b 0

:usage
echo 用法: %~nx0 ^<module-path^> [options]
echo.
echo 选项:
echo   -o, --output ^<dir^>      输出目录 (默认: .)
echo   -n, --name ^<name^>       模块名称 (默认: 从模块路径推断)
echo   -v, --version ^<ver^>     模块版本 (默认: 1.0.0)
echo   --no-source             不包含源代码
echo   --no-minify             不压缩CHTL文件
echo   --sign                  对模块进行签名
echo   -h, --help, /?          显示帮助信息
echo.
echo 示例:
echo   %~nx0 src\main\java\com\chtl\module\Chtholly -o dist\
echo   %~nx0 mymodule --name MyModule --version 2.0.0
exit /b 0