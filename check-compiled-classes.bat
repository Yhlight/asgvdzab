@echo off
echo ===================================
echo Checking Compiled Classes
echo ===================================
echo.

cd /d "%~dp0"

echo Checking target\classes directory...
if not exist target\classes (
    echo [ERROR] target\classes directory does not exist
    pause
    exit /b 1
)

echo.
echo Class files in target\classes:
dir target\classes\*.class /s /b 2>nul > class_list.txt

if %errorlevel% neq 0 (
    echo [ERROR] No class files found
) else (
    for /f %%A in ('type class_list.txt ^| find /c /v ""') do echo Total class files: %%A
    echo.
    echo First 20 class files:
    type class_list.txt | more +0
)

echo.
echo Directory structure:
tree target\classes /f

echo.
echo Checking for com directory:
if exist target\classes\com (
    echo [OK] com directory exists
    dir target\classes\com /s /b | find /c ".class" > count.txt
    set /p COM_CLASSES=<count.txt
    echo Classes in com directory: %COM_CLASSES%
    del count.txt
) else (
    echo [WARNING] com directory does not exist
)

del class_list.txt 2>nul

echo.
pause