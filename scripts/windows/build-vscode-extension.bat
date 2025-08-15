@echo off
REM VSCode CHTL插件构建脚本 - Windows平台
REM 构建插件并打包编译器和模块

setlocal enabledelayedexpansion

REM 颜色定义
set "GREEN=[92m"
set "RED=[91m"
set "YELLOW=[93m"
set "NC=[0m"

REM 打印信息
:info
echo %GREEN%[INFO]%NC% %~1
goto :eof

:error
echo %RED%[ERROR]%NC% %~1 >&2
exit /b 1

:warning
echo %YELLOW%[WARN]%NC% %~1
goto :eof

echo ===================================
echo VSCode CHTL插件构建脚本 - Windows平台
echo ===================================
echo.

REM 切换到项目根目录
cd /d "%~dp0\..\.."

REM 检查环境
call :info "检查构建环境..."

REM 检查Node.js
where node >nul 2>&1
if %errorlevel% neq 0 (
    call :error "未找到Node.js，请安装Node.js 14+"
    exit /b 1
)

REM 检查Node版本
for /f "tokens=1 delims=v" %%i in ('node -v') do set NODE_VERSION_STR=%%i
for /f "tokens=1 delims=." %%i in ("%NODE_VERSION_STR%") do set NODE_VERSION=%%i

if %NODE_VERSION% lss 14 (
    call :error "需要Node.js 14或更高版本，当前版本：%NODE_VERSION_STR%"
    exit /b 1
)

REM 检查npm
where npm >nul 2>&1
if %errorlevel% neq 0 (
    call :error "未找到npm"
    exit /b 1
)

REM 检查vsce
where vsce >nul 2>&1
if %errorlevel% neq 0 (
    call :warning "未找到vsce，正在安装..."
    call npm install -g vsce
)

call :info "环境检查通过"

REM 构建编译器
call :info "构建CHTL编译器..."

REM 检查是否已经构建
if exist "dist\chtl-compiler-windows\lib\chtl-compiler.jar" (
    call :info "使用已存在的编译器构建"
) else (
    REM 运行编译器构建脚本
    if exist "scripts\windows\build-compiler.bat" (
        call "scripts\windows\build-compiler.bat"
    ) else (
        call :error "找不到编译器构建脚本"
        exit /b 1
    )
)

REM 准备插件资源
call :info "准备插件资源..."

pushd vscode-chtl

REM 创建资源目录
if not exist resources mkdir resources
if not exist resources\compiler mkdir resources\compiler
if not exist resources\modules mkdir resources\modules

REM 复制编译器
if exist "..\dist\chtl-compiler-windows" (
    call :info "复制编译器文件..."
    xcopy /s /e /y "..\dist\chtl-compiler-windows\*" "resources\compiler\" >nul
) else (
    call :error "编译器构建文件不存在"
    exit /b 1
)

REM 复制模块
if exist "..\dist\chtl-compiler-windows\modules" (
    call :info "复制官方模块..."
    xcopy /s /e /y "..\dist\chtl-compiler-windows\modules\*" "resources\modules\" >nul
)

REM 创建编译器包装脚本
(
echo const { spawn } = require('child_process'^);
echo const path = require('path'^);
echo const os = require('os'^);
echo.
echo /**
echo  * CHTL编译器包装器
echo  */
echo class CHTLCompilerWrapper {
echo     constructor(compilerPath^) {
echo         this.compilerPath = compilerPath;
echo         this.javaPath = this.findJava(^);
echo     }
echo.
echo     findJava(^) {
echo         // 尝试使用系统Java
echo         const javaHome = process.env.JAVA_HOME;
echo         if (javaHome^) {
echo             const javaPath = path.join(javaHome, 'bin', 'java'^);
echo             if (require('fs'^).existsSync(javaPath^)^) {
echo                 return javaPath;
echo             }
echo         }
echo         // 使用系统默认Java
echo         return 'java';
echo     }
echo.
echo     compile(inputFile, options = {}^) {
echo         return new Promise((resolve, reject^) =^> {
echo             const args = [
echo                 '-cp',
echo                 path.join(this.compilerPath, 'lib', '*'^),
echo                 'com.chtl.cli.CHTLCLI',
echo                 inputFile
echo             ];
echo.
echo             if (options.output^) {
echo                 args.push('-o', options.output^);
echo             }
echo.
echo             if (options.optimize^) {
echo                 args.push('--optimize'^);
echo             }
echo.
echo             if (options.sourcemap^) {
echo                 args.push('--sourcemap'^);
echo             }
echo.
echo             const process = spawn(this.javaPath, args, {
echo                 cwd: options.cwd ^|^| process.cwd(^)
echo             }^);
echo.
echo             let stdout = '';
echo             let stderr = '';
echo.
echo             process.stdout.on('data', (data^) =^> {
echo                 stdout += data.toString(^);
echo             }^);
echo.
echo             process.stderr.on('data', (data^) =^> {
echo                 stderr += data.toString(^);
echo             }^);
echo.
echo             process.on('close', (code^) =^> {
echo                 if (code === 0^) {
echo                     resolve({ stdout, stderr }^);
echo                 } else {
echo                     reject(new Error(`Compiler exited with code ${code}\n${stderr}`^)^);
echo                 }
echo             }^);
echo         }^);
echo     }
echo }
echo.
echo module.exports = CHTLCompilerWrapper;
) > resources\compiler\chtl-wrapper.js

