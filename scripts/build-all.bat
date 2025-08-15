@echo off
REM CHTL项目统一构建脚本 - Windows版本

setlocal enabledelayedexpansion

REM 颜色定义
set "GREEN=[92m"
set "RED=[91m"
set "YELLOW=[93m"
set "BLUE=[94m"
set "NC=[0m"

REM 默认值
set "BUILD_TARGET=all"
set "PLATFORM=windows"
set "SKIP_TESTS=false"
set "MODULE_PATH="

REM 解析参数
:parse_args
if "%1"=="" goto :check_params
if "%1"=="all" (
    set "BUILD_TARGET=all"
    shift
    goto :parse_args
)
if "%1"=="compiler" (
    set "BUILD_TARGET=compiler"
    shift
    goto :parse_args
)
if "%1"=="vscode" (
    set "BUILD_TARGET=vscode"
    shift
    goto :parse_args
)
if "%1"=="clean" (
    set "BUILD_TARGET=clean"
    shift
    goto :parse_args
)
if "%1"=="module" (
    set "BUILD_TARGET=module"
    set "MODULE_PATH=%2"
    set "MODULE_TYPE=unified"
    shift
    shift
    REM 继续解析module特定参数
    :parse_module_type
    if "%1"=="--type" (
        set "MODULE_TYPE=%2"
        shift
        shift
    )
    goto :parse_args
)
if "%1"=="--skip-tests" (
    set "SKIP_TESTS=true"
    shift
    goto :parse_args
)
if "%1"=="-h" goto :usage
if "%1"=="--help" goto :usage
if "%1"=="/?" goto :usage

echo %RED%[ERROR]%NC% 未知参数: %1
exit /b 1

:check_params
REM 显示构建信息
echo ========================================
echo CHTL项目统一构建脚本
echo ========================================
echo 构建目标: %BUILD_TARGET%
echo 目标平台: %PLATFORM%
echo 跳过测试: %SKIP_TESTS%
echo.

REM 执行构建
if "%BUILD_TARGET%"=="all" goto :build_all
if "%BUILD_TARGET%"=="compiler" goto :build_compiler
if "%BUILD_TARGET%"=="vscode" goto :build_vscode
if "%BUILD_TARGET%"=="module" goto :build_module
if "%BUILD_TARGET%"=="clean" goto :clean_all

:build_all
echo %BLUE%=== 构建编译器 ===%NC%
call :build_compiler
echo.
echo %BLUE%=== 构建VSCode插件 ===%NC%
call :build_vscode
goto :show_results

:build_compiler
echo %GREEN%[INFO]%NC% 开始构建编译器...
call "%~dp0windows\build-compiler.bat"
if %errorlevel% neq 0 (
    echo %RED%[ERROR]%NC% 编译器构建失败
    exit /b 1
)
echo %GREEN%[INFO]%NC% 编译器构建完成
goto :eof

:build_vscode
echo %GREEN%[INFO]%NC% 开始构建VSCode插件...
call "%~dp0windows\build-vscode-extension.bat"
if %errorlevel% neq 0 (
    echo %RED%[ERROR]%NC% VSCode插件构建失败
    exit /b 1
)
echo %GREEN%[INFO]%NC% VSCode插件构建完成
goto :eof

:build_module
if "%MODULE_PATH%"=="" (
    echo %RED%[ERROR]%NC% 请指定模块路径
    exit /b 1
)

if "%MODULE_TYPE%"=="" set MODULE_TYPE=unified

echo %GREEN%[INFO]%NC% 开始打包模块: %MODULE_PATH% (类型: %MODULE_TYPE%)
cd /d "%~dp0.."

REM 根据模块类型选择相应的打包脚本
if /i "%MODULE_TYPE%"=="cmod" (
    set PACKAGE_SCRIPT=package-cmod
) else if /i "%MODULE_TYPE%"=="cjmod" (
    set PACKAGE_SCRIPT=package-cjmod
) else (
    set PACKAGE_SCRIPT=package-unified
)

call "scripts\windows\%PACKAGE_SCRIPT%.bat" "%MODULE_PATH%"
if %errorlevel% neq 0 (
    echo %RED%[ERROR]%NC% 模块打包失败
    exit /b 1
)
echo %GREEN%[INFO]%NC% 模块打包完成
goto :show_results

:clean_all
echo %GREEN%[INFO]%NC% 清理构建文件...
cd /d "%~dp0.."

REM 清理目录
if exist target rmdir /s /q target
if exist build rmdir /s /q build
if exist dist rmdir /s /q dist

REM 清理VSCode插件构建文件
if exist vscode-chtl (
    cd vscode-chtl
    if exist out rmdir /s /q out
    if exist node_modules rmdir /s /q node_modules
    if exist resources\compiler rmdir /s /q resources\compiler
    if exist resources\modules rmdir /s /q resources\modules
    del /q *.vsix 2>nul
    cd ..
)

REM 清理测试输出
if exist test-output rmdir /s /q test-output
if exist test-reports rmdir /s /q test-reports

REM 清理临时文件
del /s /q *.class 2>nul
del /s /q *.log 2>nul
del /s /q *.tmp 2>nul

echo %GREEN%[INFO]%NC% 清理完成
goto :end

:show_results
echo.
echo %BLUE%=== 构建结果 ===%NC%

cd /d "%~dp0.."

REM 检查编译器
if "%BUILD_TARGET%"=="all" goto :check_compiler
if "%BUILD_TARGET%"=="compiler" goto :check_compiler
goto :check_vscode

:check_compiler
echo 编译器包:
if exist "dist\chtl-compiler-windows.zip" (
    for %%F in ("dist\chtl-compiler-windows.zip") do echo   %%~nxF (%%~zF 字节^)
) else (
    echo   未找到
)

:check_vscode
if "%BUILD_TARGET%"=="all" goto :do_check_vscode
if "%BUILD_TARGET%"=="vscode" goto :do_check_vscode
goto :check_module

:do_check_vscode
echo.
echo VSCode插件:
if exist "dist\*.vsix" (
    for %%F in (dist\*.vsix) do echo   %%~nxF (%%~zF 字节^)
) else (
    echo   未找到
)

:check_module
if "%BUILD_TARGET%"=="module" (
    echo.
    echo 模块包:
    if exist "*.cjmod" (
        for %%F in (*.cjmod) do echo   %%~nxF (%%~zF 字节^)
    ) else if exist "dist\*.cjmod" (
        for %%F in (dist\*.cjmod) do echo   %%~nxF (%%~zF 字节^)
    ) else (
        echo   未找到
    )
)

echo.
echo %BLUE%=== 下一步 ===%NC%
echo 1. 安装编译器: 以管理员身份运行 dist\install.bat
echo 2. 安装VSCode插件: code --install-extension dist\*.vsix

:end
echo.
echo %GREEN%[INFO]%NC% 构建完成！
endlocal
exit /b 0

:usage
echo 用法: %~nx0 [选项]
echo.
echo 选项:
echo   all              构建所有组件（默认）
echo   compiler         仅构建编译器
echo   vscode           仅构建VSCode插件
echo   module ^<path^> [--type cmod^|cjmod^|unified]    打包指定模块
echo   clean            清理构建文件
echo   --skip-tests     跳过测试
echo   -h, --help, /?   显示帮助信息
echo.
echo 示例:
echo   %~nx0                    # 构建所有组件
echo   %~nx0 compiler           # 仅构建编译器
echo   %~nx0 module Chtholly    # 打包Chtholly模块
exit /b 0