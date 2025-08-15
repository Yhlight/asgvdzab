@echo off
REM CHTL Compiler Build Script - Windows Platform
REM Build compiler and package official modules

setlocal enabledelayedexpansion

REM Color definitions (Windows 10 support)
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
echo ===================================
echo CHTL Compiler Build Script - Windows
echo ===================================
echo.

REM Switch to project root
cd /d "%~dp0\..\.."

REM Check environment
call :info "Checking build environment..."

REM Check Java
where java >nul 2>&1
if %errorlevel% neq 0 (
    call :error "Java not found, please install Java 17 or higher"
    exit /b 1
)

REM Check Java version
for /f "tokens=3" %%i in ('java -version 2^>^&1 ^| findstr /i "version"') do (
    set JAVA_VERSION_STR=%%i
)
set JAVA_VERSION_STR=%JAVA_VERSION_STR:"=%
for /f "delims=. tokens=1" %%i in ("%JAVA_VERSION_STR%") do (
    set JAVA_VERSION=%%i
)

if %JAVA_VERSION% lss 17 (
    call :error "Java 17 or higher required, current version: %JAVA_VERSION%"
    exit /b 1
)

REM Check Maven
where mvn >nul 2>&1
if %errorlevel% neq 0 (
    call :error "Maven not found, please install Maven 3.6+"
    exit /b 1
)

call :info "Environment check passed"

REM Clean build directories
call :info "Cleaning build directories..."
if exist target rmdir /s /q target
if exist build rmdir /s /q build
if exist dist rmdir /s /q dist

REM Compile project
call :info "Compiling CHTL compiler..."
call mvn clean package -DskipTests
if %errorlevel% neq 0 (
    call :error "Compilation failed"
    exit /b 1
)
call :info "Compilation successful"

REM Prepare modules
call :info "Preparing official modules..."

REM Create temporary module directory
set "TEMP_MODULE_DIR=build\modules"
mkdir "%TEMP_MODULE_DIR%" 2>nul

REM Copy official modules
if exist "src\main\java\com\chtl\module" (
    xcopy /s /e /y "src\main\java\com\chtl\module\*" "%TEMP_MODULE_DIR%\" >nul
)

REM Build Chtholly module (if exists)
if exist "%TEMP_MODULE_DIR%\Chtholly" (
    call :info "Building Chtholly module..."
    
    REM Compile CJMOD part
    if exist "%TEMP_MODULE_DIR%\Chtholly\CJMOD\src" (
        pushd "%TEMP_MODULE_DIR%\Chtholly\CJMOD"
        
        REM Create build directory
        mkdir build 2>nul
        
        REM Compile Java files
        dir /s /b src\*.java > java_files.txt
        javac -cp "..\..\..\..\..\target\chtl-compiler.jar" -d build @java_files.txt
        del java_files.txt
        
        REM Package as JAR
        jar cf chtholly-cjmod.jar -C build .
        
        popd
    )
)

call :info "Module preparation complete"

REM Create release package
call :info "Creating release package..."

REM Create distribution directory
set "DIST_DIR=dist\chtl-compiler-windows"
mkdir "%DIST_DIR%\bin" 2>nul
mkdir "%DIST_DIR%\lib" 2>nul
mkdir "%DIST_DIR%\modules" 2>nul
mkdir "%DIST_DIR%\config" 2>nul
mkdir "%DIST_DIR%\docs" 2>nul

REM Copy compiler JAR
copy target\chtl-compiler*.jar "%DIST_DIR%\lib\chtl-compiler.jar" >nul

REM Copy dependencies
if exist "target\lib" (
    xcopy /y "target\lib\*.jar" "%DIST_DIR%\lib\" >nul
)

REM Copy modules
if exist "build\modules" (
    xcopy /s /e /y "build\modules\*" "%DIST_DIR%\modules\" >nul
)

REM Create launch script
echo @echo off > "%DIST_DIR%\bin\chtl.bat"
echo REM CHTL Compiler Launch Script >> "%DIST_DIR%\bin\chtl.bat"
echo. >> "%DIST_DIR%\bin\chtl.bat"
echo setlocal >> "%DIST_DIR%\bin\chtl.bat"
echo set "SCRIPT_DIR=%%~dp0" >> "%DIST_DIR%\bin\chtl.bat"
echo set "BASE_DIR=%%SCRIPT_DIR%%\.." >> "%DIST_DIR%\bin\chtl.bat"
echo. >> "%DIST_DIR%\bin\chtl.bat"
echo REM Set classpath >> "%DIST_DIR%\bin\chtl.bat"
echo set "CLASSPATH=%%BASE_DIR%%\lib\*" >> "%DIST_DIR%\bin\chtl.bat"
echo. >> "%DIST_DIR%\bin\chtl.bat"
echo REM Set module path >> "%DIST_DIR%\bin\chtl.bat"
echo set "CHTL_MODULE_PATH=%%BASE_DIR%%\modules" >> "%DIST_DIR%\bin\chtl.bat"
echo. >> "%DIST_DIR%\bin\chtl.bat"
echo REM Run compiler >> "%DIST_DIR%\bin\chtl.bat"
echo java -cp "%%CLASSPATH%%" com.chtl.cli.CHTLCLI %%* >> "%DIST_DIR%\bin\chtl.bat"

