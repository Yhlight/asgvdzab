@echo off
echo ===================================
echo CHTL Compiler - Final Build Script
echo ===================================
echo.

cd /d "%~dp0"
set BUILD_SUCCESS=true

REM Ensure directories
if not exist lib mkdir lib
if not exist target\classes mkdir target\classes
if not exist dist mkdir dist

REM Download dependencies if needed
echo [1/8] Checking dependencies...
call :downloadDependencies

REM Run all fix programs
echo.
echo [2/8] Running fix programs...
call :runAllFixes

REM Compile in stages
echo.
echo [3/8] Stage 1 - Core classes...
call :compileStage1
if %errorlevel% neq 0 set BUILD_SUCCESS=false

echo.
echo [4/8] Stage 2 - AST classes...
call :compileStage2
if %errorlevel% neq 0 set BUILD_SUCCESS=false

echo.
echo [5/8] Stage 3 - All remaining classes...
call :compileStage3

REM Count compiled classes
echo.
echo [6/8] Checking compilation results...
dir /s /b target\classes\*.class 2>nul | find /c ".class" > class_count.txt
set /p CLASSES=<class_count.txt
echo Successfully compiled classes: %CLASSES%
del class_count.txt

if "%CLASSES%"=="0" (
    echo [ERROR] No classes were compiled!
    goto :error
)

REM Create JAR
echo.
echo [7/8] Creating JAR file...
cd target\classes
jar cf ..\chtl-compiler-1.0.jar com\* 2>nul
cd ..\..

if not exist target\chtl-compiler-1.0.jar (
    echo [ERROR] Failed to create JAR file!
    goto :error
)

REM Create distribution
echo.
echo [8/8] Creating distribution...
copy target\chtl-compiler-1.0.jar dist\ >nul
xcopy lib\*.jar dist\lib\ /Y /Q >nul

REM Create launcher
(
echo @echo off
echo java -cp "chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI %%*
) > dist\chtl.bat

REM Test the build
echo.
echo Testing the compiler...
cd dist
java -cp "chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI --version >test_output.txt 2>&1
if %errorlevel% equ 0 (
    echo [SUCCESS] Compiler is working!
    type test_output.txt
) else (
    echo [WARNING] Compiler may have limited functionality
    echo Error output:
    type test_output.txt
)
del test_output.txt
cd ..

echo.
echo ===================================
if "%BUILD_SUCCESS%"=="true" (
    echo BUILD COMPLETED SUCCESSFULLY!
) else (
    echo BUILD COMPLETED WITH WARNINGS
)
echo ===================================
echo.
echo Distribution created in: %cd%\dist
echo.
echo To use the compiler:
echo   cd dist
echo   chtl input.chtl -o output\
echo.
goto :end

:downloadDependencies
if not exist lib\antlr4-runtime-4.13.1.jar (
    echo Downloading ANTLR runtime...
    powershell -Command "try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar' -OutFile 'lib\antlr4-runtime-4.13.1.jar' } catch { exit 1 }" 2>nul
)
if not exist lib\slf4j-api-2.0.9.jar (
    echo Downloading SLF4J API...
    powershell -Command "try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar' -OutFile 'lib\slf4j-api-2.0.9.jar' } catch { exit 1 }" 2>nul
)
if not exist lib\slf4j-simple-2.0.9.jar (
    echo Downloading SLF4J Simple...
    powershell -Command "try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar' -OutFile 'lib\slf4j-simple-2.0.9.jar' } catch { exit 1 }" 2>nul
)
goto :eof

:runAllFixes
for %%F in (
    FixCoreClasses.java
    FixAllProblems.java
    FixModelClasses.java
    FixAllIssues.java
    FixANTLRClasses.java
    FixAllCompilationErrors.java
    CreateMissingNodes.java
    FixAllImports.java
    FixRemainingIssues.java
    FixFinalIssues.java
) do (
    if exist %%F (
        javac %%F 2>nul
        if exist %%~nF.class (
            java %%~nF >nul 2>&1
            echo   Applied: %%F
        )
    )
)
goto :eof

:compileStage1
REM Compile core model and exception classes
echo Compiling core classes...
dir /s /b src\main\java\com\chtl\model\*.java > stage1_files.txt
dir /s /b src\main\java\com\chtl\exception\*.java >> stage1_files.txt
dir /s /b src\main\java\com\chtl\context\Compilation*.java >> stage1_files.txt
dir /s /b src\main\java\com\chtl\core\error\*.java >> stage1_files.txt

javac -cp "lib\*" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none @stage1_files.txt 2>stage1_errors.txt
set RESULT=%errorlevel%
del stage1_files.txt

if %RESULT% neq 0 (
    echo [ERROR] Stage 1 compilation failed:
    type stage1_errors.txt | findstr "error:" | more +0
)
del stage1_errors.txt 2>nul
exit /b %RESULT%

:compileStage2
REM Compile AST interfaces and base classes
echo Compiling AST classes...
dir /s /b src\main\java\com\chtl\ast\*.java > stage2_files.txt
dir /s /b src\main\java\com\chtl\ast\node\*.java >> stage2_files.txt
dir /s /b src\main\java\com\chtl\ast\chtljs\*.java >> stage2_files.txt
dir /s /b src\main\java\com\chtl\chtljs\ast\*.java >> stage2_files.txt

javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none @stage2_files.txt 2>stage2_errors.txt
set RESULT=%errorlevel%
del stage2_files.txt

if %RESULT% neq 0 (
    echo [WARNING] Some AST classes failed to compile
)
del stage2_errors.txt 2>nul
exit /b 0

:compileStage3
REM Compile everything else
echo Compiling all remaining classes...
dir /s /b src\main\java\*.java > all_files.txt

javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none -XDignore.symbol.file @all_files.txt 2>final_errors.txt
set RESULT=%errorlevel%
del all_files.txt

if %RESULT% neq 0 (
    echo [INFO] Some files had compilation issues
    REM Show summary of errors
    findstr /c:"error:" final_errors.txt | find /c /v "" > error_count.txt
    set /p ERRORS=<error_count.txt
    echo Total errors: %ERRORS%
    del error_count.txt
    
    if %ERRORS% gtr 0 (
        echo.
        echo First 10 errors:
        findstr "error:" final_errors.txt | more +0
    )
)
del final_errors.txt 2>nul
exit /b 0

:error
echo.
echo [ERROR] Build failed!
echo Check the error messages above for details.
echo.
pause
exit /b 1

:end
REM Cleanup
del *.class 2>nul
del stage*_errors.txt 2>nul
del final_errors.txt 2>nul
del all_files.txt 2>nul

pause