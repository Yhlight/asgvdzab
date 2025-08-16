@echo off
echo ===================================
echo CHTL Build Diagnostics
echo ===================================
echo.

cd /d "%~dp0"

echo Current directory: %cd%
echo.

echo Checking Java installation...
java -version 2>&1 | findstr "version"
javac -version 2>&1
echo.

echo Checking directory structure...
if exist src\main\java (
    echo [OK] src\main\java exists
) else (
    echo [ERROR] src\main\java NOT FOUND
)

if exist lib (
    echo [OK] lib directory exists
    dir lib\*.jar 2>nul | findstr ".jar"
) else (
    echo [ERROR] lib directory NOT FOUND
)
echo.

echo Counting Java source files...
set COUNT=0
for /r src\main\java %%f in (*.java) do set /a COUNT+=1
echo Total Java files found: %COUNT%
echo.

echo Testing file listing methods...
echo.
echo Method 1 - Using DIR /S /B:
dir /s /b src\main\java\*.java > test1.txt 2>nul
for /f %%A in ('type test1.txt ^| find /c /v ""') do echo Files found: %%A
echo First 5 files:
type test1.txt 2>nul | more +0
echo.

echo Method 2 - Using FOR /R:
set COUNT2=0
if exist test2.txt del test2.txt
for /r src\main\java %%f in (*.java) do (
    echo %%f >> test2.txt
    set /a COUNT2+=1
)
echo Files found: %COUNT2%
echo First 5 files:
type test2.txt 2>nul | more +0
echo.

echo Testing simple compilation...
if exist test1.txt (
    echo Compiling first file from list...
    for /f "usebackq tokens=*" %%f in (`type test1.txt ^| findstr /n "^" ^| findstr "^1:"`) do (
        set FIRSTFILE=%%f
        set FIRSTFILE=!FIRSTFILE:~2!
    )
    
    if defined FIRSTFILE (
        echo Compiling: !FIRSTFILE!
        javac -cp "lib\*" -d target\classes -encoding UTF-8 !FIRSTFILE! 2>&1
    )
) else (
    echo No file list available for test
)

echo.
echo Checking for fix programs...
for %%F in (Fix*.java) do (
    if exist %%F echo Found: %%F
)

echo.
echo Testing PowerShell availability...
powershell -Command "Write-Host 'PowerShell is available'" 2>nul
if %errorlevel% neq 0 (
    echo [WARNING] PowerShell not available or restricted
)

REM Cleanup
del test1.txt 2>nul
del test2.txt 2>nul

echo.
echo Diagnostics complete.
pause