#!/bin/bash

# CHTL编译器构建脚本

echo "开始构建CHTL编译器..."

# 创建构建目录
if [ ! -d "build" ]; then
    mkdir build
    echo "创建构建目录: build/"
fi

# 进入构建目录
cd build

# 配置CMake
echo "配置CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "CMake配置失败！"
    exit 1
fi

# 编译项目
echo "编译项目..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "编译失败！"
    exit 1
fi

echo "构建完成！"
echo "可执行文件位置: build/chtl_compiler"

# 测试编译器
echo "测试编译器..."
./chtl_compiler --version

if [ $? -eq 0 ]; then
    echo "编译器测试成功！"
else
    echo "编译器测试失败！"
    exit 1
fi

echo ""
echo "使用方法:"
echo "  ./chtl_compiler <输入文件> [选项]"
echo "  ./chtl_compiler --help 查看所有选项"
echo ""
echo "示例:"
echo "  ./chtl_compiler ../test.chtl -d"
echo "  ./chtl_compiler ../test.chtl -o output.html -m -f"