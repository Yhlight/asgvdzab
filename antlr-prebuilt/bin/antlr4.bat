@echo off
REM ANTLR4 预编译版本启动脚本 (Windows)
REM 版本: 4.13.1

set SCRIPT_DIR=%~dp0
set ANTLR_JAR=%SCRIPT_DIR%antlr4.jar

if not exist "%ANTLR_JAR%" (
    echo 错误: 未找到ANTLR4工具: %ANTLR_JAR%
    exit /b 1
)

REM 运行ANTLR4工具
java -jar "%ANTLR_JAR%" %*