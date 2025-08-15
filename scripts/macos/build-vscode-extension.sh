#!/bin/bash

# VSCode CHTL插件构建脚本 - macOS平台
# 构建插件并打包编译器和模块

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 打印信息
info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
    exit 1
}

warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

# 检查环境
check_environment() {
    info "检查构建环境..."
    
    # 检查Node.js
    if ! command -v node &> /dev/null; then
        # 尝试通过Homebrew安装
        if command -v brew &> /dev/null; then
            warning "未找到Node.js，尝试通过Homebrew安装..."
            brew install node
        else
            error "未找到Node.js，请安装Node.js 14+"
        fi
    fi
    
    NODE_VERSION=$(node -v | sed 's/v\([0-9]*\).*/\1/')
    if [ "$NODE_VERSION" -lt 14 ]; then
        error "需要Node.js 14或更高版本，当前版本：$(node -v)"
    fi
    
    # 检查npm
    if ! command -v npm &> /dev/null; then
        error "未找到npm"
    fi
    
    # 检查vsce
    if ! command -v vsce &> /dev/null; then
        warning "未找到vsce，正在安装..."
        npm install -g vsce
    fi
    
    info "环境检查通过"
}

# 构建编译器
build_compiler() {
    info "构建CHTL编译器..."
    
    # 检查是否已经构建
    if [ -f "dist/chtl-compiler-macos/lib/chtl-compiler.jar" ]; then
        info "使用已存在的编译器构建"
        return
    fi
    
    # 运行编译器构建脚本
    if [ -f "scripts/macos/build-compiler.sh" ]; then
        bash scripts/macos/build-compiler.sh
    else
        error "找不到编译器构建脚本"
    fi
}

# 准备插件资源
prepare_extension_resources() {
    info "准备插件资源..."
    
    cd vscode-chtl
    
    # 创建资源目录
    mkdir -p resources/{compiler,modules}
    
    # 复制编译器
    if [ -d "../dist/chtl-compiler-macos" ]; then
        info "复制编译器文件..."
        cp -r ../dist/chtl-compiler-macos/* resources/compiler/
    else
        error "编译器构建文件不存在"
    fi
    
    # 复制模块
    if [ -d "../dist/chtl-compiler-macos/modules" ]; then
        info "复制官方模块..."
        cp -r ../dist/chtl-compiler-macos/modules/* resources/modules/
    fi
    
    # 创建编译器包装脚本
    cat > resources/compiler/chtl-wrapper.js << 'EOF'
const { spawn } = require('child_process');
const path = require('path');
const os = require('os');

/**
 * CHTL编译器包装器
 */
class CHTLCompilerWrapper {
    constructor(compilerPath) {
        this.compilerPath = compilerPath;
        this.javaPath = this.findJava();
    }

    findJava() {
        // macOS特定：检查java_home
        if (os.platform() === 'darwin') {
            try {
                const { execSync } = require('child_process');
                const javaHome = execSync('/usr/libexec/java_home -v 17 2>/dev/null || /usr/libexec/java_home').toString().trim();
                if (javaHome) {
                    const javaPath = path.join(javaHome, 'bin', 'java');
                    if (require('fs').existsSync(javaPath)) {
                        return javaPath;
                    }
                }
            } catch (e) {
                // 忽略错误，尝试其他方法
            }
        }
        
        // 尝试使用系统Java
        const javaHome = process.env.JAVA_HOME;
        if (javaHome) {
            const javaPath = path.join(javaHome, 'bin', 'java');
            if (require('fs').existsSync(javaPath)) {
                return javaPath;
            }
        }
        // 使用系统默认Java
        return 'java';
    }

    compile(inputFile, options = {}) {
        return new Promise((resolve, reject) => {
            const args = [
                '-cp',
                path.join(this.compilerPath, 'lib', '*'),
                'com.chtl.cli.CHTLCLI',
                inputFile
            ];

            if (options.output) {
                args.push('-o', options.output);
            }

            if (options.optimize) {
                args.push('--optimize');
            }

            if (options.sourcemap) {
                args.push('--sourcemap');
            }

            const process = spawn(this.javaPath, args, {
                cwd: options.cwd || process.cwd()
            });

            let stdout = '';
            let stderr = '';

            process.stdout.on('data', (data) => {
                stdout += data.toString();
            });

            process.stderr.on('data', (data) => {
                stderr += data.toString();
            });

            process.on('close', (code) => {
                if (code === 0) {
                    resolve({ stdout, stderr });
                } else {
                    reject(new Error(`Compiler exited with code ${code}\n${stderr}`));
                }
            });
        });
    }
}

module.exports = CHTLCompilerWrapper;
EOF
    
    cd ..
}

