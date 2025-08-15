@echo off
REM CHTL Compiler Build Script - Windows Platform (Fixed Version)
REM Build compiler and package official modules

setlocal

echo ===================================
echo CHTL Compiler Build Script - Windows
echo ===================================
echo.

REM Switch to project root
cd /d "%~dp0\..\.."

REM Check environment
echo [INFO] Checking build environment...

REM Check Java
where java >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Java not found, please install Java 17 or higher
    exit /b 1
)

REM Check Maven
where mvn >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Maven not found, please install Maven 3.6+
    exit /b 1
)

echo [INFO] Environment check passed

REM Clean build directories
echo [INFO] Cleaning build directories...
if exist target rmdir /s /q target
if exist build rmdir /s /q build
if exist dist rmdir /s /q dist

REM Compile project
echo [INFO] Compiling CHTL compiler...
mvn clean package -DskipTests
if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed
    exit /b 1
)
echo [INFO] Compilation successful

REM Create distribution
echo [INFO] Creating distribution package...

REM Create directories
mkdir dist\chtl-compiler-windows\bin 2>nul
mkdir dist\chtl-compiler-windows\lib 2>nul
mkdir dist\chtl-compiler-windows\modules 2>nul

REM Copy compiler JAR
copy target\chtl-compiler-1.0.jar dist\chtl-compiler-windows\lib\chtl-compiler.jar >nul

REM Create launcher script
(
echo @echo off
echo java -jar "%%~dp0\..\lib\chtl-compiler.jar" %%*
) > dist\chtl-compiler-windows\bin\chtl.bat

echo [INFO] Distribution created

REM Package as ZIP
cd dist
powershell -Command "if (Test-Path 'chtl-compiler-windows.zip') { Remove-Item 'chtl-compiler-windows.zip' }; Compress-Archive -Path 'chtl-compiler-windows' -DestinationPath 'chtl-compiler-windows.zip'"
cd ..

echo.
echo ===================================
echo Build completed successfully!
echo ===================================
echo.
echo Output: dist\chtl-compiler-windows.zip

endlocal