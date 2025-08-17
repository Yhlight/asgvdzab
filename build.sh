#!/bin/bash

# CHTL编译器构建脚本
# 使用方法: ./build.sh [选项]

set -e  # 遇到错误时退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 默认配置
BUILD_TYPE="Release"
CLEAN_BUILD=false
PARALLEL_JOBS=$(nproc 2>/dev/null || echo 4)
INSTALL_DEPS=false
RUN_TESTS=false
VERBOSE=false

# 打印帮助信息
print_help() {
    echo "CHTL编译器构建脚本"
    echo ""
    echo "使用方法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  -d, --debug          构建Debug版本 (默认: Release)"
    echo "  -c, --clean          清理构建目录"
    echo "  -j, --jobs <数量>    并行编译任务数 (默认: CPU核心数)"
    echo "  -i, --install-deps   安装依赖项"
    echo "  -t, --test           运行测试"
    echo "  -v, --verbose        详细输出"
    echo "  -h, --help           显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0                   # 构建Release版本"
    echo "  $0 -d                # 构建Debug版本"
    echo "  $0 -c -d -j8         # 清理构建，Debug版本，8个并行任务"
    echo "  $0 -i -t             # 安装依赖并运行测试"
}

# 打印彩色消息
print_message() {
    local color=$1
    local message=$2
    echo -e "${color}${message}${NC}"
}

# 检查依赖
check_dependencies() {
    print_message $BLUE "检查构建依赖..."
    
    # 检查CMake
    if ! command -v cmake &> /dev/null; then
        print_message $RED "错误: 未找到CMake，请先安装CMake 3.16或更高版本"
        exit 1
    fi
    
    local cmake_version=$(cmake --version | head -n1 | cut -d' ' -f3)
    print_message $GREEN "找到CMake版本: $cmake_version"
    
    # 检查编译器
    if command -v g++ &> /dev/null; then
        local gcc_version=$(g++ --version | head -n1)
        print_message $GREEN "找到编译器: $gcc_version"
    elif command -v clang++ &> /dev/null; then
        local clang_version=$(clang++ --version | head -n1)
        print_message $GREEN "找到编译器: $clang_version"
    else
        print_message $RED "错误: 未找到C++编译器"
        exit 1
    fi
}

# 安装依赖
install_dependencies() {
    print_message $BLUE "安装依赖项..."
    
    if command -v apt-get &> /dev/null; then
        # Ubuntu/Debian
        sudo apt-get update
        sudo apt-get install -y cmake build-essential libantlr4-runtime-dev
    elif command -v yum &> /dev/null; then
        # CentOS/RHEL
        sudo yum install -y cmake gcc-c++ antlr4-cpp-runtime-devel
    elif command -v brew &> /dev/null; then
        # macOS
        brew install cmake antlr4-cpp-runtime
    else
        print_message $YELLOW "警告: 无法自动安装依赖，请手动安装以下依赖:"
        print_message $YELLOW "  - CMake 3.16+"
        print_message $YELLOW "  - C++17编译器"
        print_message $YELLOW "  - ANTLR4 C++运行时"
    fi
}

# 清理构建目录
clean_build() {
    print_message $BLUE "清理构建目录..."
    if [ -d "build" ]; then
        rm -rf build
        print_message $GREEN "构建目录已清理"
    else
        print_message $YELLOW "构建目录不存在，跳过清理"
    fi
}

# 创建构建目录
create_build_dir() {
    print_message $BLUE "创建构建目录..."
    mkdir -p build
    cd build
}

# 配置项目
configure_project() {
    print_message $BLUE "配置项目 (${BUILD_TYPE})..."
    
    local cmake_args="-DCMAKE_BUILD_TYPE=${BUILD_TYPE}"
    
    if [ "$VERBOSE" = true ]; then
        cmake_args="$cmake_args -DCMAKE_VERBOSE_MAKEFILE=ON"
    fi
    
    cmake .. $cmake_args
    
    if [ $? -eq 0 ]; then
        print_message $GREEN "项目配置成功"
    else
        print_message $RED "项目配置失败"
        exit 1
    fi
}

# 编译项目
build_project() {
    print_message $BLUE "编译项目 (使用 $PARALLEL_JOBS 个并行任务)..."
    
    local start_time=$(date +%s)
    
    if [ "$VERBOSE" = true ]; then
        make -j$PARALLEL_JOBS VERBOSE=1
    else
        make -j$PARALLEL_JOBS
    fi
    
    local end_time=$(date +%s)
    local duration=$((end_time - start_time))
    
    if [ $? -eq 0 ]; then
        print_message $GREEN "编译成功! 用时: ${duration}秒"
    else
        print_message $RED "编译失败"
        exit 1
    fi
}

# 运行测试
run_tests() {
    print_message $BLUE "运行测试..."
    
    if [ -f "test/chtl_test" ]; then
        ./test/chtl_test
        if [ $? -eq 0 ]; then
            print_message $GREEN "所有测试通过"
        else
            print_message $RED "测试失败"
            exit 1
        fi
    else
        print_message $YELLOW "未找到测试程序，跳过测试"
    fi
}

# 显示构建结果
show_results() {
    print_message $BLUE "构建结果:"
    
    if [ -f "Bin/ChtlCompiler" ]; then
        local file_size=$(ls -lh Bin/ChtlCompiler | awk '{print $5}')
        print_message $GREEN "  可执行文件: Bin/ChtlCompiler ($file_size)"
    fi
    
    if [ -f "Lib/libChtlCore.a" ]; then
        local lib_size=$(ls -lh Lib/libChtlCore.a | awk '{print $5}')
        print_message $GREEN "  静态库: Lib/libChtlCore.a ($lib_size)"
    fi
    
    print_message $BLUE "使用方法:"
    print_message $YELLOW "  ./Bin/ChtlCompiler ../Examples/Example.chtl -o Output.html"
}

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -j|--jobs)
            PARALLEL_JOBS="$2"
            shift 2
            ;;
        -i|--install-deps)
            INSTALL_DEPS=true
            shift
            ;;
        -t|--test)
            RUN_TESTS=true
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            print_help
            exit 0
            ;;
        *)
            print_message $RED "未知选项: $1"
            print_help
            exit 1
            ;;
    esac
done

# 主构建流程
main() {
    print_message $BLUE "========================================="
    print_message $BLUE "    CHTL编译器构建脚本"
    print_message $BLUE "========================================="
    
    # 安装依赖
    if [ "$INSTALL_DEPS" = true ]; then
        install_dependencies
    fi
    
    # 检查依赖
    check_dependencies
    
    # 清理构建
    if [ "$CLEAN_BUILD" = true ]; then
        clean_build
    fi
    
    # 创建构建目录
    create_build_dir
    
    # 配置项目
    configure_project
    
    # 编译项目
    build_project
    
    # 运行测试
    if [ "$RUN_TESTS" = true ]; then
        run_tests
    fi
    
    # 显示结果
    show_results
    
    print_message $GREEN "构建完成!"
}

# 运行主函数
main