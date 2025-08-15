@echo off
REM CHTL Unified Build Script - Fixed Version

setlocal

echo ========================================
echo CHTL Unified Build Script
echo ========================================
echo.

REM Parse command line arguments
set BUILD_TARGET=all

if "%1"=="compiler" set BUILD_TARGET=compiler
if "%1"=="vscode" set BUILD_TARGET=vscode
if "%1"=="clean" set BUILD_TARGET=clean
if "%1"=="--help" goto :show_help
if "%1"=="-h" goto :show_help

REM Change to script directory
cd /d "%~dp0"

if "%BUILD_TARGET%"=="clean" goto :clean
if "%BUILD_TARGET%"=="compiler" goto :build_compiler_only
if "%BUILD_TARGET%"=="vscode" goto :build_vscode_only

REM Build all
:build_all
echo [1/2] Building CHTL compiler...
call windows\build-compiler-fixed.bat
if %errorlevel% neq 0 (
    echo [ERROR] Compiler build failed
    exit /b 1
)

echo.
echo [2/2] Building VSCode extension...
echo [INFO] VSCode extension build skipped (run separately if needed)

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
goto :end

:build_compiler_only
echo Building CHTL compiler...
call windows\build-compiler-fixed.bat
goto :end

:build_vscode_only
echo [INFO] VSCode extension build not implemented in fixed version
goto :end

:clean
echo Cleaning build outputs...
cd ..
if exist target rmdir /s /q target
if exist build rmdir /s /q build
if exist dist rmdir /s /q dist
echo Clean completed
goto :end

:show_help
echo Usage: build-all-fixed.bat [target]
echo.
echo Targets:
echo   all       Build all components (default)
echo   compiler  Build CHTL compiler only
echo   vscode    Build VSCode extension only
echo   clean     Clean all build outputs
echo.
echo Options:
echo   -h, --help  Show this help message
goto :end

:end
endlocal