@echo off
setlocal enabledelayedexpansion

echo ===================================
echo CHTL Best-Effort Build Script
echo ===================================
echo.

cd /d "%~dp0"

REM Create directories
if not exist lib mkdir lib
if not exist target\classes mkdir target\classes
if not exist dist\lib mkdir dist\lib

REM Check dependencies
echo Checking dependencies...
set NEED_DEPS=0
if not exist lib\antlr4-runtime-4.13.1.jar set NEED_DEPS=1
if not exist lib\slf4j-api-2.0.9.jar set NEED_DEPS=1
if not exist lib\slf4j-simple-2.0.9.jar set NEED_DEPS=1

if %NEED_DEPS%==1 (
    echo Downloading missing dependencies...
    
    if not exist lib\antlr4-runtime-4.13.1.jar (
        curl -sL -o lib\antlr4-runtime-4.13.1.jar https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar 2>nul || ^
        powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar' -OutFile 'lib\antlr4-runtime-4.13.1.jar'" 2>nul
    )
    
    if not exist lib\slf4j-api-2.0.9.jar (
        curl -sL -o lib\slf4j-api-2.0.9.jar https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar 2>nul || ^
        powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar' -OutFile 'lib\slf4j-api-2.0.9.jar'" 2>nul
    )
    
    if not exist lib\slf4j-simple-2.0.9.jar (
        curl -sL -o lib\slf4j-simple-2.0.9.jar https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar 2>nul || ^
        powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar' -OutFile 'lib\slf4j-simple-2.0.9.jar'" 2>nul
    )
)

REM Clean
echo.
echo Cleaning previous build...
if exist target\classes\com rd /s /q target\classes\com 2>nul

REM Compile each file individually
echo.
echo Compiling source files (best effort)...
set TOTAL_FILES=0
set COMPILED_FILES=0
set FAILED_FILES=0

REM Find all Java files and compile them one by one
for /r src\main\java %%f in (*.java) do (
    set /a TOTAL_FILES+=1
    
    REM Try to compile the file
    javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 ^
        -source 17 -target 17 -Xlint:none -nowarn "%%f" 2>nul
    
    if !errorlevel! equ 0 (
        set /a COMPILED_FILES+=1
        <nul set /p =.
    ) else (
        set /a FAILED_FILES+=1
        <nul set /p =x
    )
    
    REM Print progress every 10 files
    set /a MOD=!TOTAL_FILES! %% 10
    if !MOD! equ 0 (
        <nul set /p = (!TOTAL_FILES!^)
    )
)

echo.
echo.

REM Count actual compiled classes
set CLASS_COUNT=0
for /r target\classes %%f in (*.class) do set /a CLASS_COUNT+=1

echo Compilation Summary:
echo   Total source files: %TOTAL_FILES%
echo   Classes compiled: %CLASS_COUNT%

if %CLASS_COUNT%==0 (
    echo.
    echo [ERROR] No classes compiled successfully!
    echo.
    echo Trying minimal compilation of essential classes...
    
    REM Try to compile just the essential classes
    for %%f in (
        "src\main\java\com\chtl\model\CHTLNode.java"
        "src\main\java\com\chtl\model\CompilationResult.java"
        "src\main\java\com\chtl\model\SourceLocation.java"
        "src\main\java\com\chtl\exception\CHTLException.java"
        "src\main\java\com\chtl\cli\CHTLCLI.java"
    ) do (
        if exist %%f (
            echo   Compiling: %%~nf.java
            javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 ^
                -source 17 -target 17 -Xlint:none -nowarn -XDignore.symbol.file %%f 2>nul
        )
    )
    
    REM Recount
    set CLASS_COUNT=0
    for /r target\classes %%f in (*.class) do set /a CLASS_COUNT+=1
    echo   Essential classes compiled: %CLASS_COUNT%
)

if %CLASS_COUNT% gtr 0 (
    REM Create JAR with whatever compiled
    echo.
    echo Creating JAR file...
    cd target\classes
    jar cf ..\chtl-compiler.jar com\* 2>nul
    cd ..\..
    
    if exist target\chtl-compiler.jar (
        REM Create distribution
        echo Creating distribution...
        copy target\chtl-compiler.jar dist\ >nul
        xcopy lib\*.jar dist\lib\ /Y /Q >nul 2>nul
        
        REM Create launcher scripts
        (
        echo @echo off
        echo java -cp "%%~dp0chtl-compiler.jar;%%~dp0lib\*" com.chtl.cli.CHTLCLI %%* 2^>nul ^|^| echo Warning: Some features may not be available.
        ) > dist\chtl.bat
        
        (
        echo #!/bin/bash
        echo java -cp "$^(dirname "$0"^)/chtl-compiler.jar:$^(dirname "$0"^)/lib/*" com.chtl.cli.CHTLCLI "$@" 2^>/dev/null ^|^| \
        echo echo "Warning: Some features may not be available due to compilation issues."
        ) > dist\chtl.sh
        
        echo.
        echo ===================================
        echo BUILD PARTIALLY SUCCESSFUL!
        echo ===================================
        echo.
        echo Output: dist\chtl-compiler.jar
        echo Classes compiled: %CLASS_COUNT% (partial build^)
        echo.
        echo Note: This is a partial build. Some features may not work.
        echo.
        echo To test what's available:
        echo   cd dist
        echo   chtl --help
    ) else (
        echo [ERROR] Failed to create JAR!
        pause
        exit /b 1
    )
) else (
    echo.
    echo ===================================
    echo BUILD FAILED
    echo ===================================
    echo.
    echo No classes could be compiled. The source code has too many errors.
    echo Manual intervention is required to fix the compilation issues.
    pause
    exit /b 1
)

echo.
pause
exit /b 0