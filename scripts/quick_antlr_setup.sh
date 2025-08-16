#!/bin/bash

# ANTLR快速设置脚本
# 下载预编译版本或编译最小版本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
ANTLR_DIR="$PROJECT_ROOT/antlr-quick"

echo "=== ANTLR快速设置 ==="
echo "目标目录: $ANTLR_DIR"

mkdir -p "$ANTLR_DIR"

# 方案1：下载预编译的ANTLR工具
download_prebuilt_tool() {
    echo "=== 下载预编译ANTLR工具 ==="
    
    cd "$ANTLR_DIR"
    
    ANTLR_VERSION="4.13.1"
    ANTLR_JAR_URL="https://www.antlr.org/download/antlr-${ANTLR_VERSION}-complete.jar"
    
    echo "下载ANTLR $ANTLR_VERSION 工具..."
    
    if [ ! -f "antlr4.jar" ]; then
        if command -v wget &> /dev/null; then
            wget -O "antlr4.jar" "$ANTLR_JAR_URL"
        elif command -v curl &> /dev/null; then
            curl -L -o "antlr4.jar" "$ANTLR_JAR_URL"
        else
            echo "❌ 需要wget或curl来下载文件"
            return 1
        fi
    fi
    
    # 创建运行脚本
    cat > antlr4 << 'EOF'
#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
java -jar "$SCRIPT_DIR/antlr4.jar" "$@"
EOF
    chmod +x antlr4
    
    echo "✓ ANTLR工具下载完成"
}

# 方案2：使用系统包管理器
install_system_antlr() {
    echo "=== 使用系统包管理器安装ANTLR ==="
    
    if command -v apt-get &> /dev/null; then
        # Ubuntu/Debian
        sudo apt-get update
        sudo apt-get install -y antlr4 libantlr4-runtime-dev
        echo "✓ 通过apt-get安装完成"
        return 0
    elif command -v yum &> /dev/null; then
        # CentOS/RHEL
        sudo yum install -y antlr4-cpp-runtime-devel
        echo "✓ 通过yum安装完成"
        return 0
    elif command -v brew &> /dev/null; then
        # macOS
        brew install antlr
        echo "✓ 通过Homebrew安装完成"
        return 0
    else
        echo "⚠ 未找到支持的包管理器"
        return 1
    fi
}

