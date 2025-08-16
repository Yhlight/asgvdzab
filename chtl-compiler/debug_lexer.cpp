#include <iostream>
#include "compiler/chtl/chtl_lexer.h"

using namespace chtl;
using namespace chtl::compiler;

int main() {
    std::string source = R"(div {
    text { "Hello CHTL" }
})";
    
    std::cout << "Source:\n" << source << "\n\n";
    
    CHTLLexer lexer(source, "debug.chtl");
    
    std::cout << "Tokens:\n";
    while (true) {
        Token token = lexer.nextToken();
        
        std::cout << "Type: " << static_cast<int>(token.type) 
                  << ", Value: [" << token.value << "]"
                  << ", Location: " << token.location.line << ":" << token.location.column
                  << std::endl;
        
        if (token.type == TokenType::EOF_TOKEN) {
            break;
        }
    }
    
    return 0;
}