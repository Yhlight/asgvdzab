@echo off
echo ===================================
echo CHTL Compilation Error Diagnosis
echo ===================================
echo.

cd /d "%~dp0"

echo Checking Java version...
java -version 2>&1 | findstr /i "version"
echo.

echo Checking compile errors...
if exist compile_errors.txt (
    echo Total errors in compile_errors.txt:
    findstr /c:"error:" compile_errors.txt | find /c /v "" 
    echo.
    echo First 50 errors:
    echo ================================
    type compile_errors.txt | findstr "error:" | more +0
    echo.
    echo ================================
    echo.
    echo To see all errors, open compile_errors.txt
) else (
    echo No compile_errors.txt found
)

echo.
pause