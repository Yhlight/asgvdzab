#!/bin/bash

# CJMOD模块打包脚本 - Linux平台

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

# 显示使用说明
usage() {
    echo "用法: $0 <module-path> [options]"
    echo
    echo "选项:"
    echo "  -o, --output <dir>      输出目录 (默认: .)"
    echo "  -n, --name <name>       模块名称 (默认: 从info文件读取)"
    echo "  -v, --version <ver>     模块版本 (默认: 从info文件读取)"
    echo "  --no-source             不包含源代码"
    echo "  --sign                  对模块进行签名"
    echo "  -h, --help              显示帮助信息"
    echo
    echo "示例:"
    echo "  $0 src/main/java/com/chtl/module/Chtholly/CJMOD -o dist/"
    echo "  $0 mymodule/CJMOD --sign"
}

# 解析命令行参数
MODULE_PATH=""
OUTPUT_DIR="."
MODULE_NAME=""
MODULE_VERSION=""
INCLUDE_SOURCE=true
SIGN_MODULE=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -o|--output)
            OUTPUT_DIR="$2"
            shift 2
            ;;
        -n|--name)
            MODULE_NAME="$2"
            shift 2
            ;;
        -v|--version)
            MODULE_VERSION="$2"
            shift 2
            ;;
        --no-source)
            INCLUDE_SOURCE=false
            shift
            ;;
        --sign)
            SIGN_MODULE=true
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            if [ -z "$MODULE_PATH" ]; then
                MODULE_PATH="$1"
            else
                error "未知参数: $1"
            fi
            shift
            ;;
    esac
done

# 验证参数
if [ -z "$MODULE_PATH" ]; then
    error "请指定CJMOD模块路径"
fi

if [ ! -d "$MODULE_PATH" ]; then
    error "模块路径不存在: $MODULE_PATH"
fi

# 获取模块名（从路径推断）
INFERRED_NAME=$(basename "$(dirname "$MODULE_PATH")")
if [ "$INFERRED_NAME" == "CJMOD" ] || [ "$INFERRED_NAME" == "cjmod" ]; then
    INFERRED_NAME=$(basename "$(dirname "$(dirname "$MODULE_PATH")")")
fi

# 检查CJMOD结构
check_cjmod_structure() {
    info "检查CJMOD模块结构..."
    
    if [ ! -d "$MODULE_PATH/src" ]; then
        error "缺少src目录"
    fi
    
    if [ ! -d "$MODULE_PATH/info" ]; then
        error "缺少info目录"
    fi
    
    # 检查info文件
    INFO_FILE="$MODULE_PATH/info/${INFERRED_NAME}.chtl"
    if [ ! -f "$INFO_FILE" ]; then
        error "缺少info文件: ${INFERRED_NAME}.chtl"
    fi
    
    # 检查是否有Java源文件
    if ! find "$MODULE_PATH/src" -name "*.java" -type f | grep -q .; then
        error "没有找到Java源文件"
    fi
    
    info "CJMOD结构检查通过"
}

# 读取模块信息
read_module_info() {
    info "读取模块信息..."
    
    INFO_FILE="$MODULE_PATH/info/${INFERRED_NAME}.chtl"
    
    # 如果没有指定名称，从info文件读取
    if [ -z "$MODULE_NAME" ]; then
        MODULE_NAME=$(grep -o 'name[[:space:]]*=[[:space:]]*"[^"]*"' "$INFO_FILE" | sed 's/.*"\([^"]*\)".*/\1/')
    fi
    
    if [ -z "$MODULE_VERSION" ]; then
        MODULE_VERSION=$(grep -o 'version[[:space:]]*=[[:space:]]*"[^"]*"' "$INFO_FILE" | sed 's/.*"\([^"]*\)".*/\1/')
    fi
    
    if [ -z "$MODULE_NAME" ]; then
        MODULE_NAME="$INFERRED_NAME"
        warning "使用推断的模块名称: $MODULE_NAME"
    fi
    
    if [ -z "$MODULE_VERSION" ]; then
        MODULE_VERSION="1.0.0"
        warning "使用默认版本: $MODULE_VERSION"
    fi
    
    info "模块: $MODULE_NAME v$MODULE_VERSION"
}

# 编译Java源代码
compile_java_sources() {
    info "编译Java源代码..."
    
    # 创建build目录
    BUILD_DIR="$MODULE_PATH/build"
    mkdir -p "$BUILD_DIR"
    
    # 构建类路径
    CLASSPATH=""
    
    # 添加CHTL编译器到类路径
    if [ -f "target/chtl-compiler.jar" ]; then
        CLASSPATH="target/chtl-compiler.jar"
    elif [ -f "/opt/chtl/lib/chtl-compiler.jar" ]; then
        CLASSPATH="/opt/chtl/lib/chtl-compiler.jar"
    else
        # 尝试查找最近的编译器JAR
        COMPILER_JAR=$(find . -name "chtl-compiler*.jar" -type f | head -1)
        if [ -n "$COMPILER_JAR" ]; then
            CLASSPATH="$COMPILER_JAR"
        else
            error "找不到CHTL编译器JAR文件"
        fi
    fi
    
    # 添加lib目录（如果存在）
    if [ -d "$MODULE_PATH/lib" ]; then
        CLASSPATH="$CLASSPATH:$MODULE_PATH/lib/*"
    fi
    
    # 编译
    find "$MODULE_PATH/src" -name "*.java" -print0 | xargs -0 javac -cp "$CLASSPATH" -d "$BUILD_DIR"
    
    if [ $? -ne 0 ]; then
        error "编译失败"
    fi
    
    info "编译成功"
}

