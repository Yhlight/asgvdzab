#include "scanner/scanner.h"
#include "dispatcher/dispatcher.h"
#include "merger/merger.h"
#include <iostream>
#include <cassert>

using namespace chtl;

void testFullCompilation() {
    std::cout << "Testing full compilation...\n";
    
    std::string chtl_code = R"(
html {
    head {
        title {
            text { "CHTL Test Page" }
        }
    }
    
    body {
        div {
            id: container;
            
            style {
                .box {
                    width: 300px;
                    height: 200px;
                    background-color: #f0f0f0;
                    padding: 20px;
                }
                
                &:hover {
                    background-color: #e0e0e0;
                }
            }
            
            h1 {
                text { "Welcome to CHTL!" }
            }
            
            p {
                text { "This is a test of the CHTL compiler." }
            }
            
            button {
                id: testBtn;
                text { "Click Me" }
                
                script {
                    {{testBtn}}->addEventListener('click', () => {
                        alert('Button clicked!');
                    });
                }
            }
        }
    }
}
)";
    
    // 创建编译器组件
    CHTLUnifiedScanner scanner;
    CompilerDispatcher dispatcher;
    ResultMerger merger;
    
    // 扫描
    auto fragments = scanner.scan(chtl_code);
    std::cout << "Scanned " << fragments.size() << " fragments\n";
    
    // 编译
    CompileOptions options;
    options.debug_mode = true;
    
    auto results = dispatcher.dispatch(fragments, options);
    
    // 检查编译结果
    int success_count = 0;
    int fail_count = 0;
    for (const auto& result : results) {
        if (result.result.success) {
            success_count++;
        } else {
            fail_count++;
            std::cout << "Compile error: " << result.result.error_msg << "\n";
        }
    }
    
    std::cout << "Compilation: " << success_count << " success, " 
              << fail_count << " failed\n";
    
    assert(fail_count == 0);
    
    // 合并
    std::string html = merger.merge(results, options);
    
    // 基本验证
    assert(html.find("<!DOCTYPE html>") != std::string::npos);
    assert(html.find("<html") != std::string::npos);
    assert(html.find("</html>") != std::string::npos);
    assert(html.find("Welcome to CHTL!") != std::string::npos);
    
    std::cout << "Generated HTML size: " << html.size() << " bytes\n";
    std::cout << "Full compilation test passed!\n\n";
}

void testCHTLJSCompilation() {
    std::cout << "Testing CHTL JS compilation...\n";
    
    std::string chtl_js_code = R"(
script {
    {{.box}}->addEventListener('click', function(e) {
        {{#output}}->textContent = 'Box clicked!';
    });
    
    const anim = animate({
        target: {{button}},
        duration: 1000,
        begin: {
            opacity: 0
        },
        end: {
            opacity: 1
        }
    });
}
)";
    
    CHTLUnifiedScanner scanner;
    CompilerDispatcher dispatcher;
    
    auto fragments = scanner.scan(chtl_js_code);
    
    CompileOptions options;
    auto results = dispatcher.dispatch(fragments, options);
    
    bool found_js = false;
    for (const auto& result : results) {
        if (result.type == CodeFragmentType::CHTL_JS && result.result.success) {
            found_js = true;
            
            // 验证转换
            assert(result.result.output.find("document.querySelectorAll") != std::string::npos);
            assert(result.result.output.find(".addEventListener") != std::string::npos);
            std::cout << "CHTL JS successfully transformed\n";
        }
    }
    
    assert(found_js);
    std::cout << "CHTL JS compilation test passed!\n\n";
}

int main() {
    std::cout << "Running Compiler Tests...\n\n";
    
    try {
        testFullCompilation();
        testCHTLJSCompilation();
        
        std::cout << "All compiler tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}