# 安装依赖
install_dependencies() {
    info "安装依赖..."
    
    cd vscode-chtl
    npm install
    cd ..
}

# 编译TypeScript
compile_typescript() {
    info "编译TypeScript..."
    
    cd vscode-chtl
    npm run compile
    cd ..
}

# 打包插件
package_extension() {
    info "打包VSCode插件..."
    
    cd vscode-chtl
    
    # 更新package.json中的版本信息
    EXTENSION_VERSION=$(node -p "require('./package.json').version")
    
    # 创建VSIX包
    vsce package --no-dependencies
    
    # 移动到dist目录
    mkdir -p ../dist
    mv *.vsix ../dist/
    
    cd ..
    
    info "插件打包完成: dist/chtl-${EXTENSION_VERSION}.vsix"
}

# 创建安装说明
create_install_guide() {
    info "创建安装说明..."
    
    cat > dist/VSCODE_INSTALL_MACOS.md << 'EOF'
# VSCode CHTL插件安装指南 - macOS

## 安装步骤

### 方法1：通过VSCode界面安装

1. 打开VSCode
2. 按 `Cmd+Shift+P` 打开命令面板
3. 输入 `Extensions: Install from VSIX...`
4. 选择下载的 `.vsix` 文件
5. 重启VSCode

### 方法2：通过命令行安装

```bash
code --install-extension chtl-*.vsix
```

### 方法3：双击安装

在Finder中双击 `.vsix` 文件，VSCode会自动打开并提示安装。

## 功能特性

- **语法高亮**：完整的CHTL语法支持
- **智能补全**：关键字、模板、函数自动补全
- **错误检查**：实时语法错误提示
- **代码格式化**：自动代码格式化
- **代码片段**：常用代码模板
- **内置编译器**：无需单独安装编译器

## 使用方法

1. 创建 `.chtl` 文件
2. 编写CHTL代码
3. 按 `Cmd+Shift+B` 编译
4. 或右键选择 "Compile CHTL"

## 配置

打开VSCode设置（`Cmd+,`），搜索 "CHTL" 可以配置：

- `chtl.compiler.optimize`: 启用优化（默认：true）
- `chtl.compiler.sourcemap`: 生成源映射（默认：true）
- `chtl.compiler.outputDirectory`: 输出目录（默认：output）

## 故障排除

### Java未找到

插件需要Java 17+运行。请确保：

1. 已安装Java 17或更高版本
   ```bash
   brew install openjdk@17
   ```
2. 设置JAVA_HOME环境变量
   ```bash
   export JAVA_HOME=$(/usr/libexec/java_home -v 17)
   ```

### 编译失败

1. 检查CHTL语法是否正确
2. 查看输出面板的错误信息
3. 确保有写入权限

### 权限问题

如果遇到权限错误，可能需要：

```bash
chmod +x ~/.vscode/extensions/chtl-*/resources/compiler/bin/*
```

## 更新日志

查看 [CHANGELOG.md](https://github.com/your-org/vscode-chtl/blob/main/CHANGELOG.md)

## 反馈

- Issues: https://github.com/your-org/vscode-chtl/issues
- Email: chtl-dev@example.com
EOF
}

# 清理临时文件
cleanup() {
    info "清理临时文件..."
    
    cd vscode-chtl
    
    # 删除打包时产生的临时文件
    rm -rf resources/compiler
    rm -rf resources/modules
    rm -f resources/compiler/chtl-wrapper.js
    
    cd ..
}

# 显示构建信息
show_build_info() {
    info "构建信息："
    echo "  平台：macOS $(sw_vers -productVersion)"
    echo "  Node版本：$(node -v)"
    echo "  npm版本：$(npm -v)"
    echo "  构建时间：$(date)"
    
    # 显示生成的文件
    echo
    info "生成的文件："
    ls -lh dist/*.vsix 2>/dev/null || echo "  没有找到VSIX文件"
}

# 主流程
main() {
    echo "==================================="
    echo "VSCode CHTL插件构建脚本 - macOS平台"
    echo "==================================="
    echo
    
    # 切换到项目根目录
    cd "$(dirname "$0")/../.."
    
    check_environment
    build_compiler
    prepare_extension_resources
    install_dependencies
    compile_typescript
    package_extension
    create_install_guide
    cleanup
    show_build_info
    
    echo
    info "构建完成！"
    echo
    echo "安装方法："
    echo "  1. 在VSCode中: Cmd+Shift+P -> Extensions: Install from VSIX"
    echo "  2. 命令行: code --install-extension dist/*.vsix"
    echo "  3. 双击: 在Finder中双击.vsix文件"
}

# 运行主流程
main "$@"