popd

REM 安装依赖
call :info "安装依赖..."

pushd vscode-chtl
call npm install
popd

REM 编译TypeScript
call :info "编译TypeScript..."

pushd vscode-chtl
call npm run compile
popd

REM 打包插件
call :info "打包VSCode插件..."

pushd vscode-chtl

REM 更新package.json中的版本信息
for /f "tokens=2 delims=:," %%a in ('type package.json ^| findstr /i "\"version\""') do (
    set "EXTENSION_VERSION=%%~a"
    set "EXTENSION_VERSION=!EXTENSION_VERSION:~1,-1!"
    set "EXTENSION_VERSION=!EXTENSION_VERSION: =!"
)

REM 创建VSIX包
call vsce package --no-dependencies

REM 移动到dist目录
if not exist ..\dist mkdir ..\dist
move *.vsix ..\dist\ >nul

popd

call :info "插件打包完成: dist\chtl-%EXTENSION_VERSION%.vsix"

REM 创建安装说明
call :info "创建安装说明..."

(
echo # VSCode CHTL插件安装指南
echo.
echo ## 安装步骤
echo.
echo ### 方法1：通过VSCode界面安装
echo.
echo 1. 打开VSCode
echo 2. 按 `Ctrl+Shift+P` 打开命令面板
echo 3. 输入 `Extensions: Install from VSIX...`
echo 4. 选择下载的 `.vsix` 文件
echo 5. 重启VSCode
echo.
echo ### 方法2：通过命令行安装
echo.
echo ```bash
echo code --install-extension chtl-*.vsix
echo ```
echo.
echo ## 功能特性
echo.
echo - **语法高亮**：完整的CHTL语法支持
echo - **智能补全**：关键字、模板、函数自动补全
echo - **错误检查**：实时语法错误提示
echo - **代码格式化**：自动代码格式化
echo - **代码片段**：常用代码模板
echo - **内置编译器**：无需单独安装编译器
echo.
echo ## 使用方法
echo.
echo 1. 创建 `.chtl` 文件
echo 2. 编写CHTL代码
echo 3. 按 `Ctrl+Shift+B` 编译
echo 4. 或右键选择 "Compile CHTL"
echo.
echo ## 配置
echo.
echo 打开VSCode设置，搜索 "CHTL" 可以配置：
echo.
echo - `chtl.compiler.optimize`: 启用优化（默认：true）
echo - `chtl.compiler.sourcemap`: 生成源映射（默认：true）
echo - `chtl.compiler.outputDirectory`: 输出目录（默认：output）
echo.
echo ## 故障排除
echo.
echo ### Java未找到
echo.
echo 插件需要Java 17+运行。请确保：
echo.
echo 1. 已安装Java 17或更高版本
echo 2. 设置了JAVA_HOME环境变量
echo 3. 或将java添加到PATH
echo.
echo ### 编译失败
echo.
echo 1. 检查CHTL语法是否正确
echo 2. 查看输出面板的错误信息
echo 3. 确保有写入权限
echo.
echo ## 更新日志
echo.
echo 查看 [CHANGELOG.md](https://github.com/your-org/vscode-chtl/blob/main/CHANGELOG.md^)
echo.
echo ## 反馈
echo.
echo - Issues: https://github.com/your-org/vscode-chtl/issues
echo - Email: chtl-dev@example.com
) > dist\VSCODE_INSTALL.md

REM 清理临时文件
call :info "清理临时文件..."

pushd vscode-chtl

REM 删除打包时产生的临时文件
if exist resources\compiler rmdir /s /q resources\compiler
if exist resources\modules rmdir /s /q resources\modules

popd

REM 显示构建信息
call :info "构建信息："
echo   平台：Windows
echo   Node版本：%NODE_VERSION_STR%
for /f "tokens=*" %%i in ('npm -v') do set NPM_VERSION=%%i
echo   npm版本：%NPM_VERSION%
echo   构建时间：%date% %time%

REM 显示生成的文件
echo.
call :info "生成的文件："
dir dist\*.vsix 2>nul || echo   没有找到VSIX文件

echo.
call :info "构建完成！"
echo.
echo 安装方法：
echo   1. 在VSCode中: Ctrl+Shift+P -^> Extensions: Install from VSIX
echo   2. 命令行: code --install-extension dist\*.vsix

endlocal
exit /b 0