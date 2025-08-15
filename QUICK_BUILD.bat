@echo off
REM Quick Build Script for CHTL Compiler
REM Just double-click to build!

echo ========================================
echo    CHTL Compiler Quick Build
echo ========================================
echo.

REM Try different build scripts in order of preference
echo Attempting to build CHTL compiler...
echo.

REM Try ordered build first
if exist "scripts\windows\build-ordered.bat" (
    echo [1/3] Trying ordered build...
    call scripts\windows\build-ordered.bat
    if exist "target\chtl-compiler-1.0.jar" goto :success
)

REM Try minimal build
if exist "scripts\windows\build-minimal.bat" (
    echo [2/3] Trying minimal build...
    call scripts\windows\build-minimal.bat
    if exist "target\chtl-compiler.jar" (
        ren target\chtl-compiler.jar chtl-compiler-1.0.jar
        goto :success
    )
)

REM Try complete build
if exist "scripts\build-all-complete.bat" (
    echo [3/3] Trying complete build...
    call scripts\build-all-complete.bat --skip-vscode
    if exist "target\chtl-compiler-1.0.jar" goto :success
)

echo.
echo [ERROR] All build attempts failed!
echo Please check the error messages above.
pause
exit /b 1

:success
echo.
echo ========================================
echo    BUILD SUCCESSFUL!
echo ========================================
echo.
echo Compiler JAR: target\chtl-compiler-1.0.jar
echo.
echo To test the compiler, run:
echo   java -cp "target\chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI --help
echo.
pause