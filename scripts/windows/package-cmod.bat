@echo off
REM CMOD Module Packaging Script - Windows Platform

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
set "GENERATE_EXPORT=true"
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
if "%1"=="--no-export" (
    set "GENERATE_EXPORT=false"
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
echo ======================================
echo CMOD Module Packaging Script - Windows
echo ======================================
echo.

call :info "Module path: %MODULE_PATH%"
call :info "Output directory: %OUTPUT_DIR%"

REM Check if module path exists
if not exist "%MODULE_PATH%" (
    call :error "Module path not found: %MODULE_PATH%"
    exit /b 1
)

REM Check CMOD structure
set "HAS_CMOD_STRUCTURE=false"
if exist "%MODULE_PATH%\CMOD" set "HAS_CMOD_STRUCTURE=true"
if exist "%MODULE_PATH%\Cmod" set "HAS_CMOD_STRUCTURE=true"
if exist "%MODULE_PATH%\cmod" set "HAS_CMOD_STRUCTURE=true"

if "%HAS_CMOD_STRUCTURE%"=="false" (
    REM Check if current directory is already a CMOD directory
    if exist "%MODULE_PATH%\src" (
        if exist "%MODULE_PATH%\info" (
            call :info "Detected CMOD directory structure"
        ) else (
            call :error "Invalid CMOD structure: missing info directory"
            exit /b 1
        )
    ) else (
        call :error "Invalid CMOD structure: missing src directory"
        exit /b 1
    )
) else (
    REM Handle module with CMOD + CJMOD structure
    call :info "Detected CMOD + CJMOD structure"
    if exist "%MODULE_PATH%\CMOD" (
        set "MODULE_PATH=%MODULE_PATH%\CMOD"
    ) else if exist "%MODULE_PATH%\Cmod" (
        set "MODULE_PATH=%MODULE_PATH%\Cmod"
    ) else (
        set "MODULE_PATH=%MODULE_PATH%\cmod"
    )
)

REM Read module information
call :read_module_info

REM Create working directory
set "WORK_DIR=%TEMP%\cmod_pack_%RANDOM%"
mkdir "%WORK_DIR%" 2>nul

REM Copy module structure
call :info "Copying module structure..."
xcopy /s /e /y "%MODULE_PATH%\*" "%WORK_DIR%\" >nul

REM Process CHTL files
call :info "Processing CHTL files..."
call :process_chtl_files "%WORK_DIR%"

REM Generate [Export] block if needed
if "%GENERATE_EXPORT%"=="true" (
    call :info "Generating [Export] block..."
    call :generate_export_block "%WORK_DIR%"
)

REM Create META-INF
mkdir "%WORK_DIR%\META-INF" 2>nul

REM Create manifest file
(
echo Manifest-Version: 1.0
echo Module-Type: CMOD
echo Module-Name: %MODULE_NAME%
echo Module-Version: %MODULE_VERSION%
echo Build-Time: %date% %time%
echo Build-Platform: Windows
) > "%WORK_DIR%\META-INF\MANIFEST.MF"

REM Package module
set "OUTPUT_FILE=%OUTPUT_DIR%\%MODULE_NAME%-%MODULE_VERSION%.cmod"
call :info "Creating CMOD package: %OUTPUT_FILE%"

REM Change to working directory for packaging
pushd "%WORK_DIR%"

REM Create ZIP file (CMOD format)
powershell -Command "Compress-Archive -Path '*' -DestinationPath '%OUTPUT_FILE%' -Force"
if %errorlevel% neq 0 (
    call :error "Failed to create CMOD package"
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
call :info "CMOD packaging completed!"
echo.
echo Module information:
echo   Name: %MODULE_NAME%
echo   Version: %MODULE_VERSION%
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
set "INFO_FILE="
if exist "%MODULE_PATH%\info\*.chtl" (
    for %%F in ("%MODULE_PATH%\info\*.chtl") do (
        set "INFO_FILE=%%F"
        set "MODULE_NAME=%%~nF"
    )
)

if "%INFO_FILE%"=="" (
    call :error "No info file found in info directory"
    exit /b 1
)

call :info "Reading module information from: %INFO_FILE%"

REM Parse info file
for /f "tokens=1,* delims==" %%A in ('findstr /i "name version" "%INFO_FILE%"') do (
    set "KEY=%%A"
    set "VALUE=%%B"
    
    REM Remove quotes and spaces
    set "KEY=!KEY: =!"
    set "VALUE=!VALUE:"=!"
    set "VALUE=!VALUE:;=!"
    set "VALUE=!VALUE: =!"
    
    if /i "!KEY!"=="name" set "MODULE_NAME=!VALUE!"
    if /i "!KEY!"=="version" set "MODULE_VERSION=!VALUE!"
)

if "%MODULE_VERSION%"=="" set "MODULE_VERSION=1.0.0"

call :info "Module: %MODULE_NAME% v%MODULE_VERSION%"
goto :eof

REM Process CHTL files
:process_chtl_files
set "base_dir=%~1"

REM Process all .chtl files
for /r "%base_dir%\src" %%F in (*.chtl) do (
    call :info "Processing: %%~nxF"
    REM Here you would call CHTL compiler if needed
)

REM Process submodules
for /d %%D in ("%base_dir%\src\*") do (
    if exist "%%D\src" if exist "%%D\info" (
        call :info "Found submodule: %%~nxD"
        call :process_chtl_files "%%D"
    )
)
goto :eof

REM Generate [Export] block
:generate_export_block
set "base_dir=%~1"
set "info_file=%base_dir%\info\%MODULE_NAME%.chtl"

REM Check if [Export] already exists
findstr /b "[Export]" "%info_file%" >nul 2>&1
if not errorlevel 1 (
    call :info "[Export] block already exists"
    goto :eof
)

REM Analyze module to generate export list
call :info "Analyzing module structure..."

REM Append [Export] block to info file
echo. >> "%info_file%"
echo [Export] { >> "%info_file%"

REM Find all templates
for /f "delims=" %%F in ('findstr /s /r "^\[Template\].*@.*" "%base_dir%\src\*.chtl"') do (
    REM Extract template name
    echo %%F | findstr "@Style" >nul && echo     @Style %%~nF; >> "%info_file%"
    echo %%F | findstr "@Component" >nul && echo     @Component %%~nF; >> "%info_file%"
    echo %%F | findstr "@Element" >nul && echo     @Element %%~nF; >> "%info_file%"
)

REM Find all variables
for /f "delims=" %%F in ('findstr /s /r "^\[Var\]" "%base_dir%\src\*.chtl"') do (
    echo     @Var %%~nF; >> "%info_file%"
)

echo } >> "%info_file%"
call :info "[Export] block generated"
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
echo Usage: package-cmod.bat MODULE_PATH [OPTIONS]
echo.
echo Package a CMOD module for distribution
echo.
echo Arguments:
echo   MODULE_PATH      Path to the module directory
echo.
echo Options:
echo   -o, --output DIR     Output directory (default: current)
echo   --no-export          Do not generate [Export] block
echo   --sign               Sign the module package
echo   -h, --help           Show this help message
echo.
echo Examples:
echo   package-cmod.bat MyModule
echo   package-cmod.bat MyModule -o dist
echo   package-cmod.bat path\to\module --no-export --sign
echo.
echo Module structure:
echo   MyModule/
echo   ├── src/           # CHTL source files
echo   │   ├── MyModule.chtl
echo   │   └── components/
echo   └── info/          # Module information
echo       └── MyModule.chtl
echo.
echo Or with CMOD + CJMOD structure:
echo   MyModule/
echo   ├── CMOD/
echo   │   ├── src/
echo   │   └── info/
echo   └── CJMOD/
echo.
exit /b 0