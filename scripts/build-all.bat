@echo off
REM CHTL Project Unified Build Script - Windows Version

setlocal enabledelayedexpansion

REM Color definitions
set "GREEN=[92m"
set "RED=[91m"
set "YELLOW=[93m"
set "BLUE=[94m"
set "NC=[0m"

REM Default values
set "BUILD_TARGET=all"
set "PLATFORM=windows"
set "SKIP_TESTS=false"
set "MODULE_PATH="

REM Parse arguments
:parse_args
if "%1"=="" goto :check_params
if "%1"=="all" (
    set "BUILD_TARGET=all"
    shift
    goto :parse_args
)
if "%1"=="compiler" (
    set "BUILD_TARGET=compiler"
    shift
    goto :parse_args
)
if "%1"=="vscode" (
    set "BUILD_TARGET=vscode"
    shift
    goto :parse_args
)
if "%1"=="clean" (
    set "BUILD_TARGET=clean"
    shift
    goto :parse_args
)
if "%1"=="module" (
    set "BUILD_TARGET=module"
    set "MODULE_PATH=%2"
    set "MODULE_TYPE=unified"
    shift
    shift
    REM Continue parsing module-specific parameters
    :parse_module_type
    if "%1"=="--type" (
        if "%2"=="cmod" set "MODULE_TYPE=cmod"
        if "%2"=="cjmod" set "MODULE_TYPE=cjmod"
        shift
        shift
    )
    goto :parse_args
)
if "%1"=="--skip-tests" (
    set "SKIP_TESTS=true"
    shift
    goto :parse_args
)
if "%1"=="-h" goto :show_usage
if "%1"=="--help" goto :show_usage
echo %RED%[ERROR]%NC% Unknown parameter: %1
goto :show_usage

:check_params
goto :start_build

:show_usage
echo Usage: build-all.bat [target] [options]
echo.
echo Targets:
echo   all          Build all components (default)
echo   compiler     Build CHTL compiler only
echo   vscode       Build VSCode extension only
echo   module       Build specific module
echo   clean        Clean all build outputs
echo.
echo Options:
echo   --skip-tests     Skip running tests
echo   --type TYPE      Module type (cmod/cjmod), for module target
echo.
echo Examples:
echo   build-all.bat                     # Build everything
echo   build-all.bat compiler            # Build compiler only
echo   build-all.bat module Chtholly     # Build Chtholly module
echo   build-all.bat --skip-tests        # Build without tests
exit /b 1

:start_build
echo %GREEN%========================================%NC%
echo %GREEN% CHTL Unified Build Script%NC%
echo %GREEN% Platform: %PLATFORM%%NC%
echo %GREEN% Target: %BUILD_TARGET%%NC%
echo %GREEN%========================================%NC%
echo.

if "%BUILD_TARGET%"=="clean" goto :clean_all
if "%BUILD_TARGET%"=="compiler" goto :build_compiler
if "%BUILD_TARGET%"=="vscode" goto :build_vscode
if "%BUILD_TARGET%"=="module" goto :build_module
if "%BUILD_TARGET%"=="all" goto :build_all

:build_all
echo %BLUE%[1/3] Building CHTL compiler...%NC%
call :build_compiler
if errorlevel 1 goto :error

echo.
echo %BLUE%[2/3] Building official modules...%NC%
call :build_modules
if errorlevel 1 goto :error

echo.
echo %BLUE%[3/3] Building VSCode extension...%NC%
call :build_vscode
if errorlevel 1 goto :error

echo.
echo %GREEN%========================================%NC%
echo %GREEN% Build completed successfully!%NC%
echo %GREEN%========================================%NC%
goto :end

:build_compiler
    echo %GREEN%[INFO]%NC% Starting compiler build...
    pushd "%~dp0\windows"
    call build-compiler.bat
    set BUILD_RESULT=%errorlevel%
    popd
    if %BUILD_RESULT% neq 0 (
        echo %RED%[ERROR]%NC% Compiler build failed
        exit /b 1
    )
    exit /b 0

:build_modules
    echo %GREEN%[INFO]%NC% Building official modules...
    REM Build Chtholly module
    if exist "%~dp0\..\src\main\java\com\chtl\module\Chtholly" (
        echo %GREEN%[INFO]%NC% Building Chtholly module...
        pushd "%~dp0\windows"
        call package-unified.bat "%~dp0\..\src\main\java\com\chtl\module\Chtholly"
        if errorlevel 1 (
            echo %YELLOW%[WARN]%NC% Chtholly module build failed, continuing...
        )
        popd
    )
    exit /b 0

:build_vscode
    echo %GREEN%[INFO]%NC% Starting VSCode extension build...
    pushd "%~dp0\windows"
    call build-vscode-extension.bat
    set BUILD_RESULT=%errorlevel%
    popd
    if %BUILD_RESULT% neq 0 (
        echo %RED%[ERROR]%NC% VSCode extension build failed
        exit /b 1
    )
    exit /b 0

:build_module
if "%MODULE_PATH%"=="" (
    echo %RED%[ERROR]%NC% Module path not specified
    goto :show_usage
)
echo %GREEN%[INFO]%NC% Building module: %MODULE_PATH%
echo %GREEN%[INFO]%NC% Module type: %MODULE_TYPE%
cd /d "%~dp0\windows"
if "%MODULE_TYPE%"=="cmod" (
    call package-cmod.bat "%MODULE_PATH%"
) else if "%MODULE_TYPE%"=="cjmod" (
    call package-cjmod.bat "%MODULE_PATH%"
) else (
    call package-unified.bat "%MODULE_PATH%"
)
if errorlevel 1 (
    echo %RED%[ERROR]%NC% Module build failed
    exit /b 1
)
cd /d "%~dp0\.."
exit /b 0

:clean_all
echo %GREEN%[INFO]%NC% Cleaning build outputs...

REM Clean compiler build
if exist "target" (
    echo %GREEN%[INFO]%NC% Cleaning compiler build...
    rd /s /q "target"
)

REM Clean test outputs
if exist "test-output" (
    echo %GREEN%[INFO]%NC% Cleaning test outputs...
    rd /s /q "test-output"
)

REM Clean module builds
if exist "build\modules" (
    echo %GREEN%[INFO]%NC% Cleaning module builds...
    rd /s /q "build\modules"
)

REM Clean VSCode extension build
if exist "vscode-chtl\out" (
    echo %GREEN%[INFO]%NC% Cleaning VSCode extension build...
    rd /s /q "vscode-chtl\out"
)
if exist "vscode-chtl\*.vsix" (
    del /q "vscode-chtl\*.vsix"
)

echo %GREEN%[INFO]%NC% Clean completed
goto :end

:error
echo %RED%========================================%NC%
echo %RED% Build failed!%NC%
echo %RED%========================================%NC%
exit /b 1

:end
endlocal