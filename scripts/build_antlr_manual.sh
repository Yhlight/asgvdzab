#!/bin/bash

# ANTLR手动编译脚本
# 支持Linux和Windows (通过WSL/MinGW)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/antlr-manual-build"
INSTALL_DIR="$PROJECT_ROOT/antlr-manual-install"

echo "=== ANTLR手动编译工具 ==="
echo "构建目录: $BUILD_DIR"
echo "安装目录: $INSTALL_DIR"

# 创建目录
mkdir -p "$BUILD_DIR"
mkdir -p "$INSTALL_DIR"

# 检测操作系统
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "linux"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "macos"
    elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
        echo "windows"
    else
        echo "unknown"
    fi
}

OS=$(detect_os)
echo "检测到操作系统: $OS"

# 安装依赖
install_dependencies() {
    echo "=== 安装依赖 ==="
    
    case $OS in
        "linux")
            # Ubuntu/Debian
            if command -v apt-get &> /dev/null; then
                sudo apt-get update
                sudo apt-get install -y \
                    openjdk-11-jdk \
                    maven \
                    cmake \
                    build-essential \
                    git \
                    wget \
                    unzip \
                    pkg-config \
                    uuid-dev
            # CentOS/RHEL
            elif command -v yum &> /dev/null; then
                sudo yum install -y \
                    java-11-openjdk-devel \
                    maven \
                    cmake \
                    gcc-c++ \
                    git \
                    wget \
                    unzip \
                    pkgconfig \
                    libuuid-devel
            fi
            ;;
        "macos")
            if command -v brew &> /dev/null; then
                brew install openjdk@11 maven cmake git wget
            else
                echo "请先安装Homebrew: https://brew.sh/"
                exit 1
            fi
            ;;
        "windows")
            echo "Windows环境请确保已安装："
            echo "- Java 11+ JDK"
            echo "- Maven"
            echo "- CMake"
            echo "- MinGW或Visual Studio"
            echo "- Git"
            ;;
    esac
}

# 下载ANTLR源码
download_antlr() {
    echo "=== 下载ANTLR源码 ==="
    
    cd "$BUILD_DIR"
    
    # ANTLR版本配置
    ANTLR_VERSION="4.13.1"  # 最新稳定版
    ANTLR_TOOL_URL="https://github.com/antlr/antlr4/archive/refs/tags/${ANTLR_VERSION}.tar.gz"
    ANTLR_CPP_URL="https://github.com/antlr/antlr4/archive/refs/tags/${ANTLR_VERSION}.tar.gz"
    
    echo "下载ANTLR $ANTLR_VERSION..."
    
    if [ ! -f "antlr4-${ANTLR_VERSION}.tar.gz" ]; then
        wget -O "antlr4-${ANTLR_VERSION}.tar.gz" "$ANTLR_TOOL_URL"
    fi
    
    if [ ! -d "antlr4-${ANTLR_VERSION}" ]; then
        tar -xzf "antlr4-${ANTLR_VERSION}.tar.gz"
    fi
    
    echo "✓ ANTLR源码下载完成"
}

# 编译ANTLR工具 (Java)
build_antlr_tool() {
    echo "=== 编译ANTLR工具 ==="
    
    cd "$BUILD_DIR/antlr4-${ANTLR_VERSION}"
    
    # 使用Maven编译
    echo "使用Maven编译ANTLR工具..."
    mvn clean
    mvn -DskipTests install
    
    # 复制ANTLR工具JAR
    ANTLR_JAR=$(find . -name "antlr-*-complete.jar" | head -1)
    if [ -z "$ANTLR_JAR" ]; then
        # 如果没找到complete jar，尝试查找tool jar
        ANTLR_JAR=$(find . -name "antlr4-*.jar" | grep -v sources | grep -v javadoc | head -1)
    fi
    
    if [ -n "$ANTLR_JAR" ]; then
        cp "$ANTLR_JAR" "$INSTALL_DIR/antlr4.jar"
        echo "✓ ANTLR工具编译完成: $INSTALL_DIR/antlr4.jar"
    else
        echo "❌ 未找到ANTLR工具JAR文件"
        return 1
    fi
    
    # 创建antlr4命令脚本
    create_antlr_script
}

# 创建ANTLR命令脚本
create_antlr_script() {
    echo "=== 创建ANTLR命令脚本 ==="
    
    # Linux/Mac脚本
    cat > "$INSTALL_DIR/antlr4" << 'EOF'
#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
java -jar "$SCRIPT_DIR/antlr4.jar" "$@"
EOF
    chmod +x "$INSTALL_DIR/antlr4"
    
    # Windows批处理文件
    cat > "$INSTALL_DIR/antlr4.bat" << 'EOF'
@echo off
set SCRIPT_DIR=%~dp0
java -jar "%SCRIPT_DIR%antlr4.jar" %*
EOF
    
    echo "✓ ANTLR命令脚本创建完成"
}

