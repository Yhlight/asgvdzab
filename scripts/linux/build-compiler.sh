#!/bin/bash

# CHTL编译器构建脚本 - Linux平台
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
    
    JAVA_VERSION=$(java -version 2>&1 | head -n 1 | cut -d'"' -f 2 | cut -d'.' -f 1)
    if [ "$JAVA_VERSION" -lt 17 ]; then
        error "需要Java 17或更高版本，当前版本：$JAVA_VERSION"
    fi
    
    # 检查Maven
    if ! command -v mvn &> /dev/null; then
        error "未找到Maven，请安装Maven 3.6+"
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
    DIST_DIR="dist/chtl-compiler-linux"
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

# 打包发布文件
package_distribution() {
    info "打包发布文件..."
    
    cd dist
    
    # 创建tar.gz包
    tar -czf chtl-compiler-linux.tar.gz chtl-compiler-linux/
    
    # 创建安装脚本
    cat > install.sh << 'EOF'
#!/bin/bash
# CHTL编译器安装脚本

set -e

INSTALL_DIR="/opt/chtl"

echo "安装CHTL编译器到 $INSTALL_DIR ..."

# 检查权限
if [ "$EUID" -ne 0 ]; then 
    echo "请使用sudo运行此脚本"
    exit 1
fi

# 解压文件
tar -xzf chtl-compiler-linux.tar.gz

# 创建安装目录
mkdir -p "$INSTALL_DIR"

# 复制文件
cp -r chtl-compiler-linux/* "$INSTALL_DIR/"

# 创建符号链接
ln -sf "$INSTALL_DIR/bin/chtl" /usr/local/bin/chtl
ln -sf "$INSTALL_DIR/bin/cjmod" /usr/local/bin/cjmod

# 设置环境变量
echo "export CHTL_HOME=$INSTALL_DIR" > /etc/profile.d/chtl.sh

echo "安装完成！"
echo "请运行 'source /etc/profile.d/chtl.sh' 或重新登录以使环境变量生效"
echo "然后运行 'chtl --version' 验证安装"
EOF
    
    chmod +x install.sh
    
    cd ..
    
    info "打包完成：dist/chtl-compiler-linux.tar.gz"
}

# 显示构建信息
show_build_info() {
    info "构建信息："
    echo "  平台：Linux"
    echo "  Java版本：$(java -version 2>&1 | head -n 1)"
    echo "  Maven版本：$(mvn -version | head -n 1)"
    echo "  构建时间：$(date)"
    
    if [ -f "dist/chtl-compiler-linux.tar.gz" ]; then
        echo "  包大小：$(du -h dist/chtl-compiler-linux.tar.gz | cut -f1)"
    fi
}

# 主流程
main() {
    echo "==================================="
    echo "CHTL编译器构建脚本 - Linux平台"
    echo "==================================="
    echo
    
    # 切换到项目根目录
    cd "$(dirname "$0")/../.."
    
    check_environment
    clean_build
    compile_project
    prepare_modules
    create_distribution
    package_distribution
    show_build_info
    
    echo
    info "构建完成！"
    echo
    echo "使用方法："
    echo "  1. 解压：tar -xzf dist/chtl-compiler-linux.tar.gz"
    echo "  2. 安装：sudo bash dist/install.sh"
    echo "  3. 使用：chtl input.chtl -o output/"
}

# 运行主流程
main "$@"