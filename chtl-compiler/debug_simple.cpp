#include <iostream>
#include "compiler/chtl/chtl_lexer.h"
#include "compiler/chtl/chtl_parser.h"
#include "compiler/chtl/chtl_compiler.h"

using namespace chtl;
using namespace chtl::compiler;

int main() {
    std::string source = "div { text { \"Hello\" } }";
    
    std::cout << "=== Testing Simple CHTL ===" << std::endl;
    std::cout << "Source: " << source << std::endl << std::endl;
    
    // Test lexer first
    std::cout << "Lexer output:" << std::endl;
    CHTLLexer lexer(source);
    Token token;
    do {
        token = lexer.nextToken();
        std::cout << "  Token type: " << static_cast<int>(token.type) 
                  << ", value: [" << token.value << "]" << std::endl;
    } while (token.type != TokenType::EOF_TOKEN);
    
    // Now test the full compiler
    std::cout << "\nCompiler output:" << std::endl;
    CHTLCompiler compiler;
    CodeSegment segment;
    segment.type = CodeSegmentType::CHTL;
    segment.content = source;
    
    CompilerOptions options;
    auto result = compiler.compile(segment, options);
    
    if (result.success) {
        std::cout << "Success! Output: " << result.output << std::endl;
    } else {
        std::cout << "Failed!" << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "  Error: " << error << std::endl;
        }
    }
    
    return 0;
}