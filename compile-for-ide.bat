@echo off
REM CHTL Compiler Build Script for IDE
REM This script compiles the project without Maven

echo CHTL Compiler Build for IDE
echo ===========================
echo.

REM Create necessary directories
echo Creating directories...
if not exist "target\classes" mkdir "target\classes"
if not exist "lib" mkdir "lib"

REM Check dependencies
echo.
echo Checking dependencies...
set DEPS_NEEDED=false

if not exist "lib\antlr4-runtime-4.13.1.jar" (
    echo WARNING: ANTLR runtime not found. Some features may not work.
    echo To fix: Download antlr4-runtime-4.13.1.jar from https://www.antlr.org/
    set DEPS_NEEDED=true
)

if not exist "lib\slf4j-api-2.0.9.jar" (
    echo WARNING: SLF4J API not found. Logging may not work.
    echo To fix: Download slf4j-api-2.0.9.jar from https://www.slf4j.org/
    set DEPS_NEEDED=true
)

if "%DEPS_NEEDED%"=="true" (
    echo.
    echo Some dependencies are missing. The compilation will continue but some features may not work.
    pause
)

REM Set classpath
set CLASSPATH=target\classes
for %%j in (lib\*.jar) do (
    set CLASSPATH=!CLASSPATH!;%%j
)

REM Find all Java files
echo.
echo Finding Java source files...
dir /s /b src\main\java\*.java > java_files.txt
for /f %%i in ('type java_files.txt ^| find /c /v ""') do set TOTAL_FILES=%%i
echo Found %TOTAL_FILES% Java files

REM Compile in stages
echo.
echo Stage 1: Compiling model classes...
javac -cp "%CLASSPATH%" -d target\classes src\main\java\com\chtl\model\*.java src\main\java\com\chtl\exception\*.java 2>compile_errors_stage1.txt

if %errorlevel% neq 0 (
    echo Errors in stage 1:
    type compile_errors_stage1.txt
) else (
    echo Stage 1 completed successfully
)

echo.
echo Stage 2: Compiling all classes...
javac -cp "%CLASSPATH%" -d target\classes -encoding UTF-8 @java_files.txt 2>compile_errors_final.txt

if %errorlevel% equ 0 (
    echo Compilation completed successfully!
    
    REM Create JAR file
    echo.
    echo Creating JAR file...
    cd target\classes
    jar cf ..\chtl-compiler-1.0.jar com\*
    cd ..\..
    
    if exist "target\chtl-compiler-1.0.jar" (
        echo JAR created: target\chtl-compiler-1.0.jar
        for %%F in ("target\chtl-compiler-1.0.jar") do echo Size: %%~zF bytes
    )
) else (
    echo Compilation failed. First 50 errors:
    powershell -Command "Get-Content compile_errors_final.txt | Select-Object -First 50"
)

REM Cleanup
del java_files.txt

echo.
echo Build process completed
echo.
echo To run in IDE:
echo 1. Add target\classes to your IDE's classpath
echo 2. Add any JAR files from lib\ directory to classpath
echo 3. Main class: com.chtl.cli.CHTLCLI
echo.
pause