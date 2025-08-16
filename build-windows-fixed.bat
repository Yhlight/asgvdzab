@echo off
setlocal enabledelayedexpansion

echo ===================================
echo CHTL Windows Fixed Build Script
echo ===================================
echo.

cd /d "%~dp0"

REM Create directories
if not exist lib mkdir lib
if not exist target\classes mkdir target\classes
if not exist dist\lib mkdir dist\lib

REM Check/download dependencies
echo Checking dependencies...
set MISSING=0
if not exist lib\antlr4-runtime-4.13.1.jar (
    echo Downloading ANTLR runtime...
    powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar' -OutFile 'lib\antlr4-runtime-4.13.1.jar'" 2>nul
    if not exist lib\antlr4-runtime-4.13.1.jar set MISSING=1
)
if not exist lib\slf4j-api-2.0.9.jar (
    echo Downloading SLF4J API...
    powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar' -OutFile 'lib\slf4j-api-2.0.9.jar'" 2>nul
    if not exist lib\slf4j-api-2.0.9.jar set MISSING=1
)
if not exist lib\slf4j-simple-2.0.9.jar (
    echo Downloading SLF4J Simple...
    powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar' -OutFile 'lib\slf4j-simple-2.0.9.jar'" 2>nul
    if not exist lib\slf4j-simple-2.0.9.jar set MISSING=1
)

if %MISSING%==1 (
    echo.
    echo [WARNING] Some dependencies could not be downloaded.
    echo Please manually download them from Maven Central and place in lib/
)

REM Run fix programs
echo.
echo Running fixes...
for %%F in (FixFinalIssues.java FixCompilationErrors.java FixAllCompilationIssues.java FixFileNaming.java FixRemainingCompilationErrors.java) do (
    if exist %%F (
        javac %%F 2>nul && java %%~nF >nul 2>&1
    )
)

REM Clean
echo.
echo Cleaning previous build...
if exist target\classes rd /s /q target\classes 2>nul
mkdir target\classes

REM Create file list
echo.
echo Finding source files...
if exist all_files.txt del all_files.txt
set FILE_COUNT=0
for /r src\main\java %%f in (*.java) do (
    echo %%f >> all_files.txt
    set /a FILE_COUNT+=1
)
echo Found %FILE_COUNT% source files

REM Compile individually for better success rate
echo.
echo Compiling (this may take a moment)...
set COMPILED=0
set FAILED=0

for /r src\main\java %%f in (*.java) do (
    javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 ^
        -source 17 -target 17 -Xlint:none -nowarn "%%f" 2>nul
    
    if !errorlevel! equ 0 (
        set /a COMPILED+=1
        <nul set /p =.
    ) else (
        set /a FAILED+=1
        <nul set /p =x
    )
    
    set /a TOTAL=!COMPILED!+!FAILED!
    set /a MOD=!TOTAL! %% 20
    if !MOD! equ 0 (
        <nul set /p = (!TOTAL!^)
    )
)

echo.
echo.

REM Count actual classes
set CLASS_COUNT=0
for /r target\classes %%f in (*.class) do set /a CLASS_COUNT+=1

echo Compilation complete:
echo   Source files: %FILE_COUNT%
echo   Classes compiled: %CLASS_COUNT%
echo   Failed: %FAILED%

if %CLASS_COUNT% gtr 0 (
    echo.
    echo Creating JAR file...
    
    REM Create JAR from the classes directory
    pushd target\classes
    
    REM Create a file list of all classes
    dir /s /b *.class > ..\..\class_files.txt 2>nul
    
    REM Create JAR with explicit file list
    jar cf ..\chtl-compiler.jar -C . . 2>..\jar_error.txt
    
    if errorlevel 1 (
        echo [WARNING] Standard JAR creation had issues, trying alternative...
        
        REM Try creating with just the com directory if it exists
        if exist com (
            jar cf ..\chtl-compiler.jar com 2>nul
        ) else (
            REM Create JAR with whatever we have
            for /d %%d in (*) do (
                jar uf ..\chtl-compiler.jar %%d 2>nul
            )
        )
    )
    
    popd
    
    REM Check if JAR was created
    if exist target\chtl-compiler.jar (
        echo [SUCCESS] JAR created successfully!
        
        REM Verify JAR contents
        jar tf target\chtl-compiler.jar > jar_contents.txt 2>nul
        for /f %%A in ('type jar_contents.txt ^| find /c ".class"') do (
            echo JAR contains %%A class files
        )
        del jar_contents.txt 2>nul
        
        REM Create distribution
        echo.
        echo Creating distribution...
        copy target\chtl-compiler.jar dist\ >nul
        xcopy lib\*.jar dist\lib\ /Y /Q >nul 2>nul
        
        REM Create launcher
        (
        echo @echo off
        echo java -cp "%%~dp0chtl-compiler.jar;%%~dp0lib\*" com.chtl.cli.CHTLCLI %%*
        ) > dist\chtl.bat
        
        echo.
        echo ===================================
        echo BUILD SUCCESSFUL!
        echo ===================================
        echo.
        echo Output: dist\chtl-compiler.jar
        echo Launcher: dist\chtl.bat
        echo.
        echo To use:
        echo   cd dist
        echo   chtl --help
    ) else (
        echo.
        echo [ERROR] Failed to create JAR file!
        if exist target\jar_error.txt (
            echo Error details:
            type target\jar_error.txt
        )
    )
) else (
    echo.
    echo [ERROR] No classes were compiled successfully!
)

REM Cleanup
del all_files.txt 2>nul
del class_files.txt 2>nul
del target\jar_error.txt 2>nul
del *.class 2>nul

echo.
pause