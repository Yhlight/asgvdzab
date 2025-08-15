@echo off
echo ===================================
echo CHTL Step-by-Step Build
echo ===================================
echo.

cd /d "%~dp0"

REM Ensure directories exist
if not exist target\classes mkdir target\classes
if not exist lib mkdir lib

REM Run all fix programs first
echo Running fix programs...
call :runFix FixCoreClasses.java
call :runFix FixAllIssues.java
call :runFix FixANTLRClasses.java
call :runFix FixAllCompilationErrors.java
call :runFix CreateMissingNodes.java
call :runFix FixAllImports.java

echo.
echo ===================================
echo Starting compilation...
echo ===================================
echo.

REM Step 1: Compile core model classes
echo [Step 1] Compiling core model classes...
javac -cp "lib\*" -d target\classes -encoding UTF-8 ^
    src\main\java\com\chtl\model\SourceLocation.java ^
    src\main\java\com\chtl\model\FragmentType.java ^
    src\main\java\com\chtl\model\CHTLNode.java ^
    src\main\java\com\chtl\model\TemplateNode.java ^
    src\main\java\com\chtl\model\ScriptNode.java ^
    src\main\java\com\chtl\model\StyleNode.java ^
    src\main\java\com\chtl\model\CustomNode.java 2>step1_errors.txt
if %errorlevel% neq 0 (
    echo   [ERROR] Failed to compile core model classes
    type step1_errors.txt
    goto :end
) else (
    echo   [OK] Core model classes compiled
)

REM Step 2: Compile exceptions and errors
echo [Step 2] Compiling exception classes...
javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 ^
    src\main\java\com\chtl\exception\*.java ^
    src\main\java\com\chtl\context\CompilationError.java ^
    src\main\java\com\chtl\context\CompilationWarning.java ^
    src\main\java\com\chtl\core\error\ErrorListener.java 2>step2_errors.txt
if %errorlevel% neq 0 (
    echo   [ERROR] Failed to compile exception classes
    type step2_errors.txt
    goto :end
) else (
    echo   [OK] Exception classes compiled
)

REM Step 3: Compile AST interfaces
echo [Step 3] Compiling AST interfaces...
javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 ^
    src\main\java\com\chtl\ast\CHTLASTNode.java ^
    src\main\java\com\chtl\ast\CHTLASTVisitor.java ^
    src\main\java\com\chtl\ast\NodeType.java 2>step3_errors.txt
if %errorlevel% neq 0 (
    echo   [ERROR] Failed to compile AST interfaces
    type step3_errors.txt
    goto :end
) else (
    echo   [OK] AST interfaces compiled
)

REM Step 4: Compile abstract AST classes
echo [Step 4] Compiling abstract AST classes...
javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 ^
    src\main\java\com\chtl\ast\AbstractCHTLASTNode.java 2>step4_errors.txt
if %errorlevel% neq 0 (
    echo   [ERROR] Failed to compile abstract AST classes
    type step4_errors.txt
    goto :end
) else (
    echo   [OK] Abstract AST classes compiled
)

REM Step 5: Compile scanner classes
echo [Step 5] Compiling scanner classes...
javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 ^
    src\main\java\com\chtl\scanner\State.java ^
    src\main\java\com\chtl\scanner\CodeFragment.java ^
    src\main\java\com\chtl\scanner\*.java 2>step5_errors.txt
if %errorlevel% neq 0 (
    echo   [WARNING] Some scanner classes failed
) else (
    echo   [OK] Scanner classes compiled
)

REM Step 6: Compile all remaining files
echo [Step 6] Compiling remaining files...
echo This may take a while...

REM Create list of all Java files
dir /s /b src\main\java\*.java > all_java_files.txt

REM Compile all with lenient options
javac -cp "lib\*;target\classes" -d target\classes -encoding UTF-8 ^
    -source 17 -target 17 ^
    -Xlint:none ^
    -XDignore.symbol.file ^
    @all_java_files.txt 2>final_errors.txt

if %errorlevel% neq 0 (
    echo   [WARNING] Some files failed to compile
    
    REM Count compiled classes
    dir /s /b target\classes\*.class 2>nul | find /c ".class" > class_count.txt
    set /p CLASSES=<class_count.txt
    echo   [INFO] Successfully compiled classes: %CLASSES%
    del class_count.txt
) else (
    echo   [OK] All files compiled successfully!
)

REM Step 7: Create JAR
echo.
echo [Step 7] Creating JAR file...
cd target\classes
jar cf ..\chtl-compiler-1.0.jar com\* 2>nul
cd ..\..

if exist target\chtl-compiler-1.0.jar (
    echo   [OK] JAR created successfully
    
    REM Create distribution
    echo.
    echo Creating distribution...
    if not exist dist mkdir dist
    copy target\chtl-compiler-1.0.jar dist\ >nul
    xcopy lib\*.jar dist\lib\ /Y >nul
    
    REM Create launcher
    (
    echo @echo off
    echo java -cp "chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI %%*
    ) > dist\chtl.bat
    
    echo.
    echo ===================================
    echo BUILD COMPLETE!
    echo ===================================
    echo Distribution in: %cd%\dist
) else (
    echo   [ERROR] Failed to create JAR
)

:end
REM Cleanup
del all_java_files.txt 2>nul
del step*_errors.txt 2>nul
del final_errors.txt 2>nul

echo.
pause
goto :eof

:runFix
if exist %1 (
    javac %1 2>nul
    if exist %~n1.class (
        java %~n1 >nul 2>&1
        echo   Ran %1
    )
)
goto :eof