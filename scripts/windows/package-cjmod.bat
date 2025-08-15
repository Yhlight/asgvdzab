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

REM 获取模块名（从路径推断）
for %%F in ("%MODULE_PATH%") do set "PARENT_DIR=%%~dpF"
for %%F in ("%PARENT_DIR:~0,-1%") do set "PARENT_NAME=%%~nxF"
for %%F in ("%PARENT_DIR:~0,-1%") do set "GRANDPARENT_DIR=%%~dpF"
for %%F in ("%GRANDPARENT_DIR:~0,-1%") do set "GRANDPARENT_NAME=%%~nxF"

if /i "%PARENT_NAME%"=="CJMOD" set "INFERRED_NAME=%GRANDPARENT_NAME%"
if /i "%PARENT_NAME%"=="cjmod" set "INFERRED_NAME=%GRANDPARENT_NAME%"
if not defined INFERRED_NAME (
    for %%F in ("%MODULE_PATH%") do set "INFERRED_NAME=%%~nxF"
)

echo ===================================
echo CJMOD模块打包工具 - Windows平台
echo ===================================
echo.

REM 检查CJMOD结构
call :info "检查CJMOD模块结构..."

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

REM 检查是否包含[Info]块
findstr /b "[Info]" "%INFO_FILE%" >nul 2>&1
if errorlevel 1 (
    call :warning "info文件缺少[Info]块"
)

REM 确保CJMOD不使用[Export]块
findstr /b "[Export]" "%INFO_FILE%" >nul 2>&1
if not errorlevel 1 (
    call :warning "CJMOD不应该使用[Export]块，建议移除"
)

REM 检查是否有Java源文件
dir /b /s "%MODULE_PATH%\src\*.java" >nul 2>&1
if errorlevel 1 (
    call :error "没有找到Java源文件"
    exit /b 1
)

call :info "CJMOD结构检查通过"

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

REM 编译Java源代码
call :info "编译Java源代码..."

REM 创建build目录
set "BUILD_DIR=%MODULE_PATH%\build"
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

REM 构建类路径
set "CLASSPATH="

REM 添加CHTL编译器到类路径
if exist "target\chtl-compiler.jar" (
    set "CLASSPATH=target\chtl-compiler.jar"
) else if exist "C:\Program Files\CHTL\lib\chtl-compiler.jar" (
    set "CLASSPATH=C:\Program Files\CHTL\lib\chtl-compiler.jar"
) else (
    REM 尝试查找最近的编译器JAR
    for /f "delims=" %%i in ('dir /s /b chtl-compiler*.jar 2^>nul ^| findstr /v build') do (
        set "CLASSPATH=%%i"
        goto :found_jar
    )
    call :error "找不到CHTL编译器JAR文件"
    exit /b 1
)
:found_jar

REM 添加lib目录（如果存在）
if exist "%MODULE_PATH%\lib" (
    set "CLASSPATH=%CLASSPATH%;%MODULE_PATH%\lib\*"
)

REM 编译Java文件
dir /s /b "%MODULE_PATH%\src\*.java" > java_files.txt
javac -cp "%CLASSPATH%" -d "%BUILD_DIR%" @java_files.txt
if errorlevel 1 (
    del java_files.txt
    call :error "编译失败"
    exit /b 1
)
del java_files.txt

call :info "编译成功"

REM 创建CJMOD包
call :info "创建CJMOD包..."

REM 创建输出目录
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

REM 包文件名
set "PACKAGE_FILE=%OUTPUT_DIR%\%MODULE_NAME%-%MODULE_VERSION%.cjmod"

REM 创建临时目录
set "TEMP_DIR=%TEMP%\cjmod_%RANDOM%"
mkdir "%TEMP_DIR%"

REM 复制info文件
mkdir "%TEMP_DIR%\info"
xcopy /s /e /y "%MODULE_PATH%\info\*" "%TEMP_DIR%\info\" >nul

REM 复制编译后的类文件
if exist "%BUILD_DIR%" (
    mkdir "%TEMP_DIR%\classes"
    xcopy /s /e /y "%BUILD_DIR%\*" "%TEMP_DIR%\classes\" >nul
)

