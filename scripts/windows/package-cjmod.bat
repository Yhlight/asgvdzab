@echo off
REM CJMOD Module Packaging Script - Windows Platform

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
echo ========================================
echo CJMOD Module Packaging Script - Windows
echo ========================================
echo.

call :info "Module path: %MODULE_PATH%"
call :info "Output directory: %OUTPUT_DIR%"

REM Check if module path exists
if not exist "%MODULE_PATH%" (
    call :error "Module path not found: %MODULE_PATH%"
    exit /b 1
)

REM Check CJMOD structure
set "HAS_CJMOD_STRUCTURE=false"
if exist "%MODULE_PATH%\CJMOD" set "HAS_CJMOD_STRUCTURE=true"
if exist "%MODULE_PATH%\CJmod" set "HAS_CJMOD_STRUCTURE=true"
if exist "%MODULE_PATH%\cjmod" set "HAS_CJMOD_STRUCTURE=true"

if "%HAS_CJMOD_STRUCTURE%"=="false" (
    REM Check if the current directory is already a CJMOD directory
    if exist "%MODULE_PATH%\src" (
        if exist "%MODULE_PATH%\info" (
            call :info "Detected CJMOD directory structure"
        ) else (
            call :error "Invalid CJMOD structure: missing info directory"
            exit /b 1
        )
    ) else (
        call :error "Invalid CJMOD structure: missing src directory"
        exit /b 1
    )
) else (
    REM Handle module with CMOD + CJMOD structure
    call :info "Detected CMOD + CJMOD structure"
    if exist "%MODULE_PATH%\CJMOD" (
        set "MODULE_PATH=%MODULE_PATH%\CJMOD"
    ) else if exist "%MODULE_PATH%\CJmod" (
        set "MODULE_PATH=%MODULE_PATH%\CJmod"
    ) else (
        set "MODULE_PATH=%MODULE_PATH%\cjmod"
    )
)

REM Read module information
call :read_module_info

REM Create working directory
set "WORK_DIR=%TEMP%\cjmod_pack_%RANDOM%"
mkdir "%WORK_DIR%" 2>nul

REM Copy module structure
call :info "Copying module structure..."
xcopy /s /e /y "%MODULE_PATH%\*" "%WORK_DIR%\" >nul

REM Compile Java code
if exist "%WORK_DIR%\src" (
    call :info "Compiling Java code..."
    
    REM Create class output directory
    mkdir "%WORK_DIR%\classes" 2>nul
    
    REM Find all Java files
    dir /s /b "%WORK_DIR%\src\*.java" > "%WORK_DIR%\java_files.txt"
    
    REM Check if there are Java files
    for %%F in ("%WORK_DIR%\java_files.txt") do (
        if %%~zF equ 0 (
            call :warning "No Java files found"
        ) else (
            REM Compile Java files
            javac -cp ".;%CHTL_HOME%\lib\*" -d "%WORK_DIR%\classes" @"%WORK_DIR%\java_files.txt"
            if !errorlevel! neq 0 (
                call :error "Java compilation failed"
                rmdir /s /q "%WORK_DIR%"
                exit /b 1
            )
            call :info "Java compilation successful"
        )
    )
    
    del "%WORK_DIR%\java_files.txt"
)

REM Create META-INF
mkdir "%WORK_DIR%\META-INF" 2>nul

REM Create manifest file
(
echo Manifest-Version: 1.0
echo Module-Type: CJMOD
echo Module-Name: %MODULE_NAME%
echo Module-Version: %MODULE_VERSION%
echo Build-Time: %date% %time%
echo Build-Platform: Windows
) > "%WORK_DIR%\META-INF\MANIFEST.MF"

REM Remove source files if not needed
if "%INCLUDE_SOURCE%"=="false" (
    call :info "Removing source files..."
    if exist "%WORK_DIR%\src" rmdir /s /q "%WORK_DIR%\src"
)

REM Package module
set "OUTPUT_FILE=%OUTPUT_DIR%\%MODULE_NAME%-%MODULE_VERSION%.cjmod"
call :info "Creating CJMOD package: %OUTPUT_FILE%"

REM Change to working directory for packaging
pushd "%WORK_DIR%"

REM Create JAR file
jar cf "%OUTPUT_FILE%" *
if %errorlevel% neq 0 (
    call :error "Failed to create CJMOD package"
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
call :info "CJMOD packaging completed!"
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
echo Usage: package-cjmod.bat MODULE_PATH [OPTIONS]
echo.
echo Package a CJMOD module for distribution
echo.
echo Arguments:
echo   MODULE_PATH      Path to the module directory
echo.
echo Options:
echo   -o, --output DIR     Output directory (default: current)
echo   --no-source          Exclude source files from package
echo   --sign               Sign the module package
echo   -h, --help           Show this help message
echo.
echo Examples:
echo   package-cjmod.bat MyModule
echo   package-cjmod.bat MyModule -o dist
echo   package-cjmod.bat path\to\module --no-source --sign
echo.
echo Module structure:
echo   MyModule/
echo   ├── src/           # Java source files
echo   │   ├── MyModule.java
echo   │   └── functions/
echo   └── info/          # Module information
echo       └── MyModule.chtl
echo.
echo Or with CMOD + CJMOD structure:
echo   MyModule/
echo   ├── CMOD/
echo   └── CJMOD/
echo       ├── src/
echo       └── info/
echo.
exit /b 0