# 方案3：最小编译版本
minimal_cpp_runtime() {
    echo "=== 创建最小C++运行时包装器 ==="
    
    # 创建简化的ANTLR兼容接口
    mkdir -p "$ANTLR_DIR/include/antlr4-runtime"
    mkdir -p "$ANTLR_DIR/src"
    
    # 创建简化的头文件
    cat > "$ANTLR_DIR/include/antlr4-runtime/antlr4-runtime.h" << 'EOF'
#pragma once

// 简化的ANTLR4运行时兼容接口
// 用于支持CHTL项目的基本需求

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

namespace antlr4 {

// 基础异常类
class RuntimeException : public std::runtime_error {
public:
    explicit RuntimeException(const std::string& message) : std::runtime_error(message) {}
};

// 输入流类
class ANTLRInputStream {
public:
    explicit ANTLRInputStream(const std::string& input) : input_(input) {}
    const std::string& toString() const { return input_; }
private:
    std::string input_;
};

// Token类
class Token {
public:
    virtual ~Token() = default;
    virtual std::string getText() const = 0;
    virtual size_t getType() const = 0;
};

// 基础Token实现
class CommonToken : public Token {
public:
    CommonToken(size_t type, const std::string& text) : type_(type), text_(text) {}
    std::string getText() const override { return text_; }
    size_t getType() const override { return type_; }
private:
    size_t type_;
    std::string text_;
};

// Token流
class TokenStream {
public:
    virtual ~TokenStream() = default;
    virtual Token* LT(int k) = 0;
    virtual void consume() = 0;
};

class CommonTokenStream : public TokenStream {
public:
    explicit CommonTokenStream(class Lexer* lexer) : lexer_(lexer) {}
    Token* LT(int k) override { return nullptr; }
    void consume() override {}
private:
    class Lexer* lexer_;
};

// 词法分析器基类
class Lexer {
public:
    explicit Lexer(ANTLRInputStream* input) : input_(input) {}
    virtual ~Lexer() = default;
    virtual void removeErrorListeners() {}
    virtual void addErrorListener(class BaseErrorListener* listener) {}
protected:
    ANTLRInputStream* input_;
};

// 解析器基类
class Parser {
public:
    explicit Parser(TokenStream* input) : input_(input) {}
    virtual ~Parser() = default;
    virtual void removeErrorListeners() {}
    virtual void addErrorListener(class BaseErrorListener* listener) {}
protected:
    TokenStream* input_;
};

// 解析树节点
namespace tree {
    class ParseTree {
    public:
        virtual ~ParseTree() = default;
        virtual std::string getText() const { return ""; }
        virtual size_t getChildCount() const { return 0; }
        virtual ParseTree* getChild(size_t index) const { return nullptr; }
    };
    
    class ParseTreeWalker {
    public:
        void walk(class ParseTreeListener* listener, ParseTree* tree) {
            // 简化实现：什么都不做
        }
    };
}

// 错误监听器
class BaseErrorListener {
public:
    virtual ~BaseErrorListener() = default;
    virtual void syntaxError(class Recognizer* recognizer, Token* offendingSymbol,
                           size_t line, size_t charPositionInLine, 
                           const std::string& msg, std::exception_ptr e) {}
};

// 识别器基类
class Recognizer {
public:
    virtual ~Recognizer() = default;
};

// 解析树监听器
class ParseTreeListener {
public:
    virtual ~ParseTreeListener() = default;
};

} // namespace antlr4

// 简化的生成代码兼容性
class CSS3BaseListener : public antlr4::ParseTreeListener {
public:
    virtual ~CSS3BaseListener() = default;
};

EOF

    # 创建CMake查找模块
    cat > "$ANTLR_DIR/FindANTLR4.cmake" << 'EOF'
# FindANTLR4.cmake - 查找ANTLR4库
# 为简化运行时设计

set(ANTLR4_FOUND TRUE)
set(ANTLR4_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
set(ANTLR4_LIBRARIES "")  # 头文件库，无需链接

message(STATUS "Found ANTLR4 (简化版本): ${ANTLR4_INCLUDE_DIRS}")
EOF

    echo "✓ 最小C++运行时创建完成"
}

# 生成集成说明
generate_integration_guide() {
    cat > "$ANTLR_DIR/INTEGRATION.md" << EOF
# ANTLR集成指南

## 选择的方案

根据你的环境，我们提供了多种ANTLR集成方案：

### 方案1：预编译工具 + 手动运行时
- ANTLR工具：预编译JAR
- C++运行时：手动编译或使用简化版本
- 适用于：所有环境

### 方案2：系统包管理器
- 完整的系统安装
- 适用于：有root权限的Linux/macOS

### 方案3：最小实现
- 仅提供接口兼容性
- 适用于：快速原型开发

## 在CHTL项目中使用

### 更新CMakeLists.txt：

\`\`\`cmake
# 设置ANTLR路径
set(ANTLR4_ROOT "$ANTLR_DIR")

# 方案1/2：查找完整运行时
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(ANTLR4 antlr4-runtime)
endif()

if(NOT ANTLR4_FOUND)
    find_library(ANTLR4_LIB 
        NAMES antlr4-runtime
        PATHS \${ANTLR4_ROOT}/lib /usr/local/lib /usr/lib
    )
    find_path(ANTLR4_INCLUDE_DIR 
        NAMES antlr4-runtime.h
        PATHS \${ANTLR4_ROOT}/include /usr/local/include /usr/include
    )
endif()

# 方案3：使用简化运行时
if(NOT ANTLR4_LIB)
    include(\${ANTLR4_ROOT}/FindANTLR4.cmake)
endif()
\`\`\`

### 生成代码：

\`\`\`bash
# 使用下载的ANTLR工具
$ANTLR_DIR/antlr4 grammars/CSS3.g4 -Dlanguage=Cpp -o generated/antlr/css/
$ANTLR_DIR/antlr4 grammars/JavaScript.g4 -Dlanguage=Cpp -o generated/antlr/js/
\`\`\`

## 故障排除

### 如果遇到编译错误：
1. 检查C++标准设置（需要C++17）
2. 确保ANTLR版本兼容
3. 考虑使用简化运行时（方案3）

### 如果生成的代码有问题：
1. 检查语法文件
2. 确保ANTLR工具版本正确
3. 查看生成代码的API兼容性
EOF

    echo "✓ 集成指南生成完成: $ANTLR_DIR/INTEGRATION.md"
}

# 主菜单
main() {
    echo "请选择ANTLR设置方案："
    echo "1) 下载预编译工具 (推荐)"
    echo "2) 使用系统包管理器"
    echo "3) 创建最小兼容实现"
    echo "4) 全部尝试"
    
    read -p "请选择 (1-4): " choice
    
    case $choice in
        1)
            download_prebuilt_tool
            ;;
        2)
            install_system_antlr
            ;;
        3)
            minimal_cpp_runtime
            ;;
        4)
            echo "尝试所有方案..."
            download_prebuilt_tool || echo "方案1失败"
            install_system_antlr || echo "方案2失败"
            minimal_cpp_runtime || echo "方案3失败"
            ;;
        *)
            echo "无效选择"
            exit 1
            ;;
    esac
    
    generate_integration_guide
    
    echo ""
    echo "🎉 ANTLR快速设置完成!"
    echo "设置目录: $ANTLR_DIR"
    echo "请查看 $ANTLR_DIR/INTEGRATION.md 了解集成方法"
}

if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi