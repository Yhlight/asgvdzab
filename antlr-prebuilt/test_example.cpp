#include "antlr4-runtime.h"
#include <iostream>

using namespace antlr4;

int main() {
    try {
        std::cout << "=== ANTLR4预编译包测试 ===" << std::endl;
        
        // 测试基本组件
        std::string inputText = "test input";
        ANTLRInputStream input(inputText);
        std::cout << "✓ ANTLRInputStream 创建成功" << std::endl;
        
        // 测试Token创建
        CommonToken token(1, "test");
        std::cout << "✓ CommonToken 创建成功: " << token.getText() << std::endl;
        
        std::cout << "✓ ANTLR4预编译包工作正常！" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ 错误: " << e.what() << std::endl;
        return 1;
    }
}