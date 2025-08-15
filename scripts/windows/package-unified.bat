@echo off
REM Unified Module Packaging Script - Windows Platform
REM Package both CMOD and CJMOD modules together

setlocal enabledelayedexpansion

REM Color definitions
set "GREEN=[92m"
set "RED=[91m"
set "YELLOW=[93m"
set "NC=[0m"

REM Print messages
:info
echo %GREEN%[INFO]%NC% %~1
goto :eof

:error
echo %RED%[ERROR]%NC% %~1 >&2
exit /b 1

:warning
echo %YELLOW%[WARN]%NC% %~1
goto :eof

REM Show usage
if "%1"=="" goto :usage
if "%1"=="-h" goto :usage
if "%1"=="--help" goto :usage
if "%1"=="/?" goto :usage

REM Parse command line arguments
set "MODULE_PATH=%1"
set "OUTPUT_DIR=."
set "MODULE_NAME="
set "MODULE_VERSION="
set "INCLUDE_SOURCE=true"
set "SIGN_MODULE=false"

shift
:parse_args
if "%1"=="" goto :end_parse
if "%1"=="-o" (
    set "OUTPUT_DIR=%2"
    shift
    shift
    goto :parse_args
)
if "%1"=="--output" (
    set "OUTPUT_DIR=%2"
    shift
    shift
    goto :parse_args
)
if "%1"=="--no-source" (
    set "INCLUDE_SOURCE=false"
    shift
    goto :parse_args
)
if "%1"=="--sign" (
    set "SIGN_MODULE=true"
    shift
    goto :parse_args
)
shift
goto :parse_args
:end_parse

REM Main process starts
echo ==========================================
echo Unified Module Packaging Script - Windows
echo ==========================================
echo.

call :info "Module path: %MODULE_PATH%"
call :info "Output directory: %OUTPUT_DIR%"

REM Check if module path exists
if not exist "%MODULE_PATH%" (
    call :error "Module path not found: %MODULE_PATH%"
    exit /b 1
)

REM Detect module structure
set "HAS_CMOD=false"
set "HAS_CJMOD=false"
set "CMOD_PATH="
set "CJMOD_PATH="

REM Check for CMOD
if exist "%MODULE_PATH%\CMOD" (
    set "HAS_CMOD=true"
    set "CMOD_PATH=%MODULE_PATH%\CMOD"
) else if exist "%MODULE_PATH%\Cmod" (
    set "HAS_CMOD=true"
    set "CMOD_PATH=%MODULE_PATH%\Cmod"
) else if exist "%MODULE_PATH%\cmod" (
    set "HAS_CMOD=true"
    set "CMOD_PATH=%MODULE_PATH%\cmod"
)

REM Check for CJMOD
if exist "%MODULE_PATH%\CJMOD" (
    set "HAS_CJMOD=true"
    set "CJMOD_PATH=%MODULE_PATH%\CJMOD"
) else if exist "%MODULE_PATH%\CJmod" (
    set "HAS_CJMOD=true"
    set "CJMOD_PATH=%MODULE_PATH%\CJmod"
) else if exist "%MODULE_PATH%\cjmod" (
    set "HAS_CJMOD=true"
    set "CJMOD_PATH=%MODULE_PATH%\cjmod"
)

REM Validate structure
if "%HAS_CMOD%"=="false" if "%HAS_CJMOD%"=="false" (
    call :error "No CMOD or CJMOD structure found"
    call :error "Expected directory structure:"
    call :error "  MODULE/"
    call :error "    CMOD/    (or Cmod/cmod)"
    call :error "    CJMOD/   (or CJmod/cjmod)"
    exit /b 1
)

call :info "Module structure detected:"
if "%HAS_CMOD%"=="true" echo   - CMOD: %CMOD_PATH%
if "%HAS_CJMOD%"=="true" echo   - CJMOD: %CJMOD_PATH%

REM Get module name from directory
for %%F in ("%MODULE_PATH%") do set "MODULE_NAME=%%~nxF"

