#include "scanner/scanner.h"
#include <iostream>
#include <cassert>

using namespace chtl;

void testBasicScanning() {
    std::cout << "Testing basic scanning...\n";
    
    std::string chtl_code = R"(
div {
    id: main;
    class: container;
    
    style {
        width: 100%;
        height: 100vh;
    }
    
    text {
        "Hello CHTL!"
    }
    
    script {
        console.log("CHTL Works!");
    }
}
)";
    
    CHTLUnifiedScanner scanner;
    auto fragments = scanner.scan(chtl_code);
    
    assert(fragments.size() > 0);
    std::cout << "Found " << fragments.size() << " fragments\n";
    
    for (const auto& fragment : fragments) {
        std::cout << "Fragment type: " << static_cast<int>(fragment.type)
                  << ", lines: " << fragment.line_start << "-" << fragment.line_end << "\n";
    }
    
    std::cout << "Basic scanning test passed!\n\n";
}

void testCHTLJSDetection() {
    std::cout << "Testing CHTL JS detection...\n";
    
    std::string chtl_js_code = R"(
script {
    {{button}}->addEventListener('click', () => {
        console.log('Button clicked!');
    });
}
)";
    
    CHTLUnifiedScanner scanner;
    auto fragments = scanner.scan(chtl_js_code);
    
    bool found_chtl_js = false;
    for (const auto& fragment : fragments) {
        if (fragment.type == CodeFragmentType::CHTL_JS) {
            found_chtl_js = true;
            std::cout << "Found CHTL JS fragment\n";
        }
    }
    
    assert(found_chtl_js);
    std::cout << "CHTL JS detection test passed!\n\n";
}

int main() {
    std::cout << "Running Scanner Tests...\n\n";
    
    try {
        testBasicScanning();
        testCHTLJSDetection();
        
        std::cout << "All scanner tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}