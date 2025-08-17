#!/bin/bash

# CHTL编译器清理脚本

echo "开始清理CHTL编译器构建文件..."

# 删除构建目录
if [ -d "build" ]; then
    rm -rf build
    echo "删除构建目录: build/"
fi

# 删除生成的文件
if [ -f "test.html" ]; then
    rm test.html
    echo "删除生成文件: test.html"
fi

# 删除其他可能的生成文件
find . -name "*.o" -delete 2>/dev/null
find . -name "*.so" -delete 2>/dev/null
find . -name "*.a" -delete 2>/dev/null

echo "清理完成！"
echo "现在可以重新运行 build.sh 来构建项目"