# 创建CJMOD包
create_cjmod_package() {
    info "创建CJMOD包..."
    
    # 创建输出目录
    mkdir -p "$OUTPUT_DIR"
    
    # 包文件名
    PACKAGE_FILE="$OUTPUT_DIR/${MODULE_NAME}-${MODULE_VERSION}.cjmod"
    
    # 创建临时目录
    TEMP_DIR=$(mktemp -d)
    trap "rm -rf $TEMP_DIR" EXIT
    
    # 复制info文件
    mkdir -p "$TEMP_DIR/info"
    cp -r "$MODULE_PATH/info"/* "$TEMP_DIR/info/"
    
    # 复制编译后的类文件
    if [ -d "$MODULE_PATH/build" ]; then
        mkdir -p "$TEMP_DIR/classes"
        cp -r "$MODULE_PATH/build"/* "$TEMP_DIR/classes/"
    fi
    
    # 复制依赖库（如果存在）
    if [ -d "$MODULE_PATH/lib" ]; then
        mkdir -p "$TEMP_DIR/lib"
        cp -r "$MODULE_PATH/lib"/* "$TEMP_DIR/lib/"
    fi
    
    # 复制资源文件（如果存在）
    if [ -d "$MODULE_PATH/resources" ]; then
        mkdir -p "$TEMP_DIR/resources"
        cp -r "$MODULE_PATH/resources"/* "$TEMP_DIR/resources/"
    fi
    
    # 复制源代码（如果需要）
    if [ "$INCLUDE_SOURCE" = true ] && [ -d "$MODULE_PATH/src" ]; then
        mkdir -p "$TEMP_DIR/src"
        cp -r "$MODULE_PATH/src"/* "$TEMP_DIR/src/"
    fi
    
    # 处理子模块（如果存在）
    process_submodules "$TEMP_DIR"
    
    # 创建元信息
    mkdir -p "$TEMP_DIR/META-INF"
    cat > "$TEMP_DIR/META-INF/MANIFEST.MF" << EOF
Manifest-Version: 1.0
Module-Type: CJMOD
Module-Name: $MODULE_NAME
Module-Version: $MODULE_VERSION
Created-By: CJMOD Package Tool
Build-Time: $(date -u +"%Y-%m-%d %H:%M:%S UTC")
Include-Source: $INCLUDE_SOURCE
EOF
    
    # 计算校验和
    info "计算校验和..."
    CHECKSUM=$(find "$TEMP_DIR" -type f ! -path "*/META-INF/*" -exec sha256sum {} \; | sort | sha256sum | cut -d' ' -f1)
    echo "$CHECKSUM" > "$TEMP_DIR/META-INF/CHECKSUM.SHA256"
    
    # 创建JAR包（CJMOD使用JAR格式）
    cd "$TEMP_DIR"
    jar cf "$PACKAGE_FILE" *
    cd - > /dev/null
    
    info "CJMOD包创建成功: $PACKAGE_FILE"
}

# 处理子模块
process_submodules() {
    local base_dir="$1"
    
    # 查找src目录下的子模块
    if [ -d "$MODULE_PATH/src" ]; then
        for subdir in "$MODULE_PATH"/src/*/; do
            if [ -d "$subdir" ] && [ -d "${subdir}src" ] && [ -d "${subdir}info" ]; then
                local submodule_name=$(basename "$subdir")
                info "发现子模块: $submodule_name"
                
                # 编译子模块
                if [ -d "${subdir}src" ]; then
                    local sub_build_dir="${subdir}build"
                    mkdir -p "$sub_build_dir"
                    find "${subdir}src" -name "*.java" -print0 | xargs -0 javac -cp "$CLASSPATH" -d "$sub_build_dir"
                    
                    # 复制子模块编译结果
                    mkdir -p "$base_dir/submodules/$submodule_name/classes"
                    cp -r "$sub_build_dir"/* "$base_dir/submodules/$submodule_name/classes/"
                fi
                
                # 复制子模块info
                mkdir -p "$base_dir/submodules/$submodule_name/info"
                cp -r "${subdir}info"/* "$base_dir/submodules/$submodule_name/info/"
            fi
        done
    fi
}

# 签名模块
sign_module() {
    if [ "$SIGN_MODULE" != true ]; then
        return
    fi
    
    info "签名模块..."
    
    # 检查cjmod命令是否可用
    if command -v cjmod &> /dev/null; then
        cjmod sign "$PACKAGE_FILE"
    else
        warning "cjmod命令不可用，跳过签名"
    fi
}

# 显示包信息
show_package_info() {
    info "包信息:"
    echo "  文件: $PACKAGE_FILE"
    echo "  大小: $(du -h "$PACKAGE_FILE" | cut -f1)"
    echo "  SHA256: $(sha256sum "$PACKAGE_FILE" | cut -d' ' -f1)"
    
    # 列出包内容
    echo
    info "包内容:"
    jar tf "$PACKAGE_FILE" | head -20
    
    TOTAL_FILES=$(jar tf "$PACKAGE_FILE" | wc -l)
    if [ $TOTAL_FILES -gt 20 ]; then
        echo "  ... 还有 $((TOTAL_FILES - 20)) 个文件"
    fi
}

# 主流程
main() {
    echo "==================================="
    echo "CJMOD模块打包工具 - Linux平台"
    echo "==================================="
    echo
    
    check_cjmod_structure
    read_module_info
    compile_java_sources
    create_cjmod_package
    sign_module
    show_package_info
    
    echo
    info "打包完成！"
}

# 运行主流程
main