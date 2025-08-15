#include <iostream>
#include <cassert>
#include <string>

#include "scanner/unified_scanner.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include "compilers/chtl_compiler.hpp"

using namespace chtl;

void testBasicHTMLGeneration() {
    std::cout << "测试基本HTML生成..." << std::endl;
    
    std::string chtlCode = R"(div
{
    id: test;
    class: container;
    
    text
    {
        "Hello CHTL!"
    }
})";
    
    CHTLUnifiedScanner scanner;
    auto scanResult = scanner.scan(chtlCode);
    
    assert(scanResult.success);
    assert(scanResult.segments.size() == 1);
    assert(scanResult.segments[0].type == CodeSegmentType::CHTL);
    
    CompilerDispatcher dispatcher;
    auto compileResult = dispatcher.compile(scanResult.segments);
    
    assert(compileResult.success);
    assert(compileResult.output.find("<div") != std::string::npos);
    assert(compileResult.output.find("Hello CHTL!") != std::string::npos);
    assert(compileResult.output.find("id=\"test\"") != std::string::npos);
    assert(compileResult.output.find("class=\"container\"") != std::string::npos);
    
    std::cout << "✓ 基本HTML生成测试通过" << std::endl;
}

void testLocalStyleProcessing() {
    std::cout << "测试局部样式处理..." << std::endl;
    
    std::string chtlCode = R"(div
{
    style
    {
        width: 100px;
        height: 100px;
        background-color: red;
    }
    
    text
    {
        "Styled div"
    }
})";
    
    CHTLUnifiedScanner scanner;
    auto scanResult = scanner.scan(chtlCode);
    
    assert(scanResult.success);
    
    CompilerDispatcher dispatcher;
    auto compileResult = dispatcher.compile(scanResult.segments);
    
    assert(compileResult.success);
    assert(compileResult.output.find("style=") != std::string::npos);
    assert(compileResult.output.find("width: 100px") != std::string::npos);
    assert(compileResult.output.find("Styled div") != std::string::npos);
    
    std::cout << "✓ 局部样式处理测试通过" << std::endl;
}

void testNestedElements() {
    std::cout << "测试嵌套元素..." << std::endl;
    
    std::string chtlCode = R"(div
{
    class: parent;
    
    span
    {
        text
        {
            "Child element"
        }
    }
    
    div
    {
        class: nested;
        
        text
        {
            "Nested div"
        }
    }
})";
    
    CHTLUnifiedScanner scanner;
    auto scanResult = scanner.scan(chtlCode);
    
    assert(scanResult.success);
    
    CompilerDispatcher dispatcher;
    auto compileResult = dispatcher.compile(scanResult.segments);
    
    assert(compileResult.success);
    assert(compileResult.output.find("<div") != std::string::npos);
    assert(compileResult.output.find("<span>") != std::string::npos);
    assert(compileResult.output.find("Child element") != std::string::npos);
    assert(compileResult.output.find("Nested div") != std::string::npos);
    
    std::cout << "✓ 嵌套元素测试通过" << std::endl;
}

void testTextNode() {
    std::cout << "测试文本节点..." << std::endl;
    
    std::string chtlCode = R"(text
{
    "Pure text content"
})";
    
    CHTLUnifiedScanner scanner;
    auto scanResult = scanner.scan(chtlCode);
    
    assert(scanResult.success);
    assert(scanResult.segments[0].type == CodeSegmentType::TEXT);
    
    CompilerDispatcher dispatcher;
    auto compileResult = dispatcher.compile(scanResult.segments);
    
    if (!compileResult.success) {
        std::cout << "编译失败: ";
        for (const auto& error : compileResult.errors) {
            std::cout << error << " ";
        }
        std::cout << std::endl;
    }
    
    assert(compileResult.success);
    assert(compileResult.output.find("Pure text content") != std::string::npos);
    
    std::cout << "✓ 文本节点测试通过" << std::endl;
}

int main() {
    std::cout << "=== CHTL编译器测试套件 ===" << std::endl;
    
    try {
        testBasicHTMLGeneration();
        testLocalStyleProcessing();
        testNestedElements();
        testTextNode();
        
        std::cout << "\n🎉 所有测试通过！CHTL编译器工作正常。" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ 未知错误发生" << std::endl;
        return 1;
    }
}