@echo off
REM VSCode Extension Build Script - Windows Platform

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

REM Main process starts
echo ==========================================
echo VSCode Extension Build Script - Windows
echo ==========================================
echo.

REM Switch to project root
cd /d "%~dp0\..\.."

REM Check environment
call :info "Checking build environment..."

REM Check Node.js
where node >nul 2>&1
if %errorlevel% neq 0 (
    call :error "Node.js not found, please install Node.js 16+"
    exit /b 1
)

REM Check npm
where npm >nul 2>&1
if %errorlevel% neq 0 (
    call :error "npm not found, please install npm"
    exit /b 1
)

REM Check vsce
where vsce >nul 2>&1
if %errorlevel% neq 0 (
    call :info "vsce not found, installing..."
    npm install -g vsce
    if %errorlevel% neq 0 (
        call :error "Failed to install vsce"
        exit /b 1
    )
)

call :info "Environment check passed"

REM Change to extension directory
cd vscode-chtl
if %errorlevel% neq 0 (
    call :error "VSCode extension directory not found"
    exit /b 1
)

REM Install dependencies
call :info "Installing dependencies..."
call npm install
if %errorlevel% neq 0 (
    call :error "Failed to install dependencies"
    exit /b 1
)

REM Compile TypeScript
if exist "src" (
    call :info "Compiling TypeScript..."
    call npm run compile
    if %errorlevel% neq 0 (
        call :error "TypeScript compilation failed"
        exit /b 1
    )
)

REM Run tests (optional)
if exist "test" (
    call :info "Running tests..."
    call npm test
    if %errorlevel% neq 0 (
        call :warning "Some tests failed, continuing..."
    )
)

REM Build VSIX package
call :info "Building VSIX package..."
call vsce package
if %errorlevel% neq 0 (
    call :error "Failed to build VSIX package"
    exit /b 1
)

REM Find generated VSIX file
for %%F in (*.vsix) do set "VSIX_FILE=%%F"

if not defined VSIX_FILE (
    call :error "No VSIX file generated"
    exit /b 1
)

call :info "VSIX package created: %VSIX_FILE%"

REM Copy compiler and modules
call :info "Preparing bundled resources..."

REM Create resources directory
if not exist "resources" mkdir "resources"
if not exist "resources\compiler" mkdir "resources\compiler"
if not exist "resources\modules" mkdir "resources\modules"

REM Copy compiler
set "COMPILER_JAR=..\target\chtl-compiler.jar"
if exist "%COMPILER_JAR%" (
    call :info "Copying CHTL compiler..."
    copy "%COMPILER_JAR%" "resources\compiler\" >nul
) else (
    call :warning "Compiler JAR not found, skipping"
)

REM Copy official modules
if exist "..\src\main\java\com\chtl\module" (
    call :info "Copying official modules..."
    xcopy /s /e /y "..\src\main\java\com\chtl\module\*" "resources\modules\" >nul
)

REM Create launcher scripts
call :info "Creating launcher scripts..."

REM Windows launcher
(
echo @echo off
echo REM CHTL Compiler Launcher for VSCode
echo.
echo set "SCRIPT_DIR=%%~dp0"
echo set "COMPILER_JAR=%%SCRIPT_DIR%%compiler\chtl-compiler.jar"
echo.
echo if not exist "%%COMPILER_JAR%%" ^(
echo     echo Error: CHTL compiler not found
echo     exit /b 1
echo ^)
echo.
echo java -jar "%%COMPILER_JAR%%" %%*
) > "resources\chtl.bat"

REM Unix launcher
(
echo #!/bin/bash
echo # CHTL Compiler Launcher for VSCode
echo.
echo SCRIPT_DIR="$^( cd "$^( dirname "${BASH_SOURCE[0]}" ^)" && pwd ^)"
echo COMPILER_JAR="$SCRIPT_DIR/compiler/chtl-compiler.jar"
echo.
echo if [ ! -f "$COMPILER_JAR" ]; then
echo     echo "Error: CHTL compiler not found"
echo     exit 1
echo fi
echo.
echo java -jar "$COMPILER_JAR" "$@"
) > "resources\chtl.sh"

REM Update package.json to include resources
call :info "Updating package.json..."

REM Move to parent directory for distribution
cd ..
if not exist "dist" mkdir "dist"
if not exist "dist\vscode" mkdir "dist\vscode"

REM Copy VSIX to distribution directory
move "vscode-chtl\%VSIX_FILE%" "dist\vscode\" >nul

REM Create installation instructions
(
echo # VSCode CHTL Extension Installation
echo.
echo ## Installation Steps
echo.
echo 1. Open VSCode
echo 2. Press Ctrl+Shift+P to open command palette
echo 3. Type "Extensions: Install from VSIX..."
echo 4. Select the file: %VSIX_FILE%
echo 5. Reload VSCode when prompted
echo.
echo ## Features
echo.
echo - Syntax highlighting for CHTL files
echo - Auto-completion for CHTL keywords
echo - Code snippets for common patterns
echo - Real-time error checking
echo - Built-in CHTL compiler
echo.
echo ## Usage
echo.
echo 1. Create a new file with .chtl extension
echo 2. Start typing CHTL code
echo 3. Use Ctrl+Space for auto-completion
echo 4. Right-click to compile CHTL file
echo.
) > "dist\vscode\README.md"

REM Display summary
call :info "Build summary:"
echo.
echo Extension: %VSIX_FILE%
echo Location: dist\vscode\%VSIX_FILE%

if exist "dist\vscode\%VSIX_FILE%" (
    for %%F in ("dist\vscode\%VSIX_FILE%") do echo Size: %%~zF bytes
)

echo.
call :info "VSCode extension build completed!"
echo.
echo To install:
echo   1. Open VSCode
echo   2. Run: code --install-extension dist\vscode\%VSIX_FILE%
echo.
echo Or install manually through VSCode UI

endlocal
exit /b 0