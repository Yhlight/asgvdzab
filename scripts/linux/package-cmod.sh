#!/bin/bash

# CMOD模块打包脚本 - Linux平台

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
    echo "  --no-minify             不压缩CHTL文件"
    echo "  -h, --help              显示帮助信息"
    echo
    echo "示例:"
    echo "  $0 src/main/java/com/chtl/module/Chtholly/CMOD -o dist/"
    echo "  $0 mymodule/CMOD --name MyModule --version 1.0.0"
}

# 解析命令行参数
MODULE_PATH=""
OUTPUT_DIR="."
MODULE_NAME=""
MODULE_VERSION=""
MINIFY=true

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
        --no-minify)
            MINIFY=false
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
    error "请指定CMOD模块路径"
fi

if [ ! -d "$MODULE_PATH" ]; then
    error "模块路径不存在: $MODULE_PATH"
fi

# 获取模块名（从路径推断）
INFERRED_NAME=$(basename "$(dirname "$MODULE_PATH")")
if [ "$INFERRED_NAME" == "CMOD" ] || [ "$INFERRED_NAME" == "cmod" ]; then
    INFERRED_NAME=$(basename "$(dirname "$(dirname "$MODULE_PATH")")")
fi

# 检查CMOD结构
check_cmod_structure() {
    info "检查CMOD模块结构..."
    
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
    
    # 检查是否包含[Info]块
    if ! grep -q "^\[Info\]" "$INFO_FILE"; then
        warning "info文件缺少[Info]块"
    fi
    
    # 提示[Export]块的作用
    if ! grep -q "^\[Export\]" "$INFO_FILE"; then
        info "提示: 可以添加[Export]块来优化模块查询性能"
    fi
    
    info "CMOD结构检查通过"
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

# 创建CMOD包
create_cmod_package() {
    info "创建CMOD包..."
    
    # 创建输出目录
    mkdir -p "$OUTPUT_DIR"
    
    # 包文件名
    PACKAGE_FILE="$OUTPUT_DIR/${MODULE_NAME}-${MODULE_VERSION}.cmod"
    
    # 创建临时目录
    TEMP_DIR=$(mktemp -d)
    trap "rm -rf $TEMP_DIR" EXIT
    
    # 复制CMOD结构
    cp -r "$MODULE_PATH"/* "$TEMP_DIR/"
    
    # 处理子模块（如果存在）
    process_submodules "$TEMP_DIR"
    
    # 压缩CHTL文件（如果需要）
    if [ "$MINIFY" = true ]; then
        info "压缩CHTL文件..."
        find "$TEMP_DIR" -name "*.chtl" -type f -exec bash -c '
            file="$1"
            # 简单的压缩：移除注释和多余空白
            sed -i "s|//.*$||g; /^[[:space:]]*$/d; s/^[[:space:]]*//; s/[[:space:]]*$//" "$file"
        ' _ {} \;
    fi
    
    # 创建元信息
    mkdir -p "$TEMP_DIR/META-INF"
    cat > "$TEMP_DIR/META-INF/MANIFEST.MF" << EOF
Manifest-Version: 1.0
Module-Type: CMOD
Module-Name: $MODULE_NAME
Module-Version: $MODULE_VERSION
Created-By: CMOD Package Tool
Build-Time: $(date -u +"%Y-%m-%d %H:%M:%S UTC")
EOF
    
    # 计算校验和
    info "计算校验和..."
    CHECKSUM=$(find "$TEMP_DIR" -type f ! -path "*/META-INF/*" -exec sha256sum {} \; | sort | sha256sum | cut -d' ' -f1)
    echo "$CHECKSUM" > "$TEMP_DIR/META-INF/CHECKSUM.SHA256"
    
    # 创建ZIP包（CMOD使用ZIP格式）
    cd "$TEMP_DIR"
    zip -qr "$PACKAGE_FILE" *
    cd - > /dev/null
    
    info "CMOD包创建成功: $PACKAGE_FILE"
}

# 处理子模块
process_submodules() {
    local base_dir="$1"
    
    # 查找src目录下的子模块
    if [ -d "$base_dir/src" ]; then
        for subdir in "$base_dir/src"/*; do
            if [ -d "$subdir" ] && [ -d "$subdir/src" ] && [ -d "$subdir/info" ]; then
                local submodule_name=$(basename "$subdir")
                info "发现子模块: $submodule_name"
                
                # 验证子模块结构
                if [ ! -f "$subdir/info/${submodule_name}.chtl" ]; then
                    warning "子模块缺少info文件: ${submodule_name}.chtl"
                fi
            fi
        done
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
    unzip -l "$PACKAGE_FILE" | tail -n +4 | head -20
    
    TOTAL_FILES=$(unzip -l "$PACKAGE_FILE" | tail -n +4 | head -n -2 | wc -l)
    if [ $TOTAL_FILES -gt 20 ]; then
        echo "  ... 还有 $((TOTAL_FILES - 20)) 个文件"
    fi
}

# 主流程
main() {
    echo "==================================="
    echo "CMOD模块打包工具 - Linux平台"
    echo "==================================="
    echo
    
    check_cmod_structure
    read_module_info
    create_cmod_package
    show_package_info
    
    echo
    info "打包完成！"
}

# 运行主流程
main