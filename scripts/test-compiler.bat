@echo off
REM Test CHTL Compiler

echo ========================================
echo Testing CHTL Compiler
echo ========================================
echo.

REM Check if JAR exists
if not exist "target\chtl-compiler-1.0.jar" (
    echo [ERROR] Compiler JAR not found. Please build first.
    exit /b 1
)

REM Check if lib directory exists
if not exist "lib" (
    echo [ERROR] Library directory not found.
    exit /b 1
)

REM Set classpath
set CLASSPATH=target\chtl-compiler-1.0.jar;lib\*

REM Test 1: Show help
echo [TEST 1] Testing help command...
java -cp "%CLASSPATH%" com.chtl.cli.CHTLCLI --help
if %errorlevel% neq 0 (
    echo [ERROR] Help command failed
) else (
    echo [PASS] Help command succeeded
)
echo.

REM Test 2: Create a simple CHTL file
echo [TEST 2] Creating test file...
echo [Template] @Main { > test.chtl
echo     div { >> test.chtl
echo         text { "Hello, CHTL!" } >> test.chtl
echo     } >> test.chtl
echo } >> test.chtl

REM Test 3: Compile the test file
echo [TEST 3] Compiling test file...
java -cp "%CLASSPATH%" com.chtl.cli.CHTLCLI test.chtl -o test_output
if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed
) else (
    echo [PASS] Compilation succeeded
    if exist test_output\index.html (
        echo [INFO] Output file created: test_output\index.html
    )
)

REM Cleanup
del test.chtl 2>nul

echo.
echo ========================================
echo Test Summary:
echo - Help command: Tested
echo - Basic compilation: Tested
echo ========================================
pause