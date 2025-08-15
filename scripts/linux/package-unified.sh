#!/bin/bash

# 统一模块打包脚本 - Linux平台
# 可以打包包含CMOD和CJMOD的完整模块

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
    echo "  -n, --name <name>       模块名称 (默认: 从模块路径推断)"
    echo "  -v, --version <ver>     模块版本 (默认: 1.0.0)"
    echo "  --no-source             不包含源代码"
    echo "  --no-minify             不压缩CHTL文件"
    echo "  --sign                  对模块进行签名"
    echo "  -h, --help              显示帮助信息"
    echo
    echo "示例:"
    echo "  $0 src/main/java/com/chtl/module/Chtholly -o dist/"
    echo "  $0 mymodule --name MyModule --version 2.0.0"
}

# 解析命令行参数
MODULE_PATH=""
OUTPUT_DIR="."
MODULE_NAME=""
MODULE_VERSION=""
INCLUDE_SOURCE=true
MINIFY=true
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
        --no-minify)
            MINIFY=false
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
    error "请指定模块路径"
fi

if [ ! -d "$MODULE_PATH" ]; then
    error "模块路径不存在: $MODULE_PATH"
fi

# 获取模块名
if [ -z "$MODULE_NAME" ]; then
    MODULE_NAME=$(basename "$MODULE_PATH")
fi

if [ -z "$MODULE_VERSION" ]; then
    MODULE_VERSION="1.0.0"
fi

# 检查模块结构
check_module_structure() {
    info "检查模块结构..."
    
    HAS_CMOD=false
    HAS_CJMOD=false
    
    # 检查CMOD
    if [ -d "$MODULE_PATH/CMOD" ] || [ -d "$MODULE_PATH/cmod" ]; then
        HAS_CMOD=true
        CMOD_DIR=$([ -d "$MODULE_PATH/CMOD" ] && echo "CMOD" || echo "cmod")
        info "发现CMOD模块"
    fi
    
    # 检查CJMOD
    if [ -d "$MODULE_PATH/CJMOD" ] || [ -d "$MODULE_PATH/cjmod" ]; then
        HAS_CJMOD=true
        CJMOD_DIR=$([ -d "$MODULE_PATH/CJMOD" ] && echo "CJMOD" || echo "cjmod")
        info "发现CJMOD模块"
    fi
    
    if [ "$HAS_CMOD" = false ] && [ "$HAS_CJMOD" = false ]; then
        error "没有找到CMOD或CJMOD目录"
    fi
}

# 打包CMOD
package_cmod() {
    if [ "$HAS_CMOD" = false ]; then
        return
    fi
    
    info "打包CMOD部分..."
    
    # 使用CMOD专用打包脚本
    SCRIPT_DIR="$(dirname "$0")"
    if [ -x "$SCRIPT_DIR/package-cmod.sh" ]; then
        "$SCRIPT_DIR/package-cmod.sh" "$MODULE_PATH/$CMOD_DIR" \
            -o "$OUTPUT_DIR" \
            -n "${MODULE_NAME}-cmod" \
            -v "$MODULE_VERSION" \
            $([ "$MINIFY" = false ] && echo "--no-minify")
    else
        warning "找不到package-cmod.sh脚本，跳过CMOD打包"
    fi
}

# 打包CJMOD
package_cjmod() {
    if [ "$HAS_CJMOD" = false ]; then
        return
    fi
    
    info "打包CJMOD部分..."
    
    # 使用CJMOD专用打包脚本
    SCRIPT_DIR="$(dirname "$0")"
    if [ -x "$SCRIPT_DIR/package-cjmod.sh" ]; then
        "$SCRIPT_DIR/package-cjmod.sh" "$MODULE_PATH/$CJMOD_DIR" \
            -o "$OUTPUT_DIR" \
            -n "${MODULE_NAME}-cjmod" \
            -v "$MODULE_VERSION" \
            $([ "$INCLUDE_SOURCE" = false ] && echo "--no-source") \
            $([ "$SIGN_MODULE" = true ] && echo "--sign")
    else
        warning "找不到package-cjmod.sh脚本，跳过CJMOD打包"
    fi
}

