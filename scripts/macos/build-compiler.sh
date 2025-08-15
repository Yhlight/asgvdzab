#!/bin/bash

# CHTL编译器构建脚本 - macOS平台
# 构建编译器并打包官方模块

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
    
    # 检查Java
    if ! command -v java &> /dev/null; then
        error "未找到Java，请安装Java 17或更高版本"
    fi
    
    # macOS上获取Java版本的方式略有不同
    JAVA_VERSION=$(/usr/libexec/java_home -V 2>&1 | grep -E "17\.|18\.|19\.|20\.|21\." | head -1 | awk '{print $1}' | cut -d'.' -f1)
    
    if [ -z "$JAVA_VERSION" ] || [ "$JAVA_VERSION" -lt 17 ]; then
        error "需要Java 17或更高版本"
    fi
    
    # 设置JAVA_HOME
    export JAVA_HOME=$(/usr/libexec/java_home -v 17)
    
    # 检查Maven
    if ! command -v mvn &> /dev/null; then
        # 尝试使用Homebrew安装
        if command -v brew &> /dev/null; then
            warning "未找到Maven，尝试通过Homebrew安装..."
            brew install maven
        else
            error "未找到Maven，请安装Maven 3.6+"
        fi
    fi
    
    info "环境检查通过"
}

# 清理构建目录
clean_build() {
    info "清理构建目录..."
    rm -rf target/
    rm -rf build/
    rm -rf dist/
}

# 编译项目
compile_project() {
    info "编译CHTL编译器..."
    
    # 使用Maven构建
    mvn clean package -DskipTests
    
    if [ $? -ne 0 ]; then
        error "编译失败"
    fi
    
    info "编译成功"
}