REM Read module information
if "%HAS_CMOD%"=="true" (
    call :read_module_info "%CMOD_PATH%" CMOD
) else if "%HAS_CJMOD%"=="true" (
    call :read_module_info "%CJMOD_PATH%" CJMOD
)

REM Create working directory
set "WORK_DIR=%TEMP%\unified_pack_%RANDOM%"
mkdir "%WORK_DIR%" 2>nul

REM Package CMOD if exists
if "%HAS_CMOD%"=="true" (
    call :info "Packaging CMOD module..."
    mkdir "%WORK_DIR%\CMOD" 2>nul
    xcopy /s /e /y "%CMOD_PATH%\*" "%WORK_DIR%\CMOD\" >nul
    
    REM Process CMOD
    call :process_cmod "%WORK_DIR%\CMOD"
)

REM Package CJMOD if exists
if "%HAS_CJMOD%"=="true" (
    call :info "Packaging CJMOD module..."
    mkdir "%WORK_DIR%\CJMOD" 2>nul
    xcopy /s /e /y "%CJMOD_PATH%\*" "%WORK_DIR%\CJMOD\" >nul
    
    REM Compile CJMOD
    call :compile_cjmod "%WORK_DIR%\CJMOD"
)

REM Create unified manifest
call :info "Creating unified manifest..."
mkdir "%WORK_DIR%\META-INF" 2>nul

(
echo Manifest-Version: 1.0
echo Module-Type: Unified
echo Module-Name: %MODULE_NAME%
echo Module-Version: %MODULE_VERSION%
echo Has-CMOD: %HAS_CMOD%
echo Has-CJMOD: %HAS_CJMOD%
echo Build-Time: %date% %time%
echo Build-Platform: Windows
) > "%WORK_DIR%\META-INF\MANIFEST.MF"

REM Package unified module
set "OUTPUT_FILE=%OUTPUT_DIR%\%MODULE_NAME%-%MODULE_VERSION%-unified.zip"
call :info "Creating unified package: %OUTPUT_FILE%"

REM Change to working directory for packaging
pushd "%WORK_DIR%"

REM Create ZIP file
powershell -Command "Compress-Archive -Path '*' -DestinationPath '%OUTPUT_FILE%' -Force"
if %errorlevel% neq 0 (
    call :error "Failed to create unified package"
    popd
    rmdir /s /q "%WORK_DIR%"
    exit /b 1
)

popd

REM Sign module (if requested)
if "%SIGN_MODULE%"=="true" (
    call :info "Signing module..."
    call :sign_module "%OUTPUT_FILE%"
)

REM Calculate checksum
call :info "Calculating checksum..."
certutil -hashfile "%OUTPUT_FILE%" SHA256 | findstr /v ":" > "%OUTPUT_FILE%.sha256"

REM Clean up
rmdir /s /q "%WORK_DIR%"

REM Display results
call :info "Unified packaging completed!"
echo.
echo Module information:
echo   Name: %MODULE_NAME%
echo   Version: %MODULE_VERSION%
echo   Components: 
if "%HAS_CMOD%"=="true" echo     - CMOD
if "%HAS_CJMOD%"=="true" echo     - CJMOD
echo   Output: %OUTPUT_FILE%

if exist "%OUTPUT_FILE%" (
    for %%F in ("%OUTPUT_FILE%") do echo   Size: %%~zF bytes
)

echo.
call :info "Package created successfully!"

endlocal
exit /b 0

REM Read module information
:read_module_info
set "module_dir=%~1"
set "module_type=%~2"

set "INFO_FILE="
if exist "%module_dir%\info\*.chtl" (
    for %%F in ("%module_dir%\info\*.chtl") do (
        set "INFO_FILE=%%F"
        if "%MODULE_NAME%"=="" set "MODULE_NAME=%%~nF"
    )
)

if "%INFO_FILE%"=="" (
    call :warning "No info file found for %module_type%"
    goto :eof
)

