@echo off
REM CHTL Complete Build Script
REM This script builds all components

echo ========================================
echo CHTL Complete Build Script
echo ========================================
echo.

setlocal

REM Parse arguments
set BUILD_COMPILER=true
set BUILD_VSCODE=true
set SKIP_TESTS=true

:parse_args
if "%1"=="" goto :end_parse
if "%1"=="--skip-vscode" set BUILD_VSCODE=false
if "%1"=="--skip-compiler" set BUILD_COMPILER=false
if "%1"=="--with-tests" set SKIP_TESTS=false
if "%1"=="--help" goto :show_usage
shift
goto :parse_args
:end_parse

REM Build compiler
if "%BUILD_COMPILER%"=="true" (
    echo [1/3] Building CHTL compiler...
    call "%~dp0\windows\build-compiler-precise.bat"
    if %errorlevel% neq 0 (
        echo [ERROR] Compiler build failed
        exit /b 1
    )
    echo [INFO] Compiler build completed
    echo.
)

REM Build modules
echo [2/3] Building official modules...
echo [INFO] Module packaging will be done by compiler build
echo.

REM Build VSCode extension
if "%BUILD_VSCODE%"=="true" (
    echo [3/3] Building VSCode extension...
    
    REM Check if build script exists
    if exist "%~dp0\windows\build-vscode-extension.bat" (
        call "%~dp0\windows\build-vscode-extension.bat"
        if %errorlevel% neq 0 (
            echo [WARNING] VSCode extension build failed
        ) else (
            echo [INFO] VSCode extension build completed
        )
    ) else (
        echo [WARNING] VSCode extension build script not found
        echo [INFO] Skipping VSCode extension build
    )
    echo.
)

echo ========================================
echo Build Summary:
if "%BUILD_COMPILER%"=="true" echo - Compiler: BUILT
if "%BUILD_VSCODE%"=="true" echo - VSCode Extension: ATTEMPTED
echo ========================================
echo.
echo Build process completed
goto :eof

:show_usage
echo Usage: %~nx0 [options]
echo.
echo Options:
echo   --skip-compiler    Skip compiler build
echo   --skip-vscode      Skip VSCode extension build
echo   --with-tests       Run tests during build
echo   --help             Show this help message
echo.
exit /b 0