# 准备模块
prepare_modules() {
    info "准备官方模块..."
    
    # 创建临时模块目录
    TEMP_MODULE_DIR="build/modules"
    mkdir -p "$TEMP_MODULE_DIR"
    
    # 复制官方模块
    if [ -d "src/main/java/com/chtl/module" ]; then
        cp -r src/main/java/com/chtl/module/* "$TEMP_MODULE_DIR/"
    fi
    
    # 编译Chtholly模块（如果存在）
    if [ -d "$TEMP_MODULE_DIR/Chtholly" ]; then
        info "构建Chtholly模块..."
        
        # 编译CJMOD部分
        if [ -d "$TEMP_MODULE_DIR/Chtholly/CJMOD/src" ]; then
            pushd "$TEMP_MODULE_DIR/Chtholly/CJMOD" > /dev/null
            
            # 创建build目录
            mkdir -p build
            
            # 编译Java文件
            find src -name "*.java" -print0 | xargs -0 javac -cp "../../../../../target/chtl-compiler.jar" -d build/
            
            # 打包成JAR
            jar cf chtholly-cjmod.jar -C build .
            
            popd > /dev/null
        fi
    fi
    
    info "模块准备完成"
}

# 创建发布包
create_distribution() {
    info "创建发布包..."
    
    # 创建发布目录
    DIST_DIR="dist/chtl-compiler-macos"
    mkdir -p "$DIST_DIR"/{bin,lib,modules,config,docs}
    
    # 复制编译器JAR
    cp target/chtl-compiler*.jar "$DIST_DIR/lib/chtl-compiler.jar"
    
    # 复制依赖
    if [ -d "target/lib" ]; then
        cp target/lib/*.jar "$DIST_DIR/lib/"
    fi
    
    # 复制模块
    if [ -d "build/modules" ]; then
        cp -r build/modules/* "$DIST_DIR/modules/"
    fi
    
    # 创建启动脚本
    cat > "$DIST_DIR/bin/chtl" << 'EOF'
#!/bin/bash
# CHTL编译器启动脚本

# 获取脚本所在目录
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BASE_DIR="$(dirname "$SCRIPT_DIR")"

# 设置类路径
CLASSPATH="$BASE_DIR/lib/*"

# 设置模块路径
export CHTL_MODULE_PATH="$BASE_DIR/modules"

# 检查Java版本
if [ -x "/usr/libexec/java_home" ]; then
    # macOS特定：使用java_home工具
    export JAVA_HOME=$(/usr/libexec/java_home -v 17 2>/dev/null || /usr/libexec/java_home)
fi

# 运行编译器
java -cp "$CLASSPATH" com.chtl.cli.CHTLCLI "$@"
EOF
    
    chmod +x "$DIST_DIR/bin/chtl"
    
    # 创建cjmod命令脚本
    cat > "$DIST_DIR/bin/cjmod" << 'EOF'
#!/bin/bash
# CJMOD管理工具启动脚本

# 获取脚本所在目录
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BASE_DIR="$(dirname "$SCRIPT_DIR")"

# 设置类路径
CLASSPATH="$BASE_DIR/lib/*"

# 设置模块路径
export CHTL_MODULE_PATH="$BASE_DIR/modules"

# 检查Java版本
if [ -x "/usr/libexec/java_home" ]; then
    # macOS特定：使用java_home工具
    export JAVA_HOME=$(/usr/libexec/java_home -v 17 2>/dev/null || /usr/libexec/java_home)
fi

# 运行CJMOD CLI
java -cp "$CLASSPATH" com.chtl.cli.CJmodCLI "$@"
EOF
    
    chmod +x "$DIST_DIR/bin/cjmod"
    
    # 复制文档
    cp README.md "$DIST_DIR/docs/"
    cp CHTL语法文档.md "$DIST_DIR/docs/"
    
    # 创建配置文件
    cat > "$DIST_DIR/config/chtl.conf" << EOF
# CHTL编译器配置文件

# 默认输出目录
output.dir=output

# 编译选项
compile.optimize=true
compile.sourcemap=true
compile.minify=false

# 模块路径
module.path=\${CHTL_HOME}/modules

# 日志级别
log.level=INFO
EOF
    
    info "发布包创建成功"
}

# 创建macOS应用包
create_app_bundle() {
    info "创建macOS应用包..."
    
    APP_NAME="CHTL Compiler"
    APP_DIR="dist/$APP_NAME.app"
    
    # 创建应用包结构
    mkdir -p "$APP_DIR/Contents"/{MacOS,Resources,Java}
    
    # 创建Info.plist
    cat > "$APP_DIR/Contents/Info.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleName</key>
    <string>CHTL Compiler</string>
    <key>CFBundleDisplayName</key>
    <string>CHTL Compiler</string>
    <key>CFBundleIdentifier</key>
    <string>com.chtl.compiler</string>
    <key>CFBundleVersion</key>
    <string>1.0.0</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0.0</string>
    <key>CFBundleExecutable</key>
    <string>chtl-launcher</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>LSMinimumSystemVersion</key>
    <string>10.13</string>
    <key>NSHighResolutionCapable</key>
    <true/>
</dict>
</plist>
EOF
    
    # 创建启动脚本
    cat > "$APP_DIR/Contents/MacOS/chtl-launcher" << 'EOF'
#!/bin/bash
# macOS应用启动器

# 获取应用包路径
APP_DIR="$( cd "$( dirname "$0" )/.." && pwd )"
JAVA_DIR="$APP_DIR/Java"

# 设置环境
export CHTL_HOME="$JAVA_DIR"
export CHTL_MODULE_PATH="$JAVA_DIR/modules"

# 启动终端并运行CHTL
osascript <<END
tell application "Terminal"
    do script "export CHTL_HOME='$JAVA_DIR' && export PATH='$JAVA_DIR/bin:\$PATH' && clear && echo 'CHTL Compiler - Type \"chtl --help\" for usage' && bash"
    activate
end tell
END
EOF
    
    chmod +x "$APP_DIR/Contents/MacOS/chtl-launcher"
    
    # 复制编译器文件
    cp -r dist/chtl-compiler-macos/* "$APP_DIR/Contents/Java/"
    
    # 创建图标（占位符）
    touch "$APP_DIR/Contents/Resources/chtl.icns"
    
    info "macOS应用包创建成功"
}

# 打包发布文件
package_distribution() {
    info "打包发布文件..."
    
    cd dist
    
    # 创建DMG镜像
    if command -v hdiutil &> /dev/null; then
        info "创建DMG镜像..."
        
        # 创建临时DMG
        hdiutil create -volname "CHTL Compiler" -srcfolder chtl-compiler-macos -ov -format UDZO chtl-compiler-macos.dmg
    else
        # 备用方案：创建tar.gz
        tar -czf chtl-compiler-macos.tar.gz chtl-compiler-macos/
    fi
    
    # 创建安装脚本
    cat > install.sh << 'EOF'
#!/bin/bash
# CHTL编译器安装脚本 - macOS

set -e

INSTALL_DIR="/usr/local/chtl"

echo "安装CHTL编译器到 $INSTALL_DIR ..."

# 检查权限
if [ ! -w "/usr/local" ]; then
    echo "需要管理员权限，请输入密码："
    sudo -v
fi

# 解压文件（如果是tar.gz）
if [ -f "chtl-compiler-macos.tar.gz" ]; then
    tar -xzf chtl-compiler-macos.tar.gz
fi

# 创建安装目录
sudo mkdir -p "$INSTALL_DIR"

# 复制文件
sudo cp -r chtl-compiler-macos/* "$INSTALL_DIR/"

# 创建符号链接
sudo ln -sf "$INSTALL_DIR/bin/chtl" /usr/local/bin/chtl
sudo ln -sf "$INSTALL_DIR/bin/cjmod" /usr/local/bin/cjmod

# 设置环境变量
SHELL_RC=""
if [ -n "$ZSH_VERSION" ]; then
    SHELL_RC="$HOME/.zshrc"
elif [ -n "$BASH_VERSION" ]; then
    SHELL_RC="$HOME/.bash_profile"
fi

if [ -n "$SHELL_RC" ]; then
    echo "" >> "$SHELL_RC"
    echo "# CHTL Compiler" >> "$SHELL_RC"
    echo "export CHTL_HOME=$INSTALL_DIR" >> "$SHELL_RC"
fi

echo "安装完成！"
echo "请运行 'source $SHELL_RC' 或重新打开终端"
echo "然后运行 'chtl --version' 验证安装"

# 如果存在应用包，也安装它
if [ -d "CHTL Compiler.app" ]; then
    echo ""
    echo "是否要安装CHTL Compiler.app到应用程序文件夹？[y/N]"
    read -r response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        sudo cp -r "CHTL Compiler.app" /Applications/
        echo "应用已安装到 /Applications/"
    fi
fi
EOF
    
    chmod +x install.sh
    
    cd ..
    
    if [ -f "dist/chtl-compiler-macos.dmg" ]; then
        info "打包完成：dist/chtl-compiler-macos.dmg"
    else
        info "打包完成：dist/chtl-compiler-macos.tar.gz"
    fi
}

# 显示构建信息
show_build_info() {
    info "构建信息："
    echo "  平台：macOS $(sw_vers -productVersion)"
    echo "  Java版本：$(java -version 2>&1 | head -n 1)"
    echo "  Maven版本：$(mvn -version | head -n 1)"
    echo "  构建时间：$(date)"
    
    if [ -f "dist/chtl-compiler-macos.dmg" ]; then
        echo "  DMG大小：$(du -h dist/chtl-compiler-macos.dmg | cut -f1)"
    elif [ -f "dist/chtl-compiler-macos.tar.gz" ]; then
        echo "  包大小：$(du -h dist/chtl-compiler-macos.tar.gz | cut -f1)"
    fi
}

# 主流程
main() {
    echo "==================================="
    echo "CHTL编译器构建脚本 - macOS平台"
    echo "==================================="
    echo
    
    # 切换到项目根目录
    cd "$(dirname "$0")/../.."
    
    check_environment
    clean_build
    compile_project
    prepare_modules
    create_distribution
    create_app_bundle
    package_distribution
    show_build_info
    
    echo
    info "构建完成！"
    echo
    echo "使用方法："
    if [ -f "dist/chtl-compiler-macos.dmg" ]; then
        echo "  1. 挂载：打开 dist/chtl-compiler-macos.dmg"
        echo "  2. 安装：运行其中的 install.sh"
    else
        echo "  1. 解压：tar -xzf dist/chtl-compiler-macos.tar.gz"
        echo "  2. 安装：bash dist/install.sh"
    fi
    echo "  3. 使用：chtl input.chtl -o output/"
}

# 运行主流程
main "$@"