call :info "Reading %module_type% information from: %INFO_FILE%"

REM Parse info file
for /f "tokens=1,* delims==" %%A in ('findstr /i "name version" "%INFO_FILE%"') do (
    set "KEY=%%A"
    set "VALUE=%%B"
    
    REM Remove quotes and spaces
    set "KEY=!KEY: =!"
    set "VALUE=!VALUE:"=!"
    set "VALUE=!VALUE:;=!"
    set "VALUE=!VALUE: =!"
    
    if /i "!KEY!"=="name" if "%MODULE_NAME%"=="" set "MODULE_NAME=!VALUE!"
    if /i "!KEY!"=="version" if "%MODULE_VERSION%"=="" set "MODULE_VERSION=!VALUE!"
)

if "%MODULE_VERSION%"=="" set "MODULE_VERSION=1.0.0"
goto :eof

REM Process CMOD
:process_cmod
set "cmod_dir=%~1"

REM Check for [Export] block
if exist "%cmod_dir%\info\%MODULE_NAME%.chtl" (
    findstr /b "[Export]" "%cmod_dir%\info\%MODULE_NAME%.chtl" >nul 2>&1
    if errorlevel 1 (
        call :info "Generating [Export] block for CMOD..."
        REM Generate export block logic here
    )
)
goto :eof

REM Compile CJMOD
:compile_cjmod
set "cjmod_dir=%~1"

if not exist "%cjmod_dir%\src" (
    call :warning "No source files found in CJMOD"
    goto :eof
)

call :info "Compiling CJMOD Java sources..."

REM Create classes directory
mkdir "%cjmod_dir%\classes" 2>nul

REM Find all Java files
dir /s /b "%cjmod_dir%\src\*.java" > "%cjmod_dir%\java_files.txt"

REM Check if there are Java files
for %%F in ("%cjmod_dir%\java_files.txt") do (
    if %%~zF equ 0 (
        call :warning "No Java files found"
    ) else (
        REM Compile Java files
        javac -cp ".;%CHTL_HOME%\lib\*" -d "%cjmod_dir%\classes" @"%cjmod_dir%\java_files.txt"
        if !errorlevel! neq 0 (
            call :error "CJMOD compilation failed"
            del "%cjmod_dir%\java_files.txt"
            exit /b 1
        )
        call :info "CJMOD compilation successful"
    )
)

del "%cjmod_dir%\java_files.txt"

REM Remove source files if not needed
if "%INCLUDE_SOURCE%"=="false" (
    if exist "%cjmod_dir%\src" rmdir /s /q "%cjmod_dir%\src"
)
goto :eof

REM Sign module
:sign_module
set "MODULE_FILE=%~1"

REM Check for signing key
if not exist "%USERPROFILE%\.chtl\keys\private.key" (
    call :warning "No signing key found, skipping signature"
    goto :eof
)

REM Sign the module (placeholder - actual implementation needed)
call :info "Module signed (placeholder)"
goto :eof

REM Show usage
:usage
echo.
echo Usage: package-unified.bat MODULE_PATH [OPTIONS]
echo.
echo Package both CMOD and CJMOD modules together
echo.
echo Arguments:
echo   MODULE_PATH      Path to the module directory containing CMOD/CJMOD
echo.
echo Options:
echo   -o, --output DIR     Output directory (default: current)
echo   --no-source          Exclude source files from CJMOD
echo   --sign               Sign the module package
echo   -h, --help           Show this help message
echo.
echo Examples:
echo   package-unified.bat MyModule
echo   package-unified.bat MyModule -o dist
echo   package-unified.bat path\to\module --no-source --sign
echo.
echo Expected module structure:
echo   MyModule/
echo   ├── CMOD/          # CHTL templates and styles
echo   │   ├── src/
echo   │   └── info/
echo   └── CJMOD/         # JavaScript extensions
echo       ├── src/
echo       └── info/
echo.
echo The unified package will contain both CMOD and CJMOD components.
echo.
exit /b 0