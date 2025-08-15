@echo off
REM CHTL Compiler Build Script - Windows Platform
REM Build compiler and package official modules

setlocal enabledelayedexpansion

REM Color definitions (Windows 10 support)
set "GREEN=[92m"
set "RED=[91m"
set "YELLOW=[93m"
set "NC=[0m"

REM Main process starts
echo %GREEN%===================================%NC%
echo %GREEN%CHTL Compiler Build Script - Windows%NC%
echo %GREEN%===================================%NC%
echo.

REM Switch to project root
cd /d "%~dp0\..\.."

REM Check environment
echo %GREEN%[INFO]%NC% Checking build environment...

REM Check Java
where java >nul 2>&1
if %errorlevel% neq 0 (
    echo %RED%[ERROR]%NC% Java not found, please install Java 17 or higher >&2
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
    echo %RED%[ERROR]%NC% Java 17 or higher required, current version: %JAVA_VERSION% >&2
    exit /b 1
)

REM Check Maven
where mvn >nul 2>&1
if %errorlevel% neq 0 (
    echo %RED%[ERROR]%NC% Maven not found, please install Maven 3.6+ >&2
    exit /b 1
)

echo %GREEN%[INFO]%NC% Environment check passed

REM Clean build directories
echo %GREEN%[INFO]%NC% Cleaning build directories...
if exist target rmdir /s /q target
if exist build rmdir /s /q build
if exist dist rmdir /s /q dist

REM Compile project
echo %GREEN%[INFO]%NC% Compiling CHTL compiler...
mvn clean package -DskipTests
if %errorlevel% neq 0 (
    echo %RED%[ERROR]%NC% Compilation failed >&2
    exit /b 1
)
echo %GREEN%[INFO]%NC% Compilation successful

REM Prepare modules
echo %GREEN%[INFO]%NC% Preparing official modules...

REM Create temporary module directory
set "TEMP_MODULE_DIR=build\modules"
mkdir "%TEMP_MODULE_DIR%" 2>nul

REM Copy official modules
if exist "src\main\java\com\chtl\module" (
    xcopy /s /e /y "src\main\java\com\chtl\module\*" "%TEMP_MODULE_DIR%\" >nul
)

REM Build Chtholly module (if exists)
if exist "%TEMP_MODULE_DIR%\Chtholly" (
    echo %GREEN%[INFO]%NC% Building Chtholly module...
    
    REM Compile CJMOD part
    if exist "%TEMP_MODULE_DIR%\Chtholly\CJMOD\src" (
        pushd "%TEMP_MODULE_DIR%\Chtholly\CJMOD"
        
        REM Create build directory
        mkdir build 2>nul
        
        REM Compile Java files
        dir /s /b src\*.java > java_files.txt
        javac -cp "..\..\..\..\..\target\chtl-compiler-1.0.jar" -d build @java_files.txt
        del java_files.txt
        
        REM Package as JAR
        jar cf chtholly-cjmod.jar -C build .
        
        popd
    )
)

echo %GREEN%[INFO]%NC% Module preparation complete

REM Create release package
echo %GREEN%[INFO]%NC% Creating release package...

REM Create release directory
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
    xcopy /s /e /y "target\lib\*" "%DIST_DIR%\lib\" >nul
)

REM Copy modules
if exist "%TEMP_MODULE_DIR%" (
    xcopy /s /e /y "%TEMP_MODULE_DIR%\*" "%DIST_DIR%\modules\" >nul
)

REM Create launcher script
echo @echo off > "%DIST_DIR%\bin\chtl.bat"
echo REM CHTL Compiler Launcher >> "%DIST_DIR%\bin\chtl.bat"
echo. >> "%DIST_DIR%\bin\chtl.bat"
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
echo REM CJMOD Management Tool >> "%DIST_DIR%\bin\cjmod.bat"
echo. >> "%DIST_DIR%\bin\cjmod.bat"
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
if exist "README.md" copy README.md "%DIST_DIR%\docs\" >nul
if exist "CHTL语法文档.md" copy "CHTL语法文档.md" "%DIST_DIR%\docs\" >nul

REM Create config file
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

echo %GREEN%[INFO]%NC% Release package created successfully

REM Package release file
echo %GREEN%[INFO]%NC% Packaging release file...

cd dist

REM Create ZIP package
powershell -Command "Compress-Archive -Path 'chtl-compiler-windows' -DestinationPath 'chtl-compiler-windows.zip' -Force"

REM Create installation script
(
echo @echo off
echo REM CHTL Compiler Installation Script
echo.
echo setlocal
echo.
echo set "INSTALL_DIR=C:\chtl"
echo.
echo echo Installing CHTL compiler to %%INSTALL_DIR%%...
echo.
echo REM Check administrator privileges
echo net session ^>nul 2^>^&1
echo if %%errorlevel%% neq 0 ^(
echo     echo Please run as administrator
echo     pause
echo     exit /b 1
echo ^)
echo.
echo REM Extract files
echo powershell -Command "Expand-Archive -Path 'chtl-compiler-windows.zip' -DestinationPath '.' -Force"
echo.
echo REM Create installation directory
echo if not exist "%%INSTALL_DIR%%" mkdir "%%INSTALL_DIR%%"
echo.
echo REM Copy files
echo xcopy /s /e /y "chtl-compiler-windows\*" "%%INSTALL_DIR%%\" ^>nul
echo.
echo REM Add to PATH
echo setx /M PATH "%%PATH%%;%%INSTALL_DIR%%\bin"
echo.
echo echo Installation complete!
echo echo Please restart command prompt to use 'chtl' command
echo pause
) > install.bat

cd ..

echo %GREEN%[INFO]%NC% Package complete: dist\chtl-compiler-windows.zip

REM Display build information
echo.
echo %GREEN%[INFO]%NC% Build information:
echo   Platform: Windows
echo   Build time: %date% %time%

if exist "dist\chtl-compiler-windows.zip" (
    for %%F in ("dist\chtl-compiler-windows.zip") do echo   Package size: %%~zF bytes
)

echo.
echo %GREEN%===================================%NC%
echo %GREEN%Build completed successfully!%NC%
echo %GREEN%===================================%NC%
echo.
echo Usage:
echo   1. Extract: Expand dist\chtl-compiler-windows.zip
echo   2. Install: Run install.bat as administrator
echo   3. Use: chtl input.chtl -o output\

endlocal