REM Create cjmod command script
echo @echo off > "%DIST_DIR%\bin\cjmod.bat"
echo REM CJMOD Management Tool Launch Script >> "%DIST_DIR%\bin\cjmod.bat"
echo. >> "%DIST_DIR%\bin\cjmod.bat"
echo setlocal >> "%DIST_DIR%\bin\cjmod.bat"
echo set "SCRIPT_DIR=%%~dp0" >> "%DIST_DIR%\bin\cjmod.bat"
echo set "BASE_DIR=%%SCRIPT_DIR%%\.." >> "%DIST_DIR%\bin\cjmod.bat"
echo. >> "%DIST_DIR%\bin\cjmod.bat"
echo REM Set classpath >> "%DIST_DIR%\bin\cjmod.bat"
echo set "CLASSPATH=%%BASE_DIR%%\lib\*" >> "%DIST_DIR%\bin\cjmod.bat"
echo. >> "%DIST_DIR%\bin\cjmod.bat"
echo REM Set module path >> "%DIST_DIR%\bin\cjmod.bat"
echo set "CHTL_MODULE_PATH=%%BASE_DIR%%\modules" >> "%DIST_DIR%\bin\cjmod.bat"
echo. >> "%DIST_DIR%\bin\cjmod.bat"
echo REM Run CJMOD CLI >> "%DIST_DIR%\bin\cjmod.bat"
echo java -cp "%%CLASSPATH%%" com.chtl.cli.CJmodCLI %%* >> "%DIST_DIR%\bin\cjmod.bat"

REM Copy documentation
copy README.md "%DIST_DIR%\docs\" >nul
copy BUILD_REQUIREMENTS.md "%DIST_DIR%\docs\" >nul

REM Create configuration file
(
echo # CHTL Compiler Configuration
echo.
echo # Default output directory
echo output.dir=output
echo.
echo # Compilation options
echo compile.optimize=true
echo compile.sourcemap=true
echo compile.minify=false
echo.
echo # Module path
echo module.path=${CHTL_HOME}/modules
echo.
echo # Log level
echo log.level=INFO
) > "%DIST_DIR%\config\chtl.conf"

call :info "Release package created successfully"

REM Package release files
call :info "Packaging release files..."

cd dist

REM Create ZIP package
powershell -Command "Compress-Archive -Path 'chtl-compiler-windows' -DestinationPath 'chtl-compiler-windows.zip' -Force"

REM Create installation script
(
echo @echo off
echo REM CHTL Compiler Installation Script
echo.
echo set INSTALL_DIR=C:\Program Files\CHTL
echo.
echo echo Installing CHTL compiler to %%INSTALL_DIR%% ...
echo.
echo REM Check administrator privileges
echo net session ^>nul 2^>^&1
echo if %%errorlevel%% neq 0 ^(
echo     echo Please run this script as administrator
echo     pause
echo     exit /b 1
echo ^)
echo.
echo REM Extract files
echo echo Extracting files...
echo powershell -Command "Expand-Archive -Path 'chtl-compiler-windows.zip' -DestinationPath '.' -Force"
echo.
echo REM Create installation directory
echo if not exist "%%INSTALL_DIR%%" mkdir "%%INSTALL_DIR%%"
echo.
echo REM Copy files
echo xcopy /s /e /y "chtl-compiler-windows\*" "%%INSTALL_DIR%%\" ^>nul
echo.
echo REM Add to PATH
echo echo Adding to system PATH...
echo setx /M PATH "%%PATH%%;%%INSTALL_DIR%%\bin"
echo.
echo echo Installation complete!
echo echo Please restart Command Prompt and run 'chtl --version' to verify installation
echo pause
) > install.bat

cd ..

call :info "Packaging complete: dist\chtl-compiler-windows.zip"

REM Display build information
call :info "Build information:"
echo   Platform: Windows
echo   Java version: %JAVA_VERSION%
echo   Build time: %date% %time%

if exist "dist\chtl-compiler-windows.zip" (
    for %%F in ("dist\chtl-compiler-windows.zip") do echo   Package size: %%~zF bytes
)

echo.
call :info "Build completed!"
echo.
echo Usage:
echo   1. Extract: Unzip dist\chtl-compiler-windows.zip
echo   2. Install: Run dist\install.bat as administrator
echo   3. Use: chtl input.chtl -o output\

endlocal
exit /b 0