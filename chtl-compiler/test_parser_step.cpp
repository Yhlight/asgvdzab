#include <iostream>
#include "compiler/chtl/chtl_lexer.h"
#include "compiler/chtl/chtl_parser.h"
#include "compiler/chtl/chtl_compiler.h"

using namespace chtl;
using namespace chtl::compiler;

class StepParser : public CHTLParser {
public:
    std::shared_ptr<ast::DocumentNode> parse(CHTLLexer& lexer, CHTLContext& context) override {
        lexer_ = &lexer;
        errors_.clear();
        warnings_.clear();
        
        auto document = std::make_shared<ast::DocumentNode>();
        document->location = SourceLocation();
        
        // 获取第一个token
        currentToken_ = lexer_->nextToken();
        std::cout << "Initial token: type=" << static_cast<int>(currentToken_.type) 
                  << " value=[" << currentToken_.value << "]" << std::endl;
        
        // 解析顶层元素
        while (!isAtEnd()) {
            auto node = parseTopLevel();
            if (node) {
                document->children.push_back(node);
            }
        }
        
        return document;
    }
    
    std::shared_ptr<ast::ElementNode> parseElement() override {
        std::cout << "parseElement called with token: type=" << static_cast<int>(currentToken_.type) 
                  << " value=[" << currentToken_.value << "]" << std::endl;
        
        auto element = std::make_shared<ast::ElementNode>();
        element->location = currentToken_.location;
        
        // 解析元素名（当前token应该是IDENTIFIER）
        if (check(TokenType::IDENTIFIER)) {
            element->tagName = advance().value;
            std::cout << "  Element name: " << element->tagName << std::endl;
            std::cout << "  After advance: type=" << static_cast<int>(currentToken_.type) 
                      << " value=[" << currentToken_.value << "]" << std::endl;
        } else {
            std::cout << "  ERROR: Expected element name but got type=" 
                      << static_cast<int>(currentToken_.type) << std::endl;
            error("Expected element name");
            // 使用当前token的值作为tagName（这就是问题所在）
            element->tagName = currentToken_.value;
            return element;
        }
        
        // 继续解析...
        return CHTLParser::parseElement();
    }
};

int main() {
    std::string source = "div { text { \"Hello\" } }";
    
    std::cout << "=== Step-by-step Parser Debug ===" << std::endl;
    std::cout << "Source: " << source << std::endl << std::endl;
    
    CHTLLexer lexer(source);
    StepParser parser;
    CHTLContext context;
    
    auto ast = parser.parse(lexer, context);
    
    return 0;
}