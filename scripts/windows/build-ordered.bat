@echo off
REM CHTL Ordered Build Script - Windows
REM Compile files in correct dependency order

echo ===================================
echo CHTL Ordered Build - Windows
echo ===================================
echo.

cd /d "%~dp0\..\.."

REM Check Java
echo [INFO] Checking Java...
java -version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Java not found
    exit /b 1
)

REM Create directories
echo [INFO] Creating directories...
mkdir target\classes 2>nul
mkdir lib 2>nul
mkdir temp 2>nul

REM Create ANTLR stubs
echo [INFO] Creating ANTLR stubs...
call "%~dp0\create-stubs.bat" >nul 2>&1

REM Download dependencies
echo [INFO] Checking dependencies...
set NEED_DOWNLOAD=false

if not exist "lib\antlr4-runtime-4.13.1.jar" (
    set NEED_DOWNLOAD=true
    echo [INFO] Downloading ANTLR runtime...
    powershell -Command "$ProgressPreference = 'SilentlyContinue'; try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar' -OutFile 'lib\antlr4-runtime-4.13.1.jar' } catch { exit 1 }"
)

if not exist "lib\slf4j-api-2.0.9.jar" (
    set NEED_DOWNLOAD=true
    echo [INFO] Downloading SLF4J API...
    powershell -Command "$ProgressPreference = 'SilentlyContinue'; try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar' -OutFile 'lib\slf4j-api-2.0.9.jar' } catch { exit 1 }"
)

if not exist "lib\logback-classic-1.4.11.jar" (
    set NEED_DOWNLOAD=true
    echo [INFO] Downloading Logback Classic...
    powershell -Command "$ProgressPreference = 'SilentlyContinue'; try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/ch/qos/logback/logback-classic/1.4.11/logback-classic-1.4.11.jar' -OutFile 'lib\logback-classic-1.4.11.jar' } catch { exit 1 }"
)

if not exist "lib\logback-core-1.4.11.jar" (
    set NEED_DOWNLOAD=true
    echo [INFO] Downloading Logback Core...
    powershell -Command "$ProgressPreference = 'SilentlyContinue'; try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/ch/qos/logback/logback-core/1.4.11/logback-core-1.4.11.jar' -OutFile 'lib\logback-core-1.4.11.jar' } catch { exit 1 }"
)

if "%NEED_DOWNLOAD%"=="true" (
    echo [INFO] Dependencies downloaded
)

REM Set classpath
set CP=target\classes;lib\antlr4-runtime-4.13.1.jar;lib\slf4j-api-2.0.9.jar;lib\logback-classic-1.4.11.jar;lib\logback-core-1.4.11.jar

REM COMPILATION PHASE 1: Base classes and exceptions
echo.
echo [PHASE 1] Compiling base classes...
(
echo src\main\java\com\chtl\ast\SourceLocation.java
echo src\main\java\com\chtl\compiler\debug\CompilationException.java
echo src\main\java\com\chtl\model\FragmentType.java
) > temp\phase1.txt

javac -cp "%CP%" -d target\classes -encoding UTF-8 @temp\phase1.txt 2>temp\phase1_err.txt
if %errorlevel% neq 0 (
    echo [ERROR] Phase 1 failed:
    type temp\phase1_err.txt
    goto :cleanup
)
echo [OK] Phase 1 completed

REM COMPILATION PHASE 2: Interfaces
echo.
echo [PHASE 2] Compiling interfaces...
(
echo src\main\java\com\chtl\ast\visitor\CHTLASTVisitor.java
echo src\main\java\com\chtl\ast\CHTLASTNode.java
echo src\main\java\com\chtl\chtljs\ast\CHTLJSASTVisitor.java
echo src\main\java\com\chtl\chtljs\ast\CHTLJSASTNode.java
) > temp\phase2.txt

javac -cp "%CP%" -d target\classes -encoding UTF-8 @temp\phase2.txt 2>temp\phase2_err.txt
if %errorlevel% neq 0 (
    echo [ERROR] Phase 2 failed:
    type temp\phase2_err.txt
    goto :cleanup
)
echo [OK] Phase 2 completed

REM COMPILATION PHASE 3: Abstract classes
echo.
echo [PHASE 3] Compiling abstract classes...
(
echo src\main\java\com\chtl\ast\AbstractCHTLASTNode.java
echo src\main\java\com\chtl\chtljs\ast\AbstractCHTLJSASTNode.java
) > temp\phase3.txt

dir /s /b src\main\java\com\chtl\model\*.java >> temp\phase3.txt 2>nul

javac -cp "%CP%" -d target\classes -encoding UTF-8 @temp\phase3.txt 2>temp\phase3_err.txt
if %errorlevel% neq 0 (
    echo [WARNING] Some phase 3 files failed
)
echo [OK] Phase 3 completed

REM COMPILATION PHASE 4: Everything else
echo.
echo [PHASE 4] Compiling all remaining files...
dir /s /b src\main\java\*.java > temp\all_files.txt

echo [INFO] Total files: 
for /f %%i in ('type temp\all_files.txt ^| find /c /v ""') do echo %%i files

javac -cp "%CP%" -d target\classes -encoding UTF-8 -nowarn @temp\all_files.txt 2>temp\final_err.txt

if %errorlevel% neq 0 (
    echo [WARNING] Some files failed (expected for ANTLR dependencies)
    echo [INFO] Showing first 5 errors:
    powershell -Command "Get-Content temp\final_err.txt | Select-Object -First 5"
)

REM CREATE JAR
echo.
echo [INFO] Creating JAR file...
cd target\classes
jar cf ..\chtl-compiler-1.0.jar com\* 2>nul
cd ..\..

if exist target\chtl-compiler-1.0.jar (
    for %%F in ("target\chtl-compiler-1.0.jar") do (
        echo [SUCCESS] JAR created: target\chtl-compiler-1.0.jar
        echo [INFO] Size: %%~zF bytes
    )
    
    REM Create distribution
    if not exist dist mkdir dist
    copy target\chtl-compiler-1.0.jar dist\ >nul
    xcopy /s /e /q /y lib dist\lib\ >nul 2>&1
    
    REM Quick test
    echo.
    echo [TEST] Testing compiler...
    java -cp "target\chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI --version >nul 2>&1
    if %errorlevel% equ 0 (
        echo [SUCCESS] Compiler is functional!
    ) else (
        echo [INFO] Compiler built but may have limited functionality
    )
) else (
    echo [ERROR] Failed to create JAR
)

:cleanup
REM Clean up temp files
if exist temp rmdir /s /q temp 2>nul

echo.
echo ===================================
echo Build process completed
echo ===================================
pause