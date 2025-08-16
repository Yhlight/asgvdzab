@echo off
echo ===================================
echo CHTL Minimal Build Script
echo ===================================
echo.

cd /d "%~dp0"

REM Ensure directories exist
if not exist lib mkdir lib
if not exist target\classes mkdir target\classes
if not exist dist\lib mkdir dist\lib

REM Download dependencies if needed
echo Checking dependencies...
set DEPS_OK=1
if not exist lib\antlr4-runtime-4.13.1.jar set DEPS_OK=0
if not exist lib\slf4j-api-2.0.9.jar set DEPS_OK=0
if not exist lib\slf4j-simple-2.0.9.jar set DEPS_OK=0

if %DEPS_OK%==0 (
    echo Dependencies missing. Please download:
    echo - https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar
    echo - https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar
    echo - https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar
    echo And place them in the lib directory.
    echo.
    echo Attempting automatic download...
    powershell -Command "& { try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar' -OutFile 'lib\antlr4-runtime-4.13.1.jar' } catch { } }"
    powershell -Command "& { try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar' -OutFile 'lib\slf4j-api-2.0.9.jar' } catch { } }"
    powershell -Command "& { try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar' -OutFile 'lib\slf4j-simple-2.0.9.jar' } catch { } }"
)

REM Clean
echo.
echo Cleaning previous build...
if exist target\classes rd /s /q target\classes 2>nul
mkdir target\classes

REM Compile only the most essential files
echo.
echo Compiling essential classes...
set COMPILED=0

REM List of essential files to try
set FILES_TO_COMPILE=src\main\java\com\chtl\model\CHTLNode.java
set FILES_TO_COMPILE=%FILES_TO_COMPILE% src\main\java\com\chtl\model\SourceLocation.java
set FILES_TO_COMPILE=%FILES_TO_COMPILE% src\main\java\com\chtl\model\CompilationResult.java
set FILES_TO_COMPILE=%FILES_TO_COMPILE% src\main\java\com\chtl\exception\CHTLException.java

for %%f in (%FILES_TO_COMPILE%) do (
    if exist %%f (
        echo Compiling %%~nxf...
        javac -cp "lib\*" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none %%f 2>nul
        if not errorlevel 1 set /a COMPILED+=1
    )
)

echo.
echo Compiling all other files (ignoring errors)...
for /r src\main\java %%f in (*.java) do (
    javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none -nowarn "%%f" 2>nul
)

REM Count classes
set /a COUNT=0
for /r target\classes %%f in (*.class) do set /a COUNT+=1

echo.
echo Compiled %COUNT% classes

if %COUNT% gtr 0 (
    echo.
    echo Creating JAR file...
    
    REM Create a manifest file
    echo Manifest-Version: 1.0 > target\MANIFEST.MF
    echo Main-Class: com.chtl.cli.CHTLCLI >> target\MANIFEST.MF
    echo. >> target\MANIFEST.MF
    
    REM Create JAR using the jar command with all files
    cd target
    jar cfm chtl-compiler.jar MANIFEST.MF -C classes . 2>nul
    
    if not exist chtl-compiler.jar (
        echo Trying without manifest...
        jar cf chtl-compiler.jar -C classes . 2>nul
    )
    
    cd ..
    
    if exist target\chtl-compiler.jar (
        echo [SUCCESS] JAR created!
        
        REM Create distribution
        echo.
        echo Creating distribution...
        copy target\chtl-compiler.jar dist\ >nul
        xcopy lib\*.jar dist\lib\ /Y /Q >nul 2>nul
        
        REM Create simple launcher
        echo @echo off > dist\chtl.bat
        echo java -cp "%%~dp0chtl-compiler.jar;%%~dp0lib\*" com.chtl.cli.CHTLCLI %%* >> dist\chtl.bat
        
        echo.
        echo ===================================
        echo BUILD SUCCESSFUL!
        echo ===================================
        echo.
        echo JAR file: dist\chtl-compiler.jar
        echo Classes compiled: %COUNT%
        echo.
        echo Note: This is a minimal build. Many features may not work.
    ) else (
        echo [ERROR] Failed to create JAR file
        echo.
        echo Checking what was compiled...
        dir target\classes /s /b | find ".class"
    )
) else (
    echo.
    echo [ERROR] No classes were compiled successfully
)

echo.
pause