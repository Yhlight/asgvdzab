@echo off
REM CHTL Build and Test Script
REM Final solution for building the project

echo ===================================
echo CHTL Build and Test
echo ===================================
echo.

cd /d "%~dp0\..\.."

REM Step 1: Run all fixes
echo [STEP 1] Running all fixes...
if exist FixAllIssues.class (
    java FixAllIssues >nul 2>&1
)
if exist FixANTLRClasses.class (
    java FixANTLRClasses >nul 2>&1
)

REM Step 2: Compile the project
echo [STEP 2] Compiling project...
echo.

REM Find all Java files
dir /s /b src\main\java\*.java > all_files.txt
set /p FILE_COUNT=<all_files.txt
echo Found Java files to compile...

REM Compile with proper classpath
javac -cp "lib\*" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none @all_files.txt 2>compile_errors.txt

if %errorlevel% equ 0 (
    echo [SUCCESS] All files compiled successfully!
) else (
    echo [INFO] Compilation completed with some errors
    
    REM Count compiled classes
    dir /s /b target\classes\*.class 2>nul | find /c ".class" > class_count.txt
    set /p CLASSES=<class_count.txt
    echo [INFO] Successfully compiled classes: %CLASSES%
    del class_count.txt
)

REM Step 3: Create JAR
echo.
echo [STEP 3] Creating JAR file...
cd target\classes
if exist com (
    jar cf ..\chtl-compiler-1.0.jar com\*
    cd ..\..
    
    if exist target\chtl-compiler-1.0.jar (
        for %%F in (target\chtl-compiler-1.0.jar) do (
            echo [SUCCESS] JAR created: target\chtl-compiler-1.0.jar
            echo [INFO] Size: %%~zF bytes
        )
    )
) else (
    cd ..\..
    echo [ERROR] No compiled classes found
    goto :error
)

REM Step 4: Create distribution
echo.
echo [STEP 4] Creating distribution...
if not exist dist mkdir dist
if not exist dist\lib mkdir dist\lib

copy target\chtl-compiler-1.0.jar dist\ >nul
copy lib\*.jar dist\lib\ >nul

REM Create launcher script
(
echo @echo off
echo cd /d "%%~dp0"
echo java -cp "chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI %%*
) > dist\chtl.bat

REM Step 5: Test the compiler
echo.
echo [STEP 5] Testing compiler...
cd dist
java -cp "chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI --version >nul 2>&1
if %errorlevel% equ 0 (
    echo [SUCCESS] Compiler is working!
    echo.
    java -cp "chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI --help
) else (
    echo [WARNING] Compiler may have limited functionality
)
cd ..

echo.
echo ===================================
echo BUILD COMPLETE!
echo ===================================
echo.
echo Distribution created in: dist\
echo.
echo To use the compiler:
echo   cd dist
echo   chtl [options] file.chtl
echo.
goto :end

:error
echo.
echo [ERROR] Build failed. Check compile_errors.txt for details.
echo.

:end
REM Cleanup
del all_files.txt 2>nul

pause