# 编译ANTLR C++运行时
build_antlr_cpp_runtime() {
    echo "=== 编译ANTLR C++运行时 ==="
    
    cd "$BUILD_DIR/antlr4-${ANTLR_VERSION}/runtime/Cpp"
    
    # 创建构建目录
    mkdir -p build
    cd build
    
    # CMake配置
    CMAKE_ARGS=(
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"
        -DCMAKE_CXX_STANDARD=17
        -DWITH_LIBCXX=OFF
        -DANTLR4_INSTALL=ON
    )
    
    # Windows特定配置
    if [ "$OS" = "windows" ]; then
        CMAKE_ARGS+=(-G "MinGW Makefiles")
    fi
    
    echo "CMake配置..."
    cmake .. "${CMAKE_ARGS[@]}"
    
    # 编译
    echo "编译C++运行时..."
    if [ "$OS" = "windows" ]; then
        mingw32-make -j$(nproc)
        mingw32-make install
    else
        make -j$(nproc)
        make install
    fi
    
    echo "✓ ANTLR C++运行时编译完成"
}

# 验证编译结果
verify_build() {
    echo "=== 验证编译结果 ==="
    
    # 检查ANTLR工具
    if [ -f "$INSTALL_DIR/antlr4.jar" ]; then
        echo "✓ ANTLR工具: $INSTALL_DIR/antlr4.jar"
        java -jar "$INSTALL_DIR/antlr4.jar" -version 2>/dev/null || echo "❌ ANTLR工具无法运行"
    else
        echo "❌ ANTLR工具未找到"
    fi
    
    # 检查C++运行时
    if [ -f "$INSTALL_DIR/lib/libantlr4-runtime.a" ] || [ -f "$INSTALL_DIR/lib/libantlr4-runtime.so" ]; then
        echo "✓ C++运行时库: $INSTALL_DIR/lib/"
    else
        echo "❌ C++运行时库未找到"
    fi
    
    # 检查头文件
    if [ -d "$INSTALL_DIR/include/antlr4-runtime" ]; then
        echo "✓ C++头文件: $INSTALL_DIR/include/antlr4-runtime/"
    else
        echo "❌ C++头文件未找到"
    fi
}

# 生成使用说明
generate_usage_guide() {
    echo "=== 生成使用说明 ==="
    
    cat > "$INSTALL_DIR/README.md" << EOF
# ANTLR手动编译版本使用指南

## 版本信息
- ANTLR版本: $ANTLR_VERSION
- 编译时间: $(date)
- 操作系统: $OS

## 使用ANTLR工具

### Linux/Mac:
\`\`\`bash
# 使用脚本
$INSTALL_DIR/antlr4 MyGrammar.g4 -Dlanguage=Cpp -o generated/

# 或直接使用JAR
java -jar $INSTALL_DIR/antlr4.jar MyGrammar.g4 -Dlanguage=Cpp -o generated/
\`\`\`

### Windows:
\`\`\`cmd
REM 使用批处理文件
$INSTALL_DIR\\antlr4.bat MyGrammar.g4 -Dlanguage=Cpp -o generated\\

REM 或直接使用JAR
java -jar $INSTALL_DIR\\antlr4.jar MyGrammar.g4 -Dlanguage=Cpp -o generated\\
\`\`\`

## 在CMake中使用

\`\`\`cmake
# 设置ANTLR路径
set(ANTLR4_ROOT "$INSTALL_DIR")

# 查找库
find_library(ANTLR4_LIB 
    NAMES antlr4-runtime
    PATHS \${ANTLR4_ROOT}/lib
    NO_DEFAULT_PATH
)

# 设置头文件路径
set(ANTLR4_INCLUDE_DIR "\${ANTLR4_ROOT}/include")

# 链接库
target_link_libraries(your_target \${ANTLR4_LIB})
target_include_directories(your_target PRIVATE \${ANTLR4_INCLUDE_DIR})
\`\`\`

## 环境变量设置

### 添加到PATH (可选):
\`\`\`bash
export PATH="$INSTALL_DIR:\$PATH"
\`\`\`

### 设置ANTLR4_ROOT:
\`\`\`bash
export ANTLR4_ROOT="$INSTALL_DIR"
\`\`\`
EOF

    echo "✓ 使用说明已生成: $INSTALL_DIR/README.md"
}

# 主函数
main() {
    echo "开始ANTLR手动编译..."
    
    install_dependencies
    download_antlr
    build_antlr_tool
    build_antlr_cpp_runtime
    verify_build
    generate_usage_guide
    
    echo ""
    echo "🎉 ANTLR编译完成!"
    echo "安装目录: $INSTALL_DIR"
    echo "请查看 $INSTALL_DIR/README.md 了解使用方法"
    echo ""
    echo "要在你的项目中使用，请更新CMakeLists.txt中的ANTLR路径:"
    echo "set(ANTLR4_ROOT \"$INSTALL_DIR\")"
}

# 如果脚本被直接执行
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi