#include <iostream>
#include "compiler/chtl/chtl_lexer.h"
#include "compiler/chtl/chtl_parser.h"
#include "compiler/chtl/chtl_compiler.h"

using namespace chtl;
using namespace chtl::compiler;

void printAST(std::shared_ptr<ast::ASTNode> node, int depth = 0) {
    if (!node) return;
    
    std::string indent(depth * 2, ' ');
    
    std::cout << indent << "Node Type: " << static_cast<int>(node->type) << std::endl;
    
    switch (node->type) {
        case ast::NodeType::DOCUMENT: {
            auto doc = std::static_pointer_cast<ast::DocumentNode>(node);
            std::cout << indent << "Document" << std::endl;
            for (const auto& child : doc->children) {
                printAST(child, depth + 1);
            }
            break;
        }
        case ast::NodeType::ELEMENT: {
            auto elem = std::static_pointer_cast<ast::ElementNode>(node);
            std::cout << indent << "Element: " << elem->tagName << std::endl;
            std::cout << indent << "  ID: " << elem->id << std::endl;
            std::cout << indent << "  Class: " << elem->className << std::endl;
            for (const auto& child : elem->children) {
                printAST(child, depth + 1);
            }
            break;
        }
        case ast::NodeType::TEXT: {
            auto text = std::static_pointer_cast<ast::TextNode>(node);
            std::cout << indent << "Text: \"" << text->content << "\"" << std::endl;
            break;
        }
        default:
            std::cout << indent << "Unknown node type" << std::endl;
    }
}

int main() {
    std::string source = R"(div {
    text { "Hello CHTL" }
})";
    
    std::cout << "Source:\n" << source << "\n\n";
    
    CHTLLexer lexer(source, "debug.chtl");
    CHTLParser parser;
    CHTLContext context;
    
    auto ast = parser.parse(lexer, context);
    
    if (ast) {
        std::cout << "AST:\n";
        printAST(ast);
    } else {
        std::cout << "Failed to parse\n";
    }
    
    // Also check parser errors
    auto errors = parser.getErrors();
    if (!errors.empty()) {
        std::cout << "\nParser errors:\n";
        for (const auto& error : errors) {
            std::cout << "  " << error << "\n";
        }
    }
    
    return 0;
}