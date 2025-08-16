#include <iostream>
#include <cassert>
#include "compiler/chtl/chtl_compiler.h"

using namespace chtl;

void testBasicCompilation() {
    compiler::CHTLCompiler compiler;
    
    CodeSegment segment;
    segment.type = CodeSegmentType::CHTL;
    segment.content = R"(
div {
    id: main;
    class: container;
    
    text {
        "Hello CHTL"
    }
}
)";
    
    auto result = compiler.compile(segment);
    
    assert(result.success);
    assert(!result.output.empty());
    
    std::cout << "Compilation output:\n" << result.output << "\n";
}

int main() {
    std::cout << "Running CHTL compiler tests...\n";
    
    try {
        testBasicCompilation();
        std::cout << "All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}