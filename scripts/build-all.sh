#!/bin/bash

# CHTL项目统一构建脚本
# 自动检测平台并调用相应的构建脚本

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
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

header() {
    echo -e "${BLUE}=== $1 ===${NC}"
}

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

# 显示使用说明
usage() {
    echo "用法: $0 [选项]"
    echo
    echo "选项:"
    echo "  all              构建所有组件（默认）"
    echo "  compiler         仅构建编译器"
    echo "  vscode           仅构建VSCode插件"
    echo "  module <path>    打包指定模块"
    echo "  clean            清理构建文件"
    echo "  --platform <os>  指定平台 (linux/macos/windows)"
    echo "  --skip-tests     跳过测试"
    echo "  -h, --help       显示帮助信息"
    echo
    echo "示例:"
    echo "  $0                    # 构建所有组件"
    echo "  $0 compiler           # 仅构建编译器"
    echo "  $0 module Chtholly    # 打包Chtholly模块"
    echo "  $0 --platform linux   # 为Linux平台构建"
}

# 主函数
main() {
    # 切换到脚本所在目录
    cd "$(dirname "$0")"
    
    # 默认值
    BUILD_TARGET="all"
    PLATFORM=$(detect_os)
    SKIP_TESTS=false
    MODULE_PATH=""
    
    # 解析参数
    while [[ $# -gt 0 ]]; do
        case $1 in
            all|compiler|vscode|clean)
                BUILD_TARGET="$1"
                shift
                ;;
            module)
                BUILD_TARGET="module"
                MODULE_PATH="$2"
                shift 2
                ;;
            --platform)
                PLATFORM="$2"
                shift 2
                ;;
            --skip-tests)
                SKIP_TESTS=true
                shift
                ;;
            -h|--help)
                usage
                exit 0
                ;;
            *)
                error "未知参数: $1"
                ;;
        esac
    done
    
    # 显示构建信息
    echo "========================================"
    echo "CHTL项目统一构建脚本"
    echo "========================================"
    echo "构建目标: $BUILD_TARGET"
    echo "目标平台: $PLATFORM"
    echo "跳过测试: $SKIP_TESTS"
    echo
    
    # 检查平台
    if [ "$PLATFORM" == "unknown" ]; then
        error "无法识别的操作系统: $OSTYPE"
    fi
    
    # 检查脚本是否存在
    if [ ! -d "$PLATFORM" ]; then
        error "找不到平台目录: $PLATFORM"
    fi
    
    # 执行构建
    case $BUILD_TARGET in
        all)
            build_all
            ;;
        compiler)
            build_compiler
            ;;
        vscode)
            build_vscode
            ;;
        module)
            if [ $# -lt 1 ]; then
                echo "错误: 'module' 命令需要指定模块路径"
                echo "用法: $0 module <module-path> [--type cmod|cjmod|unified]"
                exit 1
            fi
            MODULE_PATH="$1"
            MODULE_TYPE="unified"
            shift
            
            # 解析模块类型
            while [[ $# -gt 0 ]]; do
                case $1 in
                    --type)
                        MODULE_TYPE="$2"
                        shift 2
                        ;;
                    *)
                        shift
                        ;;
                esac
            done
            
            build_module
            ;;
        clean)
            clean_all
            ;;
    esac
    
    echo
    info "构建完成！"
    echo
    show_results
}

# 构建所有组件
build_all() {
    header "构建编译器"
    build_compiler
    
    echo
    header "构建VSCode插件"
    build_vscode
}

# 构建编译器
build_compiler() {
    info "开始构建编译器..."
    
    if [ "$PLATFORM" == "windows" ]; then
        # Windows使用批处理文件
        cmd.exe /c "$PLATFORM\\build-compiler.bat"
    else
        # Linux和macOS使用Shell脚本
        bash "$PLATFORM/build-compiler.sh"
    fi
    
    info "编译器构建完成"
}

