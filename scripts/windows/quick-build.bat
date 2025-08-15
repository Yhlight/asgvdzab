@echo off
REM Quick Build Script - Without Maven
REM This is a simplified build script that compiles the main CHTL files directly

setlocal enabledelayedexpansion

echo Quick CHTL Compiler Build
echo =========================
echo.

REM Switch to project root
cd /d "%~dp0\..\.."

REM Check Java
where java >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Java not found, please install Java 17 or higher
    exit /b 1
)

echo [INFO] Java found

REM Create directories
echo [INFO] Creating build directories...
if not exist "target\classes" mkdir "target\classes"

REM Find all Java files
echo [INFO] Finding Java source files...
dir /s /b src\main\java\*.java > java_files.txt

REM Count files
for /f %%i in ('type java_files.txt ^| find /c /v ""') do set FILE_COUNT=%%i
echo [INFO] Found %FILE_COUNT% Java files

REM Compile (without external dependencies for now)
echo [INFO] Compiling Java files...
echo [INFO] Note: This is a simplified build without Maven dependencies
javac -d target\classes -encoding UTF-8 @java_files.txt 2>compile_errors.txt

if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed. See compile_errors.txt for details
    type compile_errors.txt
    del java_files.txt
    exit /b 1
)

echo [INFO] Compilation successful

REM Create JAR
echo [INFO] Creating JAR file...
cd target\classes
jar cf ..\chtl-compiler-1.0.jar com\*
cd ..\..

if exist "target\chtl-compiler-1.0.jar" (
    echo [INFO] JAR created successfully: target\chtl-compiler-1.0.jar
    for %%F in ("target\chtl-compiler-1.0.jar") do echo [INFO] JAR size: %%~zF bytes
) else (
    echo [ERROR] Failed to create JAR
    exit /b 1
)

REM Clean up
del java_files.txt
if exist compile_errors.txt del compile_errors.txt

echo.
echo Build completed!
echo.
echo Note: This is a simplified build without Maven.
echo For a full build with all dependencies, please install Maven and run:
echo   mvn clean package

endlocal