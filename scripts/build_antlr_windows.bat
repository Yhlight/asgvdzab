@echo off
setlocal enabledelayedexpansion

REM ANTLR Windows手动编译脚本
REM 支持Visual Studio和MinGW

echo === ANTLR Windows手动编译工具 ===

set SCRIPT_DIR=%~dp0
set PROJECT_ROOT=%SCRIPT_DIR%..
set BUILD_DIR=%PROJECT_ROOT%\antlr-manual-build
set INSTALL_DIR=%PROJECT_ROOT%\antlr-manual-install

echo 构建目录: %BUILD_DIR%
echo 安装目录: %INSTALL_DIR%

REM 创建目录
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

REM 检测编译器
call :detect_compiler
if "%COMPILER%"=="" (
    echo ❌ 未找到支持的编译器 ^(Visual Studio 或 MinGW^)
    echo 请安装以下之一:
    echo - Visual Studio 2019/2022 with C++ workload
    echo - MinGW-w64
    pause
    exit /b 1
)

echo 检测到编译器: %COMPILER%

REM 检查Java
call :check_java
if "%JAVA_OK%"=="false" (
    echo ❌ 未找到Java 11+
    echo 请安装OpenJDK 11或更高版本
    pause
    exit /b 1
)

REM 检查Maven
call :check_maven
if "%MAVEN_OK%"=="false" (
    echo ❌ 未找到Maven
    echo 请安装Apache Maven
    pause
    exit /b 1
)

REM 主编译流程
call :download_antlr
call :build_antlr_tool
call :build_antlr_cpp_runtime
call :verify_build
call :generate_usage_guide

echo.
echo 🎉 ANTLR编译完成!
echo 安装目录: %INSTALL_DIR%
echo 请查看 %INSTALL_DIR%\README.md 了解使用方法
pause
goto :eof

REM ================== 子函数 ==================

:detect_compiler
set COMPILER=
REM 检测Visual Studio
if defined VS160COMNTOOLS (
    set COMPILER=VS2019
    set VS_PATH=%VS160COMNTOOLS%
    goto :eof
)
if defined VS170COMNTOOLS (
    set COMPILER=VS2022
    set VS_PATH=%VS170COMNTOOLS%
    goto :eof
)

REM 检测MinGW
where gcc >nul 2>&1
if %errorlevel%==0 (
    where mingw32-make >nul 2>&1
    if %errorlevel%==0 (
        set COMPILER=MinGW
        goto :eof
    )
)

REM 尝试查找Visual Studio 2019/2022
for /f "usebackq tokens=*" %%i in (`where /R "C:\Program Files*" devenv.exe 2^>nul`) do (
    set "VS_PATH=%%~dpi"
    set COMPILER=VS_AUTO
    goto :eof
)
goto :eof

:check_java
set JAVA_OK=false
java -version >nul 2>&1
if %errorlevel%==0 (
    for /f "tokens=3" %%i in ('java -version 2^>^&1 ^| findstr version') do (
        set JAVA_VERSION=%%i
        set JAVA_VERSION=!JAVA_VERSION:"=!
        for /f "tokens=1,2 delims=." %%a in ("!JAVA_VERSION!") do (
            if %%a geq 11 set JAVA_OK=true
            if %%a==1 if %%b geq 8 set JAVA_OK=true
        )
    )
)
if "%JAVA_OK%"=="true" echo ✓ Java检查通过
goto :eof

:check_maven
set MAVEN_OK=false
mvn -version >nul 2>&1
if %errorlevel%==0 (
    set MAVEN_OK=true
    echo ✓ Maven检查通过
)
goto :eof

:download_antlr
echo === 下载ANTLR源码 ===
cd /d "%BUILD_DIR%"

set ANTLR_VERSION=4.13.1
set ANTLR_URL=https://github.com/antlr/antlr4/archive/refs/tags/%ANTLR_VERSION%.zip

echo 下载ANTLR %ANTLR_VERSION%...

if not exist "antlr4-%ANTLR_VERSION%.zip" (
    echo 使用PowerShell下载...
    powershell -Command "Invoke-WebRequest -Uri '%ANTLR_URL%' -OutFile 'antlr4-%ANTLR_VERSION%.zip'"
    if !errorlevel! neq 0 (
        echo ❌ 下载失败，请检查网络连接
        pause
        exit /b 1
    )
)

if not exist "antlr4-%ANTLR_VERSION%" (
    echo 解压源码...
    powershell -Command "Expand-Archive -Path 'antlr4-%ANTLR_VERSION%.zip' -DestinationPath '.'"
)

echo ✓ ANTLR源码下载完成
goto :eof

:build_antlr_tool
echo === 编译ANTLR工具 ===
cd /d "%BUILD_DIR%\antlr4-%ANTLR_VERSION%"

echo 使用Maven编译ANTLR工具...
call mvn clean
call mvn -DskipTests install

REM 查找编译好的JAR文件
for /r %%f in (antlr-*-complete.jar) do (
    if exist "%%f" (
        copy "%%f" "%INSTALL_DIR%\antlr4.jar"
        echo ✓ ANTLR工具编译完成: %INSTALL_DIR%\antlr4.jar
        goto :create_scripts
    )
)

REM 如果没找到complete jar，找tool jar
for /r %%f in (antlr4-*.jar) do (
    echo %%f | findstr /v "sources javadoc" >nul
    if !errorlevel!==0 (
        copy "%%f" "%INSTALL_DIR%\antlr4.jar"
        echo ✓ ANTLR工具编译完成: %INSTALL_DIR%\antlr4.jar
        goto :create_scripts
    )
)

