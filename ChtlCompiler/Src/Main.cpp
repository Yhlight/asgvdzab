#include <iostream>
#include <string>
#include "Lexer/Lexer.h"
#include "Common/Context.h"

int main(int argc, char* argv[]) {
    std::cout << "CHTL Compiler v1.0.0\n";
    std::cout << "Based on C++17\n\n";
    
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input.chtl>\n";
        return 1;
    }
    
    // 简单的词法分析测试
    Chtl::Lexer lexer;
    std::string testCode = R"(
div {
    id: main;
    style {
        .box {
            color: red;
        }
    }
    text { "Hello CHTL!" }
}
)";
    
    lexer.setSource(testCode, argv[1]);
    auto tokens = lexer.tokenize();
    
    std::cout << "Tokenized " << tokens.size() << " tokens\n";
    
    return 0;
}