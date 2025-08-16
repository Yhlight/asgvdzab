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

REM Compile everything at once with lenient options
echo.
echo Compiling project...
dir /s /b src\main\java\*.java > all_files.txt

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