REM 复制依赖库（如果存在）
if exist "%MODULE_PATH%\lib" (
    mkdir "%TEMP_DIR%\lib"
    xcopy /s /e /y "%MODULE_PATH%\lib\*" "%TEMP_DIR%\lib\" >nul
)

REM 复制资源文件（如果存在）
if exist "%MODULE_PATH%\resources" (
    mkdir "%TEMP_DIR%\resources"
    xcopy /s /e /y "%MODULE_PATH%\resources\*" "%TEMP_DIR%\resources\" >nul
)

REM 复制源代码（如果需要）
if "%INCLUDE_SOURCE%"=="true" if exist "%MODULE_PATH%\src" (
    mkdir "%TEMP_DIR%\src"
    xcopy /s /e /y "%MODULE_PATH%\src\*" "%TEMP_DIR%\src\" >nul
)

REM 处理子模块
call :process_submodules "%TEMP_DIR%"

REM 创建元信息
mkdir "%TEMP_DIR%\META-INF"
(
echo Manifest-Version: 1.0
echo Module-Type: CJMOD
echo Module-Name: %MODULE_NAME%
echo Module-Version: %MODULE_VERSION%
echo Created-By: CJMOD Package Tool
echo Build-Time: %date% %time%
echo Include-Source: %INCLUDE_SOURCE%
) > "%TEMP_DIR%\META-INF\MANIFEST.MF"

REM 计算校验和
call :info "计算校验和..."
REM Windows下简化处理
set /a CHECKSUM=0
for /r "%TEMP_DIR%" %%f in (*) do set /a CHECKSUM+=1
echo %CHECKSUM% > "%TEMP_DIR%\META-INF\CHECKSUM.SHA256"

REM 创建JAR包（CJMOD使用JAR格式）
pushd "%TEMP_DIR%"
jar cf "%PACKAGE_FILE%" *
popd

REM 清理临时目录
rmdir /s /q "%TEMP_DIR%"

call :info "CJMOD包创建成功: %PACKAGE_FILE%"

REM 签名模块
if "%SIGN_MODULE%"=="true" (
    call :info "签名模块..."
    where cjmod >nul 2>&1
    if not errorlevel 1 (
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
jar tf "%PACKAGE_FILE%" | find /v "META-INF" | find /v ".class" | head -20
REM 统计文件数量
for /f %%a in ('jar tf "%PACKAGE_FILE%" ^| find /c /v ""') do set TOTAL_FILES=%%a
if %TOTAL_FILES% gtr 20 (
    set /a MORE_FILES=%TOTAL_FILES%-20
    echo   ... 还有 !MORE_FILES! 个文件
)

echo.
call :info "打包完成！"

endlocal
exit /b 0

:process_submodules
REM 处理子模块
set "base_dir=%~1"

if exist "%MODULE_PATH%\src" (
    for /d %%D in ("%MODULE_PATH%\src\*") do (
        if exist "%%D\src" if exist "%%D\info" (
            for %%F in ("%%D") do set "submodule_name=%%~nxF"
            call :info "发现子模块: !submodule_name!"
            
            REM 编译子模块
            if exist "%%D\src" (
                set "sub_build_dir=%%D\build"
                mkdir "!sub_build_dir!" 2>nul
                dir /s /b "%%D\src\*.java" > java_files.txt
                javac -cp "%CLASSPATH%" -d "!sub_build_dir!" @java_files.txt
                del java_files.txt
                
                REM 复制子模块编译结果
                mkdir "%base_dir%\submodules\!submodule_name!\classes" 2>nul
                xcopy /s /e /y "!sub_build_dir!\*" "%base_dir%\submodules\!submodule_name!\classes\" >nul
            )
            
            REM 复制子模块info
            mkdir "%base_dir%\submodules\!submodule_name!\info" 2>nul
            xcopy /s /e /y "%%D\info\*" "%base_dir%\submodules\!submodule_name!\info\" >nul
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
echo   --no-source             不包含源代码
echo   --sign                  对模块进行签名
echo   -h, --help, /?          显示帮助信息
echo.
echo 示例:
echo   %~nx0 src\main\java\com\chtl\module\Chtholly\CJMOD -o dist\
echo   %~nx0 mymodule\CJMOD --sign
exit /b 0