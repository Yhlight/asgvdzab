#include "antlr4-runtime.h"
#include "CSS3Lexer.h"
#include "CSS3Parser.h"
#include <iostream>

using namespace antlr4;

int main() {
    try {
        std::cout << "=== 简化ANTLR测试 ===" << std::endl;
        
        std::string simpleCSS = ".test { color: red; }";
        std::cout << "测试CSS: " << simpleCSS << std::endl;
        
        ANTLRInputStream input(simpleCSS);
        std::cout << "✓ ANTLRInputStream 创建成功" << std::endl;
        
        CSS3Lexer lexer(&input);
        std::cout << "✓ CSS3Lexer 创建成功" << std::endl;
        
        CommonTokenStream tokens(&lexer);
        std::cout << "✓ CommonTokenStream 创建成功" << std::endl;
        
        CSS3Parser parser(&tokens);
        std::cout << "✓ CSS3Parser 创建成功" << std::endl;
        
        std::cout << "开始解析..." << std::endl;
        auto result = parser.stylesheet();
        std::cout << "✓ 解析成功!" << std::endl;
        
        if (result) {
            std::cout << "解析树文本: " << result->getText() << std::endl;
        }
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
}