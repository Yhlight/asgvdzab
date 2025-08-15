@echo off
REM CHTL Compiler Simple Build Script - Windows
REM Skip ANTLR processing and compile directly

echo ===================================
echo CHTL Compiler Simple Build - Windows
echo ===================================
echo.

REM Switch to project root
cd /d "%~dp0\..\.."

REM Check environment
echo [INFO] Checking build environment...
java -version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Java not found, please install Java 17 or higher
    exit /b 1
)

echo [INFO] Environment check passed

REM Create directories
echo [INFO] Creating directories...
if not exist target\classes mkdir target\classes
if not exist lib mkdir lib

REM Create ANTLR stub files
echo [INFO] Creating ANTLR stub files...
call "%~dp0\create-stubs.bat" >nul 2>&1

REM Download dependencies if missing
if not exist "lib\antlr4-runtime-4.13.1.jar" (
    echo [INFO] Downloading ANTLR runtime...
    powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar' -OutFile 'lib\antlr4-runtime-4.13.1.jar'"
)

if not exist "lib\slf4j-api-2.0.9.jar" (
    echo [INFO] Downloading SLF4J API...
    powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar' -OutFile 'lib\slf4j-api-2.0.9.jar'"
)

if not exist "lib\logback-classic-1.4.11.jar" (
    echo [INFO] Downloading Logback Classic...
    powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/ch/qos/logback/logback-classic/1.4.11/logback-classic-1.4.11.jar' -OutFile 'lib\logback-classic-1.4.11.jar'"
)

if not exist "lib\logback-core-1.4.11.jar" (
    echo [INFO] Downloading Logback Core...
    powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/ch/qos/logback/logback-core/1.4.11/logback-core-1.4.11.jar' -OutFile 'lib\logback-core-1.4.11.jar'"
)

REM Set classpath
set CLASSPATH=target\classes;lib\antlr4-runtime-4.13.1.jar;lib\slf4j-api-2.0.9.jar;lib\logback-classic-1.4.11.jar;lib\logback-core-1.4.11.jar

REM Find all Java files
echo [INFO] Finding Java source files...
dir /s /b src\main\java\*.java > java_files.txt

REM Compile directly without ANTLR
echo [INFO] Compiling Java source files...
javac -cp "%CLASSPATH%" -d target\classes -encoding UTF-8 @java_files.txt 2>compile_errors.txt

if %errorlevel% equ 0 (
    echo [INFO] Compilation successful
    
    REM Create JAR
    echo [INFO] Creating JAR file...
    cd target\classes
    jar cf ..\chtl-compiler-1.0.jar com\*
    cd ..\..
    
    echo [INFO] JAR created: target\chtl-compiler-1.0.jar
    
    REM Create distribution
    echo [INFO] Creating distribution...
    if not exist dist mkdir dist
    copy target\chtl-compiler-1.0.jar dist\
    xcopy /s /e /q lib dist\lib\
    
    echo [INFO] Build completed successfully!
) else (
    echo [ERROR] Compilation failed
    echo First 20 errors:
    powershell -Command "Get-Content compile_errors.txt | Select-Object -First 20"
)

REM Cleanup
del java_files.txt

echo.
echo Build process completed
pause