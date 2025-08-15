@echo off
REM CHTL Diagnostic Build Script - Windows
REM Shows detailed compilation errors

echo ===================================
echo CHTL Diagnostic Build - Windows
echo ===================================
echo.

cd /d "%~dp0\..\.."

REM Check Java
echo [INFO] Checking Java version...
java -version
echo.

REM Create directories
echo [INFO] Creating directories...
if not exist target\classes mkdir target\classes
if not exist lib mkdir lib
if not exist temp mkdir temp

REM Create ANTLR stubs
echo [INFO] Creating ANTLR stubs...
call "%~dp0\create-stubs.bat"

REM Download one dependency at a time
echo.
echo [INFO] Downloading dependencies one by one...

if not exist "lib\antlr4-runtime-4.13.1.jar" (
    echo Downloading ANTLR runtime...
    curl -L -o "lib\antlr4-runtime-4.13.1.jar" "https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar"
    if not exist "lib\antlr4-runtime-4.13.1.jar" (
        echo [WARNING] Failed to download ANTLR runtime
    )
)

if not exist "lib\slf4j-api-2.0.9.jar" (
    echo Downloading SLF4J API...
    curl -L -o "lib\slf4j-api-2.0.9.jar" "https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar"
)

if not exist "lib\logback-classic-1.4.11.jar" (
    echo Downloading Logback Classic...
    curl -L -o "lib\logback-classic-1.4.11.jar" "https://repo1.maven.org/maven2/ch/qos/logback/logback-classic/1.4.11/logback-classic-1.4.11.jar"
)

if not exist "lib\logback-core-1.4.11.jar" (
    echo Downloading Logback Core...
    curl -L -o "lib\logback-core-1.4.11.jar" "https://repo1.maven.org/maven2/ch/qos/logback/logback-core/1.4.11/logback-core-1.4.11.jar"
)

echo.
echo [INFO] Checking downloaded files...
dir lib\*.jar

REM Try to compile a simple test file first
echo.
echo [TEST] Compiling a simple test class...
(
echo package com.chtl.test;
echo public class TestCompile {
echo     public static void main^(String[] args^) {
echo         System.out.println^("Test successful!"^);
echo     }
echo }
) > src\main\java\com\chtl\test\TestCompile.java

javac -d target\classes src\main\java\com\chtl\test\TestCompile.java
if %errorlevel% equ 0 (
    echo [SUCCESS] Test compilation works!
    dir target\classes\com\chtl\test\*.class
) else (
    echo [ERROR] Even simple compilation failed!
)

REM Now try to compile core files with verbose output
echo.
echo [INFO] Trying to compile core files with detailed errors...
echo.

REM Create a small file list
del temp\core_files.txt 2>nul
echo src\main\java\com\chtl\model\FragmentType.java >> temp\core_files.txt
echo src\main\java\com\chtl\ast\SourceLocation.java >> temp\core_files.txt
echo src\main\java\com\chtl\compiler\debug\CompilationException.java >> temp\core_files.txt

echo [INFO] Compiling core files...
javac -cp "lib\*" -d target\classes -encoding UTF-8 @temp\core_files.txt
if %errorlevel% neq 0 (
    echo [ERROR] Core compilation failed
) else (
    echo [SUCCESS] Core files compiled!
)

REM Try to compile everything and show errors
echo.
echo [INFO] Attempting full compilation...
dir /s /b src\main\java\*.java > temp\all_files.txt

echo [INFO] First, let's see first few files to compile:
type temp\all_files.txt | more /E +1 /C +5

echo.
echo [INFO] Compiling with error output...
javac -cp "target\classes;lib\*" -d target\classes -encoding UTF-8 @temp\all_files.txt 2> temp\compile_errors.txt

echo.
echo [INFO] Compilation result: %errorlevel%
echo.
echo [INFO] First 20 compilation errors:
echo ========================================
type temp\compile_errors.txt | more /E +1 /C +20
echo ========================================

REM Check if any classes were created
echo.
echo [INFO] Checking for compiled classes...
dir /s target\classes\*.class 2>nul | find /c ".class" > temp\class_count.txt
set /p CLASS_COUNT=<temp\class_count.txt
echo [INFO] Number of compiled classes: %CLASS_COUNT%

if %CLASS_COUNT% gtr 0 (
    echo.
    echo [INFO] Some classes were compiled. Creating JAR...
    cd target\classes
    jar cf ..\chtl-compiler-diagnostic.jar com\* 2>nul
    cd ..\..
    
    if exist target\chtl-compiler-diagnostic.jar (
        echo [SUCCESS] JAR created!
        for %%F in (target\chtl-compiler-diagnostic.jar) do echo [INFO] JAR size: %%~zF bytes
    )
)

echo.
echo [INFO] Full error log saved to: temp\compile_errors.txt
echo [INFO] You can review it with: type temp\compile_errors.txt ^| more
echo.
pause