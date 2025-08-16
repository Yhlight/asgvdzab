#include <iostream>
#include "compiler/chtl/chtl_compiler.h"

using namespace chtl;
using namespace chtl::compiler;

int main() {
    // 最简单的CHTL代码
    std::string source = "p { }";
    
    std::cout << "=== Compiling: " << source << " ===" << std::endl;
    
    CHTLCompiler compiler;
    CodeSegment segment;
    segment.type = CodeSegmentType::CHTL;
    segment.content = source;
    segment.location.filename = "test.chtl";
    
    CompilerOptions options;
    auto result = compiler.compile(segment, options);
    
    std::cout << "\nResult:" << std::endl;
    std::cout << "  Success: " << (result.success ? "true" : "false") << std::endl;
    std::cout << "  Output: [" << result.output << "]" << std::endl;
    
    if (!result.errors.empty()) {
        std::cout << "  Errors:" << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "    " << error << std::endl;
        }
    }
    
    // 现在试试带内容的
    std::cout << "\n=== Compiling with text ===" << std::endl;
    source = "p { text { \"Hi\" } }";
    segment.content = source;
    
    result = compiler.compile(segment, options);
    
    std::cout << "Result:" << std::endl;
    std::cout << "  Success: " << (result.success ? "true" : "false") << std::endl;
    std::cout << "  Output: [" << result.output << "]" << std::endl;
    
    return 0;
}