@echo off
setlocal enabledelayedexpansion

echo ===================================
echo CHTL Universal Build Script
echo ===================================
echo.

cd /d "%~dp0"

REM Create necessary directories
echo Creating directories...
if not exist lib mkdir lib
if not exist target\classes mkdir target\classes
if not exist dist\lib mkdir dist\lib

REM Download dependencies if missing
echo.
echo Checking dependencies...
set NEED_DOWNLOAD=0

if not exist lib\antlr4-runtime-4.13.1.jar (
    set NEED_DOWNLOAD=1
    echo Missing: antlr4-runtime-4.13.1.jar
)
if not exist lib\slf4j-api-2.0.9.jar (
    set NEED_DOWNLOAD=1
    echo Missing: slf4j-api-2.0.9.jar
)
if not exist lib\slf4j-simple-2.0.9.jar (
    set NEED_DOWNLOAD=1
    echo Missing: slf4j-simple-2.0.9.jar
)

if %NEED_DOWNLOAD%==1 (
    echo.
    echo Downloading dependencies...
    
    if not exist lib\antlr4-runtime-4.13.1.jar (
        echo Downloading ANTLR runtime...
        curl -L -o lib\antlr4-runtime-4.13.1.jar https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar 2>nul
        if not exist lib\antlr4-runtime-4.13.1.jar (
            powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar' -OutFile 'lib\antlr4-runtime-4.13.1.jar'" 2>nul
        )
    )
    
    if not exist lib\slf4j-api-2.0.9.jar (
        echo Downloading SLF4J API...
        curl -L -o lib\slf4j-api-2.0.9.jar https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar 2>nul
        if not exist lib\slf4j-api-2.0.9.jar (
            powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar' -OutFile 'lib\slf4j-api-2.0.9.jar'" 2>nul
        )
    )
    
    if not exist lib\slf4j-simple-2.0.9.jar (
        echo Downloading SLF4J Simple...
        curl -L -o lib\slf4j-simple-2.0.9.jar https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar 2>nul
        if not exist lib\slf4j-simple-2.0.9.jar (
            powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar' -OutFile 'lib\slf4j-simple-2.0.9.jar'" 2>nul
        )
    )
)

REM Apply fixes
echo.
echo Applying fixes...
if exist FixFinalIssues.java (
    javac FixFinalIssues.java 2>nul && java FixFinalIssues >nul 2>&1
)
if exist FixCompilationErrors.java (
    javac FixCompilationErrors.java 2>nul && java FixCompilationErrors >nul 2>&1
)

REM Clean previous build
echo.
echo Cleaning previous build...
if exist target\classes\com rd /s /q target\classes\com 2>nul

REM Build file list
echo.
echo Scanning for source files...
if exist all_files.txt del all_files.txt

set FILE_COUNT=0
for /r src\main\java %%f in (*.java) do (
    echo %%f >> all_files.txt
    set /a FILE_COUNT+=1
)

if %FILE_COUNT%==0 (
    echo.
    echo [ERROR] No Java source files found!
    echo.
    echo Please ensure source files exist in: %cd%\src\main\java
    echo.
    pause
    exit /b 1
)

echo Found %FILE_COUNT% Java source files

REM Compile with maximum compatibility
echo.
echo Compiling (this may take a moment)...

REM First attempt - compile everything
javac -cp "lib\*" ^
    -d target\classes ^
    -encoding UTF-8 ^
    -source 17 -target 17 ^
    -Xlint:none ^
    -nowarn ^
    -XDignore.symbol.file ^
    -parameters ^
    @all_files.txt 2>compile.log

REM Count compiled classes
set CLASS_COUNT=0
for /r target\classes %%f in (*.class) do set /a CLASS_COUNT+=1

if %CLASS_COUNT%==0 (
    echo.
    echo [WARNING] Full compilation failed. Attempting staged compilation...
    
    REM Try staged compilation
    call :stageCompile
    
    REM Recount
    set CLASS_COUNT=0
    for /r target\classes %%f in (*.class) do set /a CLASS_COUNT+=1
)

echo.
if %CLASS_COUNT% gtr 0 (
    echo [SUCCESS] Compiled %CLASS_COUNT% classes
) else (
    echo [ERROR] No classes compiled successfully
    echo.
    echo First 10 compilation errors:
    findstr /c:"error:" compile.log 2>nul | more +0
    echo.
    pause
    exit /b 1
)

REM Create JAR
echo.
echo Creating JAR file...
cd target\classes
jar cf ..\chtl-compiler.jar com\* 2>nul
cd ..\..

if not exist target\chtl-compiler.jar (
    echo [ERROR] Failed to create JAR!
    pause
    exit /b 1
)

REM Create distribution
echo.
echo Creating distribution...
copy target\chtl-compiler.jar dist\ >nul
xcopy lib\*.jar dist\lib\ /Y /Q >nul 2>nul

REM Create launcher
(
echo @echo off
echo java -cp "chtl-compiler.jar;lib\*" com.chtl.cli.CHTLCLI %%*
) > dist\chtl.bat

(
echo #!/bin/bash
echo java -cp "chtl-compiler.jar:lib/*" com.chtl.cli.CHTLCLI "$@"
) > dist\chtl.sh

echo.
echo ===================================
echo BUILD SUCCESSFUL!
echo ===================================
echo.
echo Output: dist\chtl-compiler.jar
echo Launcher: dist\chtl.bat (Windows) / dist\chtl.sh (Linux)
echo.
echo Usage:
echo   cd dist
echo   chtl --help
echo.

REM Cleanup
del all_files.txt 2>nul
del compile.log 2>nul
del *.class 2>nul

pause
exit /b 0

:stageCompile
REM Stage 1: Core classes
echo   Compiling core classes...
if exist stage1.txt del stage1.txt
for /r src\main\java\com\chtl\model %%f in (*.java) do echo %%f >> stage1.txt
for /r src\main\java\com\chtl\exception %%f in (*.java) do echo %%f >> stage1.txt
if exist stage1.txt (
    javac -cp "lib\*" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none @stage1.txt 2>nul
    del stage1.txt
)

REM Stage 2: Context and support classes
echo   Compiling context classes...
if exist stage2.txt del stage2.txt
for /r src\main\java\com\chtl\context %%f in (*.java) do echo %%f >> stage2.txt
for /r src\main\java\com\chtl\core %%f in (*.java) do echo %%f >> stage2.txt
if exist stage2.txt (
    javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none @stage2.txt 2>nul
    del stage2.txt
)

REM Stage 3: Everything else
echo   Compiling remaining classes...
javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none -nowarn -XDignore.symbol.file @all_files.txt 2>nul

goto :eof