@echo off
REM CHTL Compiler Build Script - Skip ANTLR
REM This script uses Maven but skips ANTLR processing

echo ===================================
echo CHTL Compiler Build - Skip ANTLR
echo ===================================
echo.

REM Switch to project root
cd /d "%~dp0\..\.."

REM Check Maven
where mvn >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Maven not found. Using simple build instead...
    call "%~dp0\build-compiler-simple.bat"
    exit /b %errorlevel%
)

echo [INFO] Maven found, building with Maven...

REM First create stub files
echo [INFO] Creating ANTLR stub files...
call "%~dp0\create-stubs.bat" >nul 2>&1

REM Build with Maven, skipping ANTLR
echo [INFO] Compiling with Maven (skipping ANTLR)...
mvn clean compile -Dantlr4.skip=true -DskipTests

if %errorlevel% equ 0 (
    echo [INFO] Compilation successful
    
    REM Package
    mvn package -Dantlr4.skip=true -DskipTests
    
    if exist target\chtl-compiler-1.0.jar (
        echo [INFO] JAR created successfully
        
        REM Create distribution
        echo [INFO] Creating distribution...
        if not exist dist mkdir dist
        if not exist dist\lib mkdir dist\lib
        
        copy target\chtl-compiler-1.0.jar dist\
        copy target\lib\*.jar dist\lib\ 2>nul
        
        echo [INFO] Build completed successfully!
    )
) else (
    echo [ERROR] Maven build failed. Trying simple build...
    call "%~dp0\build-compiler-simple.bat"
)

echo.
echo Build process completed
pause