# 构建VSCode插件
build_vscode() {
    info "开始构建VSCode插件..."
    
    if [ "$PLATFORM" == "windows" ]; then
        # Windows使用批处理文件
        cmd.exe /c "$PLATFORM\\build-vscode-extension.bat"
    else
        # Linux和macOS使用Shell脚本
        bash "$PLATFORM/build-vscode-extension.sh"
    fi
    
    info "VSCode插件构建完成"
}

# 打包模块
build_module() {
    if [ -z "$MODULE_PATH" ]; then
        error "请指定模块路径"
    fi
    
    MODULE_TYPE="${MODULE_TYPE:-unified}"  # 默认为unified
    
    info "开始打包模块: $MODULE_PATH (类型: $MODULE_TYPE)"
    
    # 转到项目根目录
    cd ..
    
    # 根据模块类型选择相应的打包脚本
    case "$MODULE_TYPE" in
        cmod|CMOD)
            PACKAGE_SCRIPT="package-cmod"
            ;;
        cjmod|CJMOD)
            PACKAGE_SCRIPT="package-cjmod"
            ;;
        unified|*)
            PACKAGE_SCRIPT="package-unified"
            ;;
    esac
    
    if [ "$PLATFORM" == "windows" ]; then
        # Windows使用批处理文件
        cmd.exe /c "scripts\\$PLATFORM\\${PACKAGE_SCRIPT}.bat $MODULE_PATH"
    else
        # Linux和macOS使用Shell脚本
        bash "scripts/$PLATFORM/${PACKAGE_SCRIPT}.sh" "$MODULE_PATH"
    fi
    
    info "模块打包完成"
}

# 清理构建文件
clean_all() {
    info "清理构建文件..."
    
    cd ..
    
    # 清理目录
    rm -rf target/ build/ dist/
    
    # 清理VSCode插件构建文件
    if [ -d "vscode-chtl" ]; then
        cd vscode-chtl
        rm -rf out/ node_modules/ resources/compiler resources/modules *.vsix
        cd ..
    fi
    
    # 清理测试输出
    rm -rf test-output/ test-reports/
    
    # 清理临时文件
    find . -name "*.class" -type f -delete
    find . -name "*.log" -type f -delete
    find . -name "*.tmp" -type f -delete
    
    info "清理完成"
}

# 显示构建结果
show_results() {
    cd ..
    
    header "构建结果"
    
    # 检查编译器
    if [ "$BUILD_TARGET" == "all" ] || [ "$BUILD_TARGET" == "compiler" ]; then
        echo "编译器包:"
        case $PLATFORM in
            linux)
                ls -lh dist/chtl-compiler-linux.tar.gz 2>/dev/null || echo "  未找到"
                ;;
            macos)
                ls -lh dist/chtl-compiler-macos.dmg 2>/dev/null || ls -lh dist/chtl-compiler-macos.tar.gz 2>/dev/null || echo "  未找到"
                ;;
            windows)
                ls -lh dist/chtl-compiler-windows.zip 2>/dev/null || echo "  未找到"
                ;;
        esac
    fi
    
    # 检查VSCode插件
    if [ "$BUILD_TARGET" == "all" ] || [ "$BUILD_TARGET" == "vscode" ]; then
        echo
        echo "VSCode插件:"
        ls -lh dist/*.vsix 2>/dev/null || echo "  未找到"
    fi
    
    # 检查模块包
    if [ "$BUILD_TARGET" == "module" ]; then
        echo
        echo "模块包:"
        ls -lh *.cjmod 2>/dev/null || ls -lh dist/*.cjmod 2>/dev/null || echo "  未找到"
    fi
    
    echo
    header "下一步"
    
    case $PLATFORM in
        linux)
            echo "1. 安装编译器: sudo bash dist/install.sh"
            echo "2. 安装VSCode插件: code --install-extension dist/*.vsix"
            ;;
        macos)
            echo "1. 安装编译器: 打开dist/*.dmg或运行 bash dist/install.sh"
            echo "2. 安装VSCode插件: code --install-extension dist/*.vsix"
            ;;
        windows)
            echo "1. 安装编译器: 以管理员身份运行 dist\\install.bat"
            echo "2. 安装VSCode插件: code --install-extension dist\\*.vsix"
            ;;
    esac
}

# 运行主函数
main "$@"