# 创建统一包
create_unified_package() {
    info "创建统一模块包..."
    
    # 创建输出目录
    mkdir -p "$OUTPUT_DIR"
    
    # 包文件名
    PACKAGE_FILE="$OUTPUT_DIR/${MODULE_NAME}-${MODULE_VERSION}-unified.zip"
    
    # 创建临时目录
    TEMP_DIR=$(mktemp -d)
    trap "rm -rf $TEMP_DIR" EXIT
    
    # 复制整个模块结构
    cp -r "$MODULE_PATH"/* "$TEMP_DIR/"
    
    # 如果存在单独的CMOD和CJMOD包，也包含它们
    if [ -f "$OUTPUT_DIR/${MODULE_NAME}-cmod-${MODULE_VERSION}.cmod" ]; then
        mkdir -p "$TEMP_DIR/packages"
        cp "$OUTPUT_DIR/${MODULE_NAME}-cmod-${MODULE_VERSION}.cmod" "$TEMP_DIR/packages/"
    fi
    
    if [ -f "$OUTPUT_DIR/${MODULE_NAME}-cjmod-${MODULE_VERSION}.cjmod" ]; then
        mkdir -p "$TEMP_DIR/packages"
        cp "$OUTPUT_DIR/${MODULE_NAME}-cjmod-${MODULE_VERSION}.cjmod" "$TEMP_DIR/packages/"
    fi
    
    # 创建README
    cat > "$TEMP_DIR/README.md" << EOF
# $MODULE_NAME 模块

版本: $MODULE_VERSION

## 包含内容

EOF
    
    if [ "$HAS_CMOD" = true ]; then
        echo "- CMOD: 提供CHTL模板和样式" >> "$TEMP_DIR/README.md"
    fi
    
    if [ "$HAS_CJMOD" = true ]; then
        echo "- CJMOD: 提供CHTL JS扩展功能" >> "$TEMP_DIR/README.md"
    fi
    
    cat >> "$TEMP_DIR/README.md" << EOF

## 安装方法

### 方法1：使用统一包
将整个目录复制到项目的module文件夹中。

### 方法2：分别安装
EOF
    
    if [ "$HAS_CMOD" = true ]; then
        echo "- CMOD: \`cjmod install packages/${MODULE_NAME}-cmod-${MODULE_VERSION}.cmod\`" >> "$TEMP_DIR/README.md"
    fi
    
    if [ "$HAS_CJMOD" = true ]; then
        echo "- CJMOD: \`cjmod install packages/${MODULE_NAME}-cjmod-${MODULE_VERSION}.cjmod\`" >> "$TEMP_DIR/README.md"
    fi
    
    # 创建元信息
    mkdir -p "$TEMP_DIR/META-INF"
    cat > "$TEMP_DIR/META-INF/MANIFEST.MF" << EOF
Manifest-Version: 1.0
Module-Type: Unified
Module-Name: $MODULE_NAME
Module-Version: $MODULE_VERSION
Has-CMOD: $HAS_CMOD
Has-CJMOD: $HAS_CJMOD
Created-By: Unified Package Tool
Build-Time: $(date -u +"%Y-%m-%d %H:%M:%S UTC")
EOF
    
    # 创建ZIP包
    cd "$TEMP_DIR"
    zip -qr "$PACKAGE_FILE" *
    cd - > /dev/null
    
    info "统一包创建成功: $PACKAGE_FILE"
}

# 显示包信息
show_package_info() {
    info "打包结果:"
    echo
    
    if [ "$HAS_CMOD" = true ] && [ -f "$OUTPUT_DIR/${MODULE_NAME}-cmod-${MODULE_VERSION}.cmod" ]; then
        echo "  CMOD包: ${MODULE_NAME}-cmod-${MODULE_VERSION}.cmod"
        echo "    大小: $(du -h "$OUTPUT_DIR/${MODULE_NAME}-cmod-${MODULE_VERSION}.cmod" | cut -f1)"
    fi
    
    if [ "$HAS_CJMOD" = true ] && [ -f "$OUTPUT_DIR/${MODULE_NAME}-cjmod-${MODULE_VERSION}.cjmod" ]; then
        echo "  CJMOD包: ${MODULE_NAME}-cjmod-${MODULE_VERSION}.cjmod"
        echo "    大小: $(du -h "$OUTPUT_DIR/${MODULE_NAME}-cjmod-${MODULE_VERSION}.cjmod" | cut -f1)"
    fi
    
    if [ -f "$OUTPUT_DIR/${MODULE_NAME}-${MODULE_VERSION}-unified.zip" ]; then
        echo "  统一包: ${MODULE_NAME}-${MODULE_VERSION}-unified.zip"
        echo "    大小: $(du -h "$OUTPUT_DIR/${MODULE_NAME}-${MODULE_VERSION}-unified.zip" | cut -f1)"
    fi
}

# 主流程
main() {
    echo "==================================="
    echo "统一模块打包工具 - Linux平台"
    echo "==================================="
    echo
    
    info "模块: $MODULE_NAME v$MODULE_VERSION"
    
    check_module_structure
    package_cmod
    package_cjmod
    create_unified_package
    show_package_info
    
    echo
    info "打包完成！"
}

# 运行主流程
main