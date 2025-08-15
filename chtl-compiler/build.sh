#!/bin/bash

# CHTL编译器构建脚本

# 设置颜色
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}CHTL编译器构建脚本${NC}"
echo "===================="

# 创建构建目录
if [ ! -d "build" ]; then
    echo "创建构建目录..."
    mkdir build
fi

cd build

# 运行CMake
echo -e "\n${YELLOW}配置项目...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake配置失败！${NC}"
    exit 1
fi

# 编译
echo -e "\n${YELLOW}编译项目...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}编译失败！${NC}"
    exit 1
fi

echo -e "\n${GREEN}构建成功！${NC}"
echo "可执行文件位于: build/chtl-compiler"

# 提示测试
echo -e "\n${YELLOW}测试编译器:${NC}"
echo "  ./chtl-compiler ../examples/hello.chtl"
echo "  ./chtl-compiler -m -o output.html ../examples/hello.chtl"