#include <iostream>
#include "compiler/chtl/chtl_lexer.h"
#include "compiler/chtl/chtl_parser.h"
#include "compiler/chtl/chtl_compiler.h"

using namespace chtl;
using namespace chtl::compiler;

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::KEYWORD_TEXT: return "KEYWORD_TEXT";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::EOF_TOKEN: return "EOF_TOKEN";
        default: return "OTHER(" + std::to_string(static_cast<int>(type)) + ")";
    }
}

class DebugParser : public CHTLParser {
public:
    std::shared_ptr<ast::ASTNode> parseTopLevel() override {
        std::cout << "parseTopLevel - current token: " 
                  << tokenTypeToString(currentToken_.type) 
                  << " [" << currentToken_.value << "]" << std::endl;
        return CHTLParser::parseTopLevel();
    }
    
    std::shared_ptr<ast::ElementNode> parseElement() override {
        std::cout << "parseElement - current token: " 
                  << tokenTypeToString(currentToken_.type) 
                  << " [" << currentToken_.value << "]" << std::endl;
        return CHTLParser::parseElement();
    }
};

int main() {
    std::string source = R"(div {
    text { "Hello CHTL" }
})";
    
    std::cout << "Source:\n" << source << "\n\n";
    
    CHTLLexer lexer(source, "debug.chtl");
    DebugParser parser;
    CHTLContext context;
    
    auto ast = parser.parse(lexer, context);
    
    return 0;
}