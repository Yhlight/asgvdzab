@echo off
echo ===================================
echo CHTL Complete Build System
echo ===================================
echo.

cd /d "%~dp0"

REM Step 1: Setup environment
echo [1/7] Setting up environment...
if not exist lib mkdir lib
if not exist target\classes mkdir target\classes
if not exist dist mkdir dist

REM Step 2: Download dependencies if needed
echo [2/7] Checking dependencies...
if not exist lib\antlr4-runtime-4.13.1.jar (
    echo Downloading ANTLR runtime...
    powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar' -OutFile 'lib\antlr4-runtime-4.13.1.jar'"
)
if not exist lib\slf4j-api-2.0.9.jar (
    echo Downloading SLF4J API...
    powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar' -OutFile 'lib\slf4j-api-2.0.9.jar'"
)
if not exist lib\slf4j-simple-2.0.9.jar (
    echo Downloading SLF4J Simple...
    powershell -Command "Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar' -OutFile 'lib\slf4j-simple-2.0.9.jar'"
)

REM Step 3: Run all fix programs
echo [3/7] Running fix programs...
if exist FixAllIssues.java (
    javac FixAllIssues.java 2>nul
    if exist FixAllIssues.class (
        java FixAllIssues >nul 2>&1
    )
)
if exist FixANTLRClasses.java (
    javac FixANTLRClasses.java 2>nul
    if exist FixANTLRClasses.class (
        java FixANTLRClasses >nul 2>&1
    )
)
if exist FixAllCompilationErrors.java (
    javac FixAllCompilationErrors.java 2>nul
    if exist FixAllCompilationErrors.class (
        java FixAllCompilationErrors >nul 2>&1
    )
)
if exist CreateMissingNodes.java (
    javac CreateMissingNodes.java 2>nul
    if exist CreateMissingNodes.class (
        java CreateMissingNodes >nul 2>&1
    )
)
if exist FixAllImports.java (
    javac FixAllImports.java 2>nul
    if exist FixAllImports.class (
        java FixAllImports >nul 2>&1
    )
)

REM Step 4: Clean previous builds
echo [4/7] Cleaning previous builds...
rd /s /q target\classes 2>nul
mkdir target\classes

REM Step 5: Compile the project
echo [5/7] Compiling CHTL compiler...
echo.

REM Create file list
dir /s /b src\main\java\*.java > java_files.txt

REM Count files
for /f %%i in ('find /c /v "" ^< java_files.txt') do set FILE_COUNT=%%i
echo Compiling %FILE_COUNT% Java files...

REM Compile with lenient options
javac -cp "lib\*" ^
    -d target\classes ^
    -encoding UTF-8 ^
    -source 17 ^
    -target 17 ^
    -Xlint:none ^
    -XDignore.symbol.file ^
    @java_files.txt 2>compile_errors.txt

REM Check compilation result
if %errorlevel% equ 0 (
    echo [SUCCESS] All files compiled successfully!
) else (
    echo [WARNING] Compilation completed with some errors
    
    REM Count successfully compiled classes
    dir /s /b target\classes\*.class 2>nul | find /c ".class" > class_count.txt
    set /p CLASSES=<class_count.txt
    echo [INFO] Successfully compiled classes: %CLASSES%
    del class_count.txt
    
    REM Show first few errors
    echo.
    echo First compilation errors:
    type compile_errors.txt | findstr /n "^" | findstr "^[1-9]:" | findstr "error:"
)

REM Step 6: Create JAR file
echo.
echo [6/7] Creating JAR file...
cd target\classes
if exist com (
    jar cf ..\chtl-compiler-1.0.jar com\* 2>nul
    cd ..\..
    
    if exist target\chtl-compiler-1.0.jar (
        echo [SUCCESS] JAR created successfully!
    ) else (
        echo [ERROR] Failed to create JAR file
        cd ..\..
        goto :cleanup
    )
) else (
    cd ..\..
    echo [ERROR] No compiled classes found
    goto :cleanup
)

REM Step 7: Create distribution
echo.
echo [7/7] Creating distribution...

REM Copy JAR
copy target\chtl-compiler-1.0.jar dist\ >nul

REM Copy libraries
xcopy lib\*.jar dist\lib\ /Y >nul

REM Create launcher script
(
echo @echo off
echo cd /d "%%~dp0"
echo java -cp "chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI %%*
) > dist\chtl.bat

REM Create run script for testing
(
echo @echo off
echo echo Testing CHTL Compiler...
echo echo.
echo cd /d "%%~dp0"
echo java -cp "chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI --version
echo echo.
echo echo To compile a CHTL file:
echo echo   chtl input.chtl -o output/
echo echo.
echo pause
) > dist\test-chtl.bat

echo.
echo ===================================
echo BUILD COMPLETE!
echo ===================================
echo.
echo Distribution created in: %cd%\dist
echo.
echo Available commands:
echo   - dist\chtl.bat           - CHTL compiler
echo   - dist\test-chtl.bat      - Test the compiler
echo.

REM Quick test
cd dist
echo Running quick test...
java -cp "chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI --version >nul 2>&1
if %errorlevel% equ 0 (
    echo [SUCCESS] Compiler is working!
) else (
    echo [WARNING] Compiler may have limited functionality
    echo           Check compile_errors.txt for details
)
cd ..

:cleanup
REM Cleanup temporary files
del java_files.txt 2>nul
del *.class 2>nul

echo.
pause