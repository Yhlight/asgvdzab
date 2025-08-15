@echo off
REM CMOD模块打包脚本 - Windows平台

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
set "MINIFY=true"

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
if "%1"=="--no-minify" (
    set "MINIFY=false"
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

REM 获取模块名（从路径推断）
for %%F in ("%MODULE_PATH%") do set "PARENT_DIR=%%~dpF"
for %%F in ("%PARENT_DIR:~0,-1%") do set "PARENT_NAME=%%~nxF"
for %%F in ("%PARENT_DIR:~0,-1%") do set "GRANDPARENT_DIR=%%~dpF"
for %%F in ("%GRANDPARENT_DIR:~0,-1%") do set "GRANDPARENT_NAME=%%~nxF"

if /i "%PARENT_NAME%"=="CMOD" set "INFERRED_NAME=%GRANDPARENT_NAME%"
if /i "%PARENT_NAME%"=="cmod" set "INFERRED_NAME=%GRANDPARENT_NAME%"
if not defined INFERRED_NAME (
    for %%F in ("%MODULE_PATH%") do set "INFERRED_NAME=%%~nxF"
)

echo ===================================
echo CMOD模块打包工具 - Windows平台
echo ===================================
echo.

REM 检查CMOD结构
call :info "检查CMOD模块结构..."

if not exist "%MODULE_PATH%\src" (
    call :error "缺少src目录"
    exit /b 1
)

if not exist "%MODULE_PATH%\info" (
    call :error "缺少info目录"
    exit /b 1
)

REM 检查info文件
set "INFO_FILE=%MODULE_PATH%\info\%INFERRED_NAME%.chtl"
if not exist "%INFO_FILE%" (
    call :error "缺少info文件: %INFERRED_NAME%.chtl"
    exit /b 1
)

call :info "CMOD结构检查通过"

REM 读取模块信息
call :info "读取模块信息..."

REM 如果没有指定名称和版本，从info文件读取
if "%MODULE_NAME%"=="" (
    for /f "tokens=2 delims==" %%a in ('findstr /i "name" "%INFO_FILE%" ^| findstr "="') do (
        set "TEMP_NAME=%%a"
        set "TEMP_NAME=!TEMP_NAME:~1!"
        for /f "delims=;" %%b in ("!TEMP_NAME!") do (
            set "MODULE_NAME=%%~b"
            set "MODULE_NAME=!MODULE_NAME:~0,-1!"
        )
    )
)

if "%MODULE_VERSION%"=="" (
    for /f "tokens=2 delims==" %%a in ('findstr /i "version" "%INFO_FILE%" ^| findstr "="') do (
        set "TEMP_VER=%%a"
        set "TEMP_VER=!TEMP_VER:~1!"
        for /f "delims=;" %%b in ("!TEMP_VER!") do (
            set "MODULE_VERSION=%%~b"
            set "MODULE_VERSION=!MODULE_VERSION:~0,-1!"
        )
    )
)

if "%MODULE_NAME%"=="" (
    set "MODULE_NAME=%INFERRED_NAME%"
    call :warning "使用推断的模块名称: %MODULE_NAME%"
)

if "%MODULE_VERSION%"=="" (
    set "MODULE_VERSION=1.0.0"
    call :warning "使用默认版本: %MODULE_VERSION%"
)

call :info "模块: %MODULE_NAME% v%MODULE_VERSION%"

REM 创建CMOD包
call :info "创建CMOD包..."

REM 创建输出目录
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

REM 包文件名
set "PACKAGE_FILE=%OUTPUT_DIR%\%MODULE_NAME%-%MODULE_VERSION%.cmod"

REM 创建临时目录
set "TEMP_DIR=%TEMP%\cmod_%RANDOM%"
mkdir "%TEMP_DIR%"

REM 复制CMOD结构
xcopy /s /e /y "%MODULE_PATH%\*" "%TEMP_DIR%\" >nul

REM 处理子模块
call :process_submodules "%TEMP_DIR%"

REM 压缩CHTL文件（如果需要）
if "%MINIFY%"=="true" (
    call :info "压缩CHTL文件..."
    REM Windows下的简单压缩处理
    for /r "%TEMP_DIR%" %%f in (*.chtl) do (
        REM 这里可以调用其他工具或脚本来压缩
        REM 暂时跳过实际压缩
    )
)

REM 创建元信息
mkdir "%TEMP_DIR%\META-INF"
(
echo Manifest-Version: 1.0
echo Module-Type: CMOD
echo Module-Name: %MODULE_NAME%
echo Module-Version: %MODULE_VERSION%
echo Created-By: CMOD Package Tool
echo Build-Time: %date% %time%
) > "%TEMP_DIR%\META-INF\MANIFEST.MF"

REM 计算校验和
call :info "计算校验和..."
REM Windows下简化处理，使用文件数量作为示例
set /a CHECKSUM=0
for /r "%TEMP_DIR%" %%f in (*) do set /a CHECKSUM+=1
echo %CHECKSUM% > "%TEMP_DIR%\META-INF\CHECKSUM.SHA256"

REM 创建ZIP包（CMOD使用ZIP格式）
pushd "%TEMP_DIR%"
powershell -Command "Compress-Archive -Path '*' -DestinationPath '%PACKAGE_FILE%' -Force"
popd

REM 清理临时目录
rmdir /s /q "%TEMP_DIR%"

call :info "CMOD包创建成功: %PACKAGE_FILE%"

REM 显示包信息
call :info "包信息:"
echo   文件: %PACKAGE_FILE%
for %%F in ("%PACKAGE_FILE%") do echo   大小: %%~zF 字节

REM 列出包内容
echo.
call :info "包内容:"
powershell -Command "$zip = [System.IO.Compression.ZipFile]::OpenRead('%PACKAGE_FILE%'); $entries = $zip.Entries | Select-Object -First 20; foreach($entry in $entries) { Write-Host '  ' $entry.FullName }; $zip.Dispose()"

echo.
call :info "打包完成！"

endlocal
exit /b 0

:process_submodules
REM 处理子模块
set "base_dir=%~1"

if exist "%base_dir%\src" (
    for /d %%D in ("%base_dir%\src\*") do (
        if exist "%%D\src" if exist "%%D\info" (
            for %%F in ("%%D") do set "submodule_name=%%~nxF"
            call :info "发现子模块: !submodule_name!"
            
            REM 验证子模块结构
            if not exist "%%D\info\!submodule_name!.chtl" (
                call :warning "子模块缺少info文件: !submodule_name!.chtl"
            )
        )
    )
)
goto :eof

:usage
echo 用法: %~nx0 ^<module-path^> [options]
echo.
echo 选项:
echo   -o, --output ^<dir^>      输出目录 (默认: .)
echo   -n, --name ^<name^>       模块名称 (默认: 从info文件读取)
echo   -v, --version ^<ver^>     模块版本 (默认: 从info文件读取)
echo   --no-minify             不压缩CHTL文件
echo   -h, --help, /?          显示帮助信息
echo.
echo 示例:
echo   %~nx0 src\main\java\com\chtl\module\Chtholly\CMOD -o dist\
echo   %~nx0 mymodule\CMOD --name MyModule --version 1.0.0
exit /b 0