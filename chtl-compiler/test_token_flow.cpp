#include <iostream>
#include "compiler/chtl/chtl_lexer.h"

using namespace chtl;
using namespace chtl::compiler;

int main() {
    std::string source = "div { text { \"Hello\" } }";
    
    CHTLLexer lexer(source);
    
    std::cout << "Token flow:\n";
    
    // 模拟Parser的行为
    Token currentToken = lexer.nextToken();
    std::cout << "1. Initial token: type=" << static_cast<int>(currentToken.type) 
              << " value=[" << currentToken.value << "]\n";
    
    // parseTopLevel检查是否是IDENTIFIER
    if (currentToken.type == TokenType::IDENTIFIER) {
        std::cout << "2. Found IDENTIFIER in parseTopLevel, calling parseElement\n";
        // parseElement开始
        std::cout << "3. In parseElement, current token: type=" << static_cast<int>(currentToken.type) 
                  << " value=[" << currentToken.value << "]\n";
        
        // parseElement检查是否是IDENTIFIER
        if (currentToken.type == TokenType::IDENTIFIER) {
            std::cout << "4. It's an IDENTIFIER, using as tagName: " << currentToken.value << "\n";
            // advance
            currentToken = lexer.nextToken();
            std::cout << "5. After advance, current token: type=" << static_cast<int>(currentToken.type) 
                      << " value=[" << currentToken.value << "]\n";
        } else {
            std::cout << "4. ERROR: Not an IDENTIFIER!\n";
        }
    }
    
    return 0;
}