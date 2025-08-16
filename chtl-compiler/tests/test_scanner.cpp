#include <iostream>
#include <cassert>
#include "scanner/unified_scanner.h"

using namespace chtl;

void testBasicScanning() {
    scanner::UnifiedScanner scanner;
    
    std::string source = R"(
html {
    head {
        style {
            body {
                margin: 0;
            }
        }
    }
    
    body {
        div {
            style {
                .box {
                    width: 100px;
                }
            }
            
            text {
                "Hello World"
            }
        }
    }
}
)";
    
    auto segments = scanner.scan(source, "test.chtl");
    
    assert(!segments.empty());
    std::cout << "Found " << segments.size() << " segments\n";
    
    for (const auto& segment : segments) {
        std::cout << "Segment type: " << static_cast<int>(segment.type) 
                  << ", Content length: " << segment.content.length() << "\n";
    }
}

int main() {
    std::cout << "Running scanner tests...\n";
    
    try {
        testBasicScanning();
        std::cout << "All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}