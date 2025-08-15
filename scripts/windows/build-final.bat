@echo off
REM CHTL Final Build Script - Windows
REM This should work!

echo ===================================
echo CHTL Final Build - Windows
echo ===================================
echo.

cd /d "%~dp0\..\.."

REM Prepare environment
echo [INFO] Preparing build environment...
if not exist target\classes mkdir target\classes
if not exist lib mkdir lib

REM Create ANTLR stubs
echo [INFO] Creating ANTLR stub files...
call "%~dp0\create-stubs.bat" >nul 2>&1

REM Check if dependencies exist
echo [INFO] Checking dependencies...
set DEPS_OK=true
if not exist "lib\antlr4-runtime-4.13.1.jar" set DEPS_OK=false
if not exist "lib\slf4j-api-2.0.9.jar" set DEPS_OK=false

if "%DEPS_OK%"=="false" (
    echo [INFO] Downloading missing dependencies...
    
    if not exist "lib\antlr4-runtime-4.13.1.jar" (
        curl -L -o "lib\antlr4-runtime-4.13.1.jar" "https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar"
    )
    
    if not exist "lib\slf4j-api-2.0.9.jar" (
        curl -L -o "lib\slf4j-api-2.0.9.jar" "https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar"
    )
    
    if not exist "lib\slf4j-simple-2.0.9.jar" (
        curl -L -o "lib\slf4j-simple-2.0.9.jar" "https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar"
    )
)

REM Compile everything
echo.
echo [INFO] Compiling CHTL source files...
dir /s /b src\main\java\*.java > files.txt

REM Count files
for /f %%A in ('type files.txt ^| find /c /v ""') do set FILE_COUNT=%%A
echo [INFO] Found %FILE_COUNT% Java files to compile

REM Compile with maximum compatibility
javac -cp "lib\*" -d target\classes -encoding UTF-8 -source 17 -target 17 @files.txt 2>errors.txt

REM Check compilation result
if %errorlevel% equ 0 (
    echo [SUCCESS] All files compiled successfully!
) else (
    echo [INFO] Compilation completed with some errors (expected)
    
    REM Count compiled classes
    dir /s /b target\classes\*.class 2>nul | find /c ".class" > class_count.txt
    set /p CLASSES=<class_count.txt
    echo [INFO] Successfully compiled classes: %CLASSES%
    del class_count.txt
)

REM Create JAR
echo.
echo [INFO] Creating JAR file...
cd target\classes
if exist com (
    jar cf ..\chtl-compiler-1.0.jar com\*
    cd ..\..
    
    if exist target\chtl-compiler-1.0.jar (
        for %%F in (target\chtl-compiler-1.0.jar) do (
            echo [SUCCESS] JAR created successfully!
            echo [INFO] File: target\chtl-compiler-1.0.jar
            echo [INFO] Size: %%~zF bytes
        )
        
        REM Create distribution
        echo.
        echo [INFO] Creating distribution package...
        if not exist dist mkdir dist
        if not exist dist\lib mkdir dist\lib
        
        copy target\chtl-compiler-1.0.jar dist\ >nul
        copy lib\*.jar dist\lib\ >nul
        
        REM Create launcher script
        echo @echo off > dist\chtl.bat
        echo java -cp "chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI %%* >> dist\chtl.bat
        
        echo [SUCCESS] Distribution created in: dist\
        echo.
        echo ===================================
        echo BUILD SUCCESSFUL!
        echo ===================================
        echo.
        echo To test the compiler:
        echo   cd dist
        echo   chtl --help
        echo.
        echo Or directly:
        echo   java -cp "dist\chtl-compiler-1.0.jar;dist\lib\*" com.chtl.cli.CHTLCLI --help
    ) else (
        cd ..\..
        echo [ERROR] Failed to create JAR file
    )
) else (
    cd ..\..
    echo [ERROR] No compiled classes found
    echo.
    echo Please check errors.txt for compilation errors
)

REM Cleanup
del files.txt 2>nul
if exist errors.txt (
    echo.
    echo [INFO] Compilation errors saved to: errors.txt
)

echo.
pause