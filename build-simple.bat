@echo off
echo ===================================
echo CHTL Simple Build
echo ===================================
echo.

cd /d "%~dp0"

REM Create directories
if not exist lib mkdir lib
if not exist target\classes mkdir target\classes
if not exist dist mkdir dist

REM Run fix programs
echo Running fixes...
javac FixFinalIssues.java 2>nul && java FixFinalIssues >nul 2>&1
javac FixCompilationErrors.java 2>nul && java FixCompilationErrors >nul 2>&1
javac FixAllCompilationIssues.java 2>nul && java FixAllCompilationIssues >nul 2>&1
javac FixFileNaming.java 2>nul && java FixFileNaming >nul 2>&1
javac FixRemainingCompilationErrors.java 2>nul && java FixRemainingCompilationErrors >nul 2>&1

REM Compile everything at once with lenient options
echo.
echo Compiling project...

REM Create file list using FOR loop which is more reliable on Windows
if exist all_files.txt del all_files.txt
set FILE_COUNT=0
for /r src\main\java %%f in (*.java) do (
    echo %%f >> all_files.txt
    set /a FILE_COUNT+=1
)

if %FILE_COUNT%==0 (
    echo [ERROR] No Java source files found in src\main\java!
    echo Please check that source files exist.
    pause
    exit /b 1
)

echo Found %FILE_COUNT% source files to compile...

javac -cp "lib\*" -d target\classes -encoding UTF-8 ^
    -source 17 -target 17 ^
    -Xlint:none ^
    -nowarn ^
    -XDignore.symbol.file ^
    @all_files.txt 2>compile.log

REM Check results
dir /s /b target\classes\*.class 2>nul | find /c ".class" > count.txt
set /p CLASSES=<count.txt
del count.txt

echo.
if "%CLASSES%"=="0" (
    echo [ERROR] No classes compiled!
    echo.
    echo First 20 errors:
    findstr "error:" compile.log 2>nul | more
) else (
    echo [SUCCESS] Compiled %CLASSES% classes
    
    REM Create JAR
    echo Creating JAR...
    cd target\classes
    jar cf ..\chtl-compiler.jar com\* 2>nul
    cd ..\..
    
    if exist target\chtl-compiler.jar (
        echo [SUCCESS] JAR created
        
        REM Create distribution
        copy target\chtl-compiler.jar dist\ >nul
        xcopy lib\*.jar dist\lib\ /Y /Q >nul
        
        echo.
        echo Build complete!
        echo Output: dist\chtl-compiler.jar
    )
)

del all_files.txt 2>nul
del compile.log 2>nul

echo.
pause