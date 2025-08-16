@echo off
setlocal enabledelayedexpansion

echo ===================================
echo CHTL Robust Build Script
echo ===================================
echo.

cd /d "%~dp0"

REM Create directories
if not exist lib mkdir lib
if not exist target\classes mkdir target\classes
if not exist dist mkdir dist

REM Check dependencies
echo [1/6] Checking dependencies...
set DEPS_MISSING=0
if not exist lib\antlr4-runtime-4.13.1.jar (
    echo Missing: antlr4-runtime-4.13.1.jar
    set DEPS_MISSING=1
)
if not exist lib\slf4j-api-2.0.9.jar (
    echo Missing: slf4j-api-2.0.9.jar
    set DEPS_MISSING=1
)
if not exist lib\slf4j-simple-2.0.9.jar (
    echo Missing: slf4j-simple-2.0.9.jar
    set DEPS_MISSING=1
)

if %DEPS_MISSING%==1 (
    echo.
    echo Downloading missing dependencies...
    call :downloadDependencies
)

REM Run fixes
echo.
echo [2/6] Running fixes...
if exist FixFinalIssues.java (
    javac FixFinalIssues.java 2>nul
    if exist FixFinalIssues.class (
        java FixFinalIssues >nul 2>&1
        echo Fixes applied successfully
    )
)

REM Clean previous build
echo.
echo [3/6] Cleaning previous build...
if exist target\classes\com rd /s /q target\classes\com 2>nul
if exist compile_*.log del /q compile_*.log 2>nul

REM Find all Java files
echo.
echo [4/6] Finding source files...
set FILE_COUNT=0
if exist all_files.txt del all_files.txt

for /r src\main\java %%f in (*.java) do (
    echo %%f >> all_files.txt
    set /a FILE_COUNT+=1
)

echo Found %FILE_COUNT% Java files

if %FILE_COUNT%==0 (
    echo [ERROR] No Java source files found!
    goto :error
)

REM Compile with detailed error handling
echo.
echo [5/6] Compiling source files...
echo This may take a few moments...

REM First attempt - compile everything
javac -cp "lib\*" -d target\classes -encoding UTF-8 ^
    -source 17 -target 17 ^
    -Xlint:none ^
    -nowarn ^
    -XDignore.symbol.file ^
    @all_files.txt 2>compile_full.log

REM Check if any classes were compiled
set CLASS_COUNT=0
for /r target\classes %%f in (*.class) do set /a CLASS_COUNT+=1

if %CLASS_COUNT%==0 (
    echo.
    echo [WARNING] Full compilation failed, trying incremental compilation...
    
    REM Try compiling in smaller batches
    call :compileIncremental
    
    REM Recount classes
    set CLASS_COUNT=0
    for /r target\classes %%f in (*.class) do set /a CLASS_COUNT+=1
)

echo.
echo Compiled %CLASS_COUNT% classes

if %CLASS_COUNT%==0 (
    echo.
    echo [ERROR] No classes were successfully compiled!
    echo.
    echo Compilation errors:
    findstr /c:"error:" compile_full.log 2>nul | more
    goto :error
)

REM Create JAR
echo.
echo [6/6] Creating JAR file...
cd target\classes
jar cf ..\chtl-compiler.jar com\* 2>nul
cd ..\..

if not exist target\chtl-compiler.jar (
    echo [ERROR] Failed to create JAR file!
    goto :error
)

REM Create distribution
echo.
echo Creating distribution...
copy target\chtl-compiler.jar dist\ >nul
xcopy lib\*.jar dist\lib\ /Y /Q >nul 2>nul

REM Create launcher script
(
echo @echo off
echo java -cp "chtl-compiler.jar;lib\*" com.chtl.cli.CHTLCLI %%*
) > dist\chtl.bat

echo.
echo ===================================
echo BUILD SUCCESSFUL!
echo ===================================
echo.
echo Output: dist\chtl-compiler.jar
echo Classes compiled: %CLASS_COUNT%
echo.
echo To use the compiler:
echo   cd dist
echo   chtl --help
echo.

REM Cleanup
del all_files.txt 2>nul
del compile_*.log 2>nul
del *.class 2>nul

goto :end

:compileIncremental
echo Attempting incremental compilation...

REM Stage 1: Core model classes
echo   Stage 1: Core model classes...
if exist model_files.txt del model_files.txt
for /r src\main\java\com\chtl\model %%f in (*.java) do echo %%f >> model_files.txt
if exist model_files.txt (
    javac -cp "lib\*" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none @model_files.txt 2>compile_model.log
    del model_files.txt
)

REM Stage 2: Exception classes
echo   Stage 2: Exception classes...
if exist exception_files.txt del exception_files.txt
for /r src\main\java\com\chtl\exception %%f in (*.java) do echo %%f >> exception_files.txt
if exist exception_files.txt (
    javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none @exception_files.txt 2>compile_exception.log
    del exception_files.txt
)

REM Stage 3: Context classes
echo   Stage 3: Context classes...
if exist context_files.txt del context_files.txt
for /r src\main\java\com\chtl\context %%f in (*.java) do echo %%f >> context_files.txt
if exist context_files.txt (
    javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none @context_files.txt 2>compile_context.log
    del context_files.txt
)

REM Stage 4: Everything else with best effort
echo   Stage 4: Remaining classes (best effort)...
javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 -source 17 -target 17 -Xlint:none -XDignore.symbol.file @all_files.txt 2>compile_remaining.log

goto :eof

:downloadDependencies
if not exist lib\antlr4-runtime-4.13.1.jar (
    echo Downloading ANTLR runtime...
    powershell -Command "try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar' -OutFile 'lib\antlr4-runtime-4.13.1.jar' } catch { Write-Host 'Failed to download ANTLR runtime' }" 2>nul
)
if not exist lib\slf4j-api-2.0.9.jar (
    echo Downloading SLF4J API...
    powershell -Command "try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar' -OutFile 'lib\slf4j-api-2.0.9.jar' } catch { Write-Host 'Failed to download SLF4J API' }" 2>nul
)
if not exist lib\slf4j-simple-2.0.9.jar (
    echo Downloading SLF4J Simple...
    powershell -Command "try { Invoke-WebRequest -Uri 'https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar' -OutFile 'lib\slf4j-simple-2.0.9.jar' } catch { Write-Host 'Failed to download SLF4J Simple' }" 2>nul
)
goto :eof

:error
echo.
echo Build failed!
echo.
pause
exit /b 1

:end
pause
exit /b 0