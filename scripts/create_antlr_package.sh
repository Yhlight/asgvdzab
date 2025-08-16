#!/bin/bash

# ANTLR4预编译包打包脚本
# 创建可分发的压缩包

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
PACKAGE_NAME="antlr4-prebuilt-$(date +%Y%m%d)"
DIST_DIR="$PROJECT_ROOT/dist"

echo "=== ANTLR4预编译包打包工具 ==="
echo "项目根目录: $PROJECT_ROOT"
echo "包名: $PACKAGE_NAME"
echo "输出目录: $DIST_DIR"

# 检查预编译包是否存在
if [ ! -d "$PROJECT_ROOT/antlr-prebuilt" ]; then
    echo "❌ 错误: 未找到antlr-prebuilt目录"
    echo "请先运行编译脚本创建预编译包"
    exit 1
fi

# 创建分发目录
mkdir -p "$DIST_DIR"

# 验证预编译包完整性
verify_package() {
    echo "🔍 验证预编译包完整性..."
    
    local errors=0
    
    # 检查必需文件
    local required_files=(
        "bin/antlr4.jar"
        "lib/libantlr4-runtime.a"
        "lib/libantlr4-runtime.so"
        "include/antlr4-runtime.h"
        "ANTLR4Config.cmake"
        "README.md"
        "VERSION"
    )
    
    for file in "${required_files[@]}"; do
        if [ ! -f "$PROJECT_ROOT/antlr-prebuilt/$file" ]; then
            echo "❌ 缺失文件: $file"
            ((errors++))
        else
            echo "✓ $file"
        fi
    done
    
    # 检查目录结构
    local required_dirs=(
        "bin"
        "lib" 
        "include"
        "include/tree"
        "include/atn"
        "include/dfa"
    )
    
    for dir in "${required_dirs[@]}"; do
        if [ ! -d "$PROJECT_ROOT/antlr-prebuilt/$dir" ]; then
            echo "❌ 缺失目录: $dir"
            ((errors++))
        fi
    done
    
    if [ $errors -eq 0 ]; then
        echo "✓ 预编译包完整性验证通过"
        return 0
    else
        echo "❌ 发现 $errors 个问题"
        return 1
    fi
}

# 运行测试
test_package() {
    echo "🧪 测试预编译包功能..."
    
    cd "$PROJECT_ROOT"
    
    # 测试ANTLR工具
    if java -jar antlr-prebuilt/bin/antlr4.jar 2>&1 | grep -q "ANTLR Parser Generator"; then
        echo "✓ ANTLR工具可正常运行"
    else
        echo "❌ ANTLR工具测试失败"
        return 1
    fi
    
    # 测试C++编译
    if [ -f "test_antlr_prebuilt_static" ]; then
        if ./test_antlr_prebuilt_static | grep -q "工作正常"; then
            echo "✓ C++运行时库测试通过"
        else
            echo "❌ C++运行时库测试失败"
            return 1
        fi
    else
        echo "⚠ 跳过C++测试 (测试程序不存在)"
    fi
    
    echo "✓ 所有测试通过"
}

# 创建压缩包
create_archive() {
    echo "📦 创建压缩包..."
    
    cd "$PROJECT_ROOT"
    
    # 创建tar.gz包
    tar -czf "$DIST_DIR/${PACKAGE_NAME}.tar.gz" \
        --exclude="*.tmp" \
        --exclude="*.log" \
        --exclude="test_*" \
        antlr-prebuilt/
    
    # 创建zip包 (Windows友好)
    if command -v zip &> /dev/null; then
        zip -r "$DIST_DIR/${PACKAGE_NAME}.zip" antlr-prebuilt/ \
            -x "*.tmp" "*.log" "test_*"
    fi
    
    echo "✓ 压缩包创建完成:"
    ls -lh "$DIST_DIR/${PACKAGE_NAME}."*
}

