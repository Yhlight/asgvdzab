@echo off
echo ========================================
echo CHTL Simple Build Script
echo ========================================
echo.

REM Go to project root
cd /d "%~dp0\.."

REM Check Java
echo Checking Java...
java -version 2>nul
if %errorlevel% neq 0 (
    echo ERROR: Java not found. Please install Java 17 or higher.
    pause
    exit /b 1
)

REM Check if Maven exists
echo.
echo Checking Maven...
mvn --version 2>nul
if %errorlevel% neq 0 (
    echo WARNING: Maven not found. Using direct Java compilation instead.
    goto :direct_compile
)

REM Maven build
echo.
echo Building with Maven...
mvn clean package -DskipTests
if %errorlevel% neq 0 (
    echo ERROR: Maven build failed
    pause
    exit /b 1
)
goto :build_success

:direct_compile
echo.
echo Building with direct Java compilation...

REM Create directories
if not exist "target\classes" mkdir "target\classes"

REM Find and compile Java files
echo Finding Java files...
dir /s /b src\main\java\*.java > java_files.txt
echo Compiling...
javac -d target\classes -encoding UTF-8 @java_files.txt 2>compile_errors.txt

if %errorlevel% neq 0 (
    echo ERROR: Compilation failed
    type compile_errors.txt
    del java_files.txt
    pause
    exit /b 1
)

REM Create JAR
echo Creating JAR file...
cd target\classes
jar cf ..\chtl-compiler-1.0.jar com\*
cd ..\..

del java_files.txt
if exist compile_errors.txt del compile_errors.txt

:build_success
echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.

REM Check if JAR exists
if exist "target\chtl-compiler-1.0.jar" (
    echo Output: target\chtl-compiler-1.0.jar
    for %%F in ("target\chtl-compiler-1.0.jar") do echo Size: %%~zF bytes
) else (
    echo ERROR: JAR file not created
    pause
    exit /b 1
)

echo.
echo Press any key to exit...
pause >nul