echo ❌ 未找到ANTLR工具JAR文件
pause
exit /b 1

:create_scripts
echo === 创建ANTLR命令脚本 ===

REM Windows批处理文件
echo @echo off > "%INSTALL_DIR%\antlr4.bat"
echo set SCRIPT_DIR=%%~dp0 >> "%INSTALL_DIR%\antlr4.bat"
echo java -jar "%%SCRIPT_DIR%%antlr4.jar" %%* >> "%INSTALL_DIR%\antlr4.bat"

echo ✓ ANTLR命令脚本创建完成
goto :eof

:build_antlr_cpp_runtime
echo === 编译ANTLR C++运行时 ===
cd /d "%BUILD_DIR%\antlr4-%ANTLR_VERSION%\runtime\Cpp"

if not exist "build" mkdir "build"
cd build

REM 根据编译器设置CMake参数
if "%COMPILER%"=="MinGW" (
    echo 使用MinGW编译...
    cmake .. -G "MinGW Makefiles" ^
        -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
        -DCMAKE_CXX_STANDARD=17 ^
        -DWITH_LIBCXX=OFF ^
        -DANTLR4_INSTALL=ON
    
    mingw32-make -j%NUMBER_OF_PROCESSORS%
    mingw32-make install
) else (
    echo 使用Visual Studio编译...
    REM 设置Visual Studio环境
    if defined VS_PATH (
        call "%VS_PATH%VsDevCmd.bat"
    )
    
    cmake .. -G "Visual Studio 16 2019" -A x64 ^
        -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
        -DCMAKE_CXX_STANDARD=17 ^
        -DWITH_LIBCXX=OFF ^
        -DANTLR4_INSTALL=ON
    
    cmake --build . --config Release --parallel %NUMBER_OF_PROCESSORS%
    cmake --build . --config Release --target install
)

echo ✓ ANTLR C++运行时编译完成
goto :eof

:verify_build
echo === 验证编译结果 ===

REM 检查ANTLR工具
if exist "%INSTALL_DIR%\antlr4.jar" (
    echo ✓ ANTLR工具: %INSTALL_DIR%\antlr4.jar
    java -jar "%INSTALL_DIR%\antlr4.jar" -version >nul 2>&1
    if !errorlevel!==0 (
        echo ✓ ANTLR工具可正常运行
    ) else (
        echo ❌ ANTLR工具无法运行
    )
) else (
    echo ❌ ANTLR工具未找到
)

REM 检查C++运行时
if exist "%INSTALL_DIR%\lib\antlr4-runtime.lib" (
    echo ✓ C++运行时库: %INSTALL_DIR%\lib\antlr4-runtime.lib
) else if exist "%INSTALL_DIR%\lib\libantlr4-runtime.a" (
    echo ✓ C++运行时库: %INSTALL_DIR%\lib\libantlr4-runtime.a
) else (
    echo ❌ C++运行时库未找到
)

REM 检查头文件
if exist "%INSTALL_DIR%\include\antlr4-runtime" (
    echo ✓ C++头文件: %INSTALL_DIR%\include\antlr4-runtime\
) else (
    echo ❌ C++头文件未找到
)
goto :eof

:generate_usage_guide
echo === 生成使用说明 ===

(
echo # ANTLR手动编译版本使用指南 ^(Windows^)
echo.
echo ## 版本信息
echo - ANTLR版本: %ANTLR_VERSION%
echo - 编译时间: %date% %time%
echo - 编译器: %COMPILER%
echo.
echo ## 使用ANTLR工具
echo.
echo ### Windows命令行:
echo ```cmd
echo REM 使用批处理文件
echo %INSTALL_DIR%\antlr4.bat MyGrammar.g4 -Dlanguage=Cpp -o generated\
echo.
echo REM 或直接使用JAR
echo java -jar %INSTALL_DIR%\antlr4.jar MyGrammar.g4 -Dlanguage=Cpp -o generated\
echo ```
echo.
echo ## 在CMake中使用
echo.
echo ```cmake
echo # 设置ANTLR路径
echo set^(ANTLR4_ROOT "%INSTALL_DIR%"^)
echo.
echo # 查找库
echo find_library^(ANTLR4_LIB 
echo     NAMES antlr4-runtime
echo     PATHS ${ANTLR4_ROOT}/lib
echo     NO_DEFAULT_PATH
echo ^)
echo.
echo # 设置头文件路径
echo set^(ANTLR4_INCLUDE_DIR "${ANTLR4_ROOT}/include"^)
echo.
echo # 链接库
echo target_link_libraries^(your_target ${ANTLR4_LIB}^)
echo target_include_directories^(your_target PRIVATE ${ANTLR4_INCLUDE_DIR}^)
echo ```
echo.
echo ## 环境变量设置
echo.
echo ### 添加到PATH ^(可选^):
echo ```cmd
echo set PATH=%INSTALL_DIR%;%%PATH%%
echo ```
echo.
echo ### 设置ANTLR4_ROOT:
echo ```cmd
echo set ANTLR4_ROOT=%INSTALL_DIR%
echo ```
) > "%INSTALL_DIR%\README.md"

echo ✓ 使用说明已生成: %INSTALL_DIR%\README.md
goto :eof