@echo off
echo Checking compilation errors...
echo.

if exist stage1_errors.txt (
    echo === Stage 1 Errors ===
    type stage1_errors.txt
    echo.
)

if exist stage2_errors.txt (
    echo === Stage 2 Errors ===
    type stage2_errors.txt
    echo.
)

if exist final_errors.txt (
    echo === Final Errors ===
    type final_errors.txt | findstr "error:" | more
    echo.
)

pause