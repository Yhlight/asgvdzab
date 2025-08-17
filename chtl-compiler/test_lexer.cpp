#include <iostream>
#include "compiler/chtl/chtl_lexer.h"

int main() {
    std::string code = "[Template]";
    
    chtl::CHTLLexer lexer(code);
    
    chtl::Token token;
    int count = 0;
    do {
        token = lexer.nextToken();
        std::cout << "Token " << count++ << ": type=" << static_cast<int>(token.type) 
                  << ", value='" << token.value << "'" 
                  << ", line=" << token.line << ", column=" << token.column << std::endl;
    } while (token.type != chtl::TokenType::EOF_TOKEN && count < 20);
    
    return 0;
}