# 生成校验和
generate_checksums() {
    echo "🔐 生成校验和..."
    
    cd "$DIST_DIR"
    
    # 生成SHA256校验和
    if command -v sha256sum &> /dev/null; then
        sha256sum "${PACKAGE_NAME}."* > "${PACKAGE_NAME}.sha256"
        echo "✓ SHA256校验和已生成"
    elif command -v shasum &> /dev/null; then
        shasum -a 256 "${PACKAGE_NAME}."* > "${PACKAGE_NAME}.sha256"
        echo "✓ SHA256校验和已生成"
    else
        echo "⚠ 无法生成校验和 (缺少sha256sum/shasum)"
    fi
}

# 生成发布说明
generate_release_notes() {
    echo "📄 生成发布说明..."
    
    cat > "$DIST_DIR/${PACKAGE_NAME}-RELEASE.md" << EOF
# ANTLR4 预编译包 - $PACKAGE_NAME

## 📦 发布信息

- **版本**: 4.13.1
- **发布日期**: $(date '+%Y-%m-%d')
- **编译平台**: Linux x86_64
- **编译器**: GCC $(gcc --version | head -1 | awk '{print $4}')
- **C++标准**: C++17

## 📊 包内容

$(cd "$PROJECT_ROOT" && du -sh antlr-prebuilt/)
- ANTLR4工具: $(ls -lh "$PROJECT_ROOT/antlr-prebuilt/bin/antlr4.jar" | awk '{print $5}')
- 静态库: $(ls -lh "$PROJECT_ROOT/antlr-prebuilt/lib/libantlr4-runtime.a" | awk '{print $5}')
- 动态库: $(ls -lh "$PROJECT_ROOT/antlr-prebuilt/lib/libantlr4-runtime.so" | awk '{print $5}')

## 🚀 快速使用

\`\`\`bash
# 解压
tar -xzf ${PACKAGE_NAME}.tar.gz
# 或
unzip ${PACKAGE_NAME}.zip

# 测试
java -jar antlr-prebuilt/bin/antlr4.jar

# CMake集成
include(antlr-prebuilt/ANTLR4Config.cmake)
target_link_libraries(your_target ANTLR4::antlr4_runtime)
\`\`\`

## 📋 系统要求

- Java 8+
- C++17兼容编译器
- CMake 3.10+ (可选)

## 🔗 相关链接

- [ANTLR4官方文档](https://www.antlr.org/)
- [CHTL项目](https://github.com/your-repo/chtl)

## 📞 支持

如遇问题请参考 antlr-prebuilt/README.md

---
*由CHTL项目自动生成*
EOF

    echo "✓ 发布说明已生成: ${PACKAGE_NAME}-RELEASE.md"
}

# 显示包信息
show_package_info() {
    echo ""
    echo "📈 打包统计:"
    echo "  包名: $PACKAGE_NAME"
    echo "  总大小: $(du -sh "$PROJECT_ROOT/antlr-prebuilt" | awk '{print $1}')"
    echo "  文件数: $(find "$PROJECT_ROOT/antlr-prebuilt" -type f | wc -l)"
    echo "  目录数: $(find "$PROJECT_ROOT/antlr-prebuilt" -type d | wc -l)"
    echo ""
    echo "📂 输出文件:"
    ls -la "$DIST_DIR/" | grep "$PACKAGE_NAME" || echo "  (无输出文件)"
}

# 主流程
main() {
    echo "开始打包流程..."
    
    # 验证包完整性
    if ! verify_package; then
        echo "❌ 预编译包验证失败，停止打包"
        exit 1
    fi
    
    # 运行测试
    if ! test_package; then
        echo "❌ 预编译包测试失败，停止打包"
        exit 1
    fi
    
    # 创建压缩包
    create_archive
    
    # 生成校验和
    generate_checksums
    
    # 生成发布说明
    generate_release_notes
    
    # 显示信息
    show_package_info
    
    echo ""
    echo "🎉 ANTLR4预编译包打包完成!"
    echo "输出目录: $DIST_DIR"
    echo ""
    echo "📋 使用方法:"
    echo "  1. 分发压缩包: ${PACKAGE_NAME}.tar.gz 或 ${PACKAGE_NAME}.zip"
    echo "  2. 用户解压后即可使用"
    echo "  3. 参考 ${PACKAGE_NAME}-RELEASE.md 获取详细说明"
}

# 检查参数
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi