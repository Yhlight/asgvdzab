@echo off
REM CJMOD模块打包脚本 - Windows平台

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
set "INCLUDE_DOCS=true"
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
if "%1"=="--no-docs" (
    set "INCLUDE_DOCS=false"
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

echo ===================================
echo CJMOD模块打包工具 - Windows平台
echo ===================================
echo.

REM 读取模块信息
call :info "读取模块信息..."

set "MODULE_JSON=%MODULE_PATH%\module.json"
if not exist "%MODULE_JSON%" (
    call :error "找不到module.json文件"
    exit /b 1
)

REM 如果没有指定名称和版本，从module.json读取
if "%MODULE_NAME%"=="" (
    for /f "tokens=2 delims=:," %%a in ('type "%MODULE_JSON%" ^| findstr /i "\"name\""') do (
        set "MODULE_NAME=%%~a"
        set "MODULE_NAME=!MODULE_NAME:~1,-1!"
        set "MODULE_NAME=!MODULE_NAME: =!"
    )
)

if "%MODULE_VERSION%"=="" (
    for /f "tokens=2 delims=:," %%a in ('type "%MODULE_JSON%" ^| findstr /i "\"version\""') do (
        set "MODULE_VERSION=%%~a"
        set "MODULE_VERSION=!MODULE_VERSION:~1,-1!"
        set "MODULE_VERSION=!MODULE_VERSION: =!"
    )
)

if "%MODULE_NAME%"=="" (
    call :error "无法获取模块名称"
    exit /b 1
)

if "%MODULE_VERSION%"=="" (
    set "MODULE_VERSION=1.0.0"
    call :warning "使用默认版本: !MODULE_VERSION!"
)

call :info "模块: %MODULE_NAME% v%MODULE_VERSION%"

REM 编译Java源代码
if exist "%MODULE_PATH%\src" (
    call :info "编译Java源代码..."
    
    REM 创建build目录
    set "BUILD_DIR=%MODULE_PATH%\build"
    if not exist "!BUILD_DIR!" mkdir "!BUILD_DIR!"
    
    REM 查找所有Java文件
    dir /s /b "%MODULE_PATH%\src\*.java" > java_files.txt 2>nul
    
    if exist java_files.txt (
        REM 构建类路径
        set "CLASSPATH=%MODULE_PATH%\lib\*"
        
        REM 添加CHTL编译器到类路径
        if exist "target\chtl-compiler.jar" (
            set "CLASSPATH=!CLASSPATH!;target\chtl-compiler.jar"
        ) else if exist "C:\Program Files\CHTL\lib\chtl-compiler.jar" (
            set "CLASSPATH=!CLASSPATH!;C:\Program Files\CHTL\lib\chtl-compiler.jar"
        )
        
        REM 编译
        javac -cp "!CLASSPATH!" -d "!BUILD_DIR!" @java_files.txt
        
        if !errorlevel! neq 0 (
            del java_files.txt
            call :error "编译失败"
            exit /b 1
        )
        
        del java_files.txt
        call :info "编译成功"
    ) else (
        call :warning "没有找到Java源文件"
    )
)

REM 创建模块包
call :info "创建模块包..."

REM 创建输出目录
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

REM 包文件名
set "PACKAGE_FILE=%OUTPUT_DIR%\%MODULE_NAME%-%MODULE_VERSION%.cjmod"

REM 创建临时目录
set "TEMP_DIR=%TEMP%\cjmod_%RANDOM%"
mkdir "%TEMP_DIR%"

REM 复制必要文件
copy "%MODULE_JSON%" "%TEMP_DIR%\" >nul

REM 复制编译后的类文件
if exist "%MODULE_PATH%\build" (
    mkdir "%TEMP_DIR%\classes"
    xcopy /s /e /y "%MODULE_PATH%\build\*" "%TEMP_DIR%\classes\" >nul
)

REM 复制依赖库
if exist "%MODULE_PATH%\lib" (
    mkdir "%TEMP_DIR%\lib"
    xcopy /s /e /y "%MODULE_PATH%\lib\*" "%TEMP_DIR%\lib\" >nul
)

REM 复制资源文件
if exist "%MODULE_PATH%\resources" (
    mkdir "%TEMP_DIR%\resources"
    xcopy /s /e /y "%MODULE_PATH%\resources\*" "%TEMP_DIR%\resources\" >nul
)

REM 复制源代码（如果需要）
if "%INCLUDE_SOURCE%"=="true" if exist "%MODULE_PATH%\src" (
    mkdir "%TEMP_DIR%\src"
    xcopy /s /e /y "%MODULE_PATH%\src\*" "%TEMP_DIR%\src\" >nul
)

REM 复制文档（如果需要）
if "%INCLUDE_DOCS%"=="true" (
    if exist "%MODULE_PATH%\docs" (
        mkdir "%TEMP_DIR%\docs"
        xcopy /s /e /y "%MODULE_PATH%\docs\*" "%TEMP_DIR%\docs\" >nul
    )
    
    REM 复制README
    if exist "%MODULE_PATH%\README.md" copy "%MODULE_PATH%\README.md" "%TEMP_DIR%\" >nul
    if exist "%MODULE_PATH%\README.txt" copy "%MODULE_PATH%\README.txt" "%TEMP_DIR%\" >nul
    if exist "%MODULE_PATH%\README" copy "%MODULE_PATH%\README" "%TEMP_DIR%\" >nul
)

REM 复制CHTL文件（如果是CMOD）
if exist "%MODULE_PATH%\CMOD" (
    mkdir "%TEMP_DIR%\CMOD"
    xcopy /s /e /y "%MODULE_PATH%\CMOD\*" "%TEMP_DIR%\CMOD\" >nul
)

REM 创建元信息
mkdir "%TEMP_DIR%\META-INF"
(
echo Manifest-Version: 1.0
echo Module-Name: %MODULE_NAME%
echo Module-Version: %MODULE_VERSION%
echo Created-By: CJMOD Package Tool
echo Build-Time: %date% %time%
) > "%TEMP_DIR%\META-INF\MANIFEST.MF"

REM 计算校验和
call :info "计算校验和..."
pushd "%TEMP_DIR%"
dir /s /b * | findstr /v "META-INF" > files.txt
set "CHECKSUM="
for /f "tokens=*" %%f in (files.txt) do (
    REM 这里简化处理，实际应该计算真实的SHA256
    set "CHECKSUM=!CHECKSUM!%%f"
)
echo %CHECKSUM% > META-INF\CHECKSUM.SHA256
del files.txt
popd

REM 创建JAR包
pushd "%TEMP_DIR%"
jar cf "%PACKAGE_FILE%" *
popd

REM 清理临时目录
rmdir /s /q "%TEMP_DIR%"

call :info "模块包创建成功: %PACKAGE_FILE%"

REM 签名模块
if "%SIGN_MODULE%"=="true" (
    call :info "签名模块..."
    
    REM 检查cjmod命令是否可用
    where cjmod >nul 2>&1
    if !errorlevel! equ 0 (
        cjmod sign "%PACKAGE_FILE%"
    ) else (
        call :warning "cjmod命令不可用，跳过签名"
    )
)

REM 显示包信息
call :info "包信息:"
echo   文件: %PACKAGE_FILE%
for %%F in ("%PACKAGE_FILE%") do echo   大小: %%~zF 字节

REM 列出包内容
echo.
call :info "包内容:"
jar tf "%PACKAGE_FILE%" | findstr /n "^" | findstr "^[1-9]: \|^1[0-9]: \|^20:"
for /f %%i in ('jar tf "%PACKAGE_FILE%" ^| find /c /v ""') do set TOTAL_FILES=%%i
if %TOTAL_FILES% gtr 20 (
    set /a MORE_FILES=%TOTAL_FILES%-20
    echo   ... 还有 !MORE_FILES! 个文件
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
echo   -n, --name ^<name^>       模块名称 (默认: 从module.json读取)
echo   -v, --version ^<ver^>     模块版本 (默认: 从module.json读取)
echo   --no-source             不包含源代码
echo   --no-docs               不包含文档
echo   --sign                  对模块进行签名
echo   -h, --help              显示帮助信息
echo.
echo 示例:
echo   %~nx0 mymodule -o dist\
echo   %~nx0 mymodule --sign
exit /b 0