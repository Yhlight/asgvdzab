#include "compilers/antlr_css_compiler.hpp"
#include "compilers/antlr_js_compiler.hpp"
#include <iostream>
#include <fstream>

using namespace chtl;

void testCSSCompiler() {
    std::cout << "=== ANTLR CSS编译器测试 ===" << std::endl;
    
    ANTLRCSSCompiler cssCompiler;
    std::cout << "✓ " << cssCompiler.getName() << " 创建成功" << std::endl;
    
    // 测试CSS
    std::string testCSS = R"(
        .header {
            color: #ff0000;
            background: blue;
            font-family: Arial, sans-serif;
        }
        
        #main-content {
            margin: 10px;
            padding: 5px;
        }
        
        button:hover {
            background-color: #008CBA;
        }
        
        div > p {
            font-size: 14px;
        }
    )";
    
    std::cout << "\n测试CSS代码:\n" << testCSS << std::endl;
    
    CSSCompileOptions options;
    options.extractSelectors = true;
    options.extractColors = true;
    options.validateStandardCompliance = true;
    options.minify = false;
    
    auto result = cssCompiler.compileCSS(testCSS, options);
    
    if (result.success) {
        std::cout << "✓ CSS编译成功!" << std::endl;
        std::cout << "选择器数量: " << result.selectors.size() << std::endl;
        std::cout << "规则数量: " << result.rules.size() << std::endl;
        std::cout << "颜色数量: " << result.colors.size() << std::endl;
        
        std::cout << "\n提取的选择器:" << std::endl;
        for (const auto& selector : result.selectors) {
            std::cout << "  - " << selector.originalText 
                      << " (类型: " << (int)selector.type 
                      << ", 权重: " << selector.specificity << ")" << std::endl;
        }
        
        std::cout << "\n提取的颜色:" << std::endl;
        for (const auto& color : result.colors) {
            std::cout << "  - " << color << std::endl;
        }
        
        if (!result.warnings.empty()) {
            std::cout << "\n警告:" << std::endl;
            for (const auto& warning : result.warnings) {
                std::cout << "  ⚠ " << warning << std::endl;
            }
        }
    } else {
        std::cout << "✗ CSS编译失败:" << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "  错误: " << error << std::endl;
        }
    }
}

void testJSCompiler() {
    std::cout << "\n=== ANTLR JavaScript编译器测试 ===" << std::endl;
    
    ANTLRJSCompiler jsCompiler;
    std::cout << "✓ " << jsCompiler.getName() << " 创建成功" << std::endl;
    
    // 测试JavaScript
    std::string testJS = R"(
        function calculateSum(a, b) {
            return a + b;
        }
        
        class MathHelper {
            constructor() {
                this.name = "MathHelper";
            }
            
            multiply(x, y) {
                return x * y;
            }
        }
        
        const pi = 3.14159;
        let radius = 5;
        var area = pi * radius * radius;
        
        function greetUser(name) {
            console.log("Hello, " + name + "!");
        }
    )";
    
    std::cout << "\n测试JavaScript代码:\n" << testJS << std::endl;
    
    JSCompileOptions options;
    options.extractFunctions = true;
    options.extractVariables = true;
    options.extractClasses = true;
    options.validateSyntax = true;
    
    auto result = jsCompiler.compileJS(testJS, options);
    
    if (result.success) {
        std::cout << "✓ JavaScript编译成功!" << std::endl;
        std::cout << "函数数量: " << result.functions.size() << std::endl;
        std::cout << "变量数量: " << result.variables.size() << std::endl;
        std::cout << "类数量: " << result.classes.size() << std::endl;
        
        std::cout << "\n提取的函数:" << std::endl;
        for (const auto& func : result.functions) {
            std::cout << "  - " << func << "()" << std::endl;
        }
        
        std::cout << "\n提取的变量:" << std::endl;
        for (const auto& var : result.variables) {
            std::cout << "  - " << var << std::endl;
        }
        
        std::cout << "\n提取的类:" << std::endl;
        for (const auto& cls : result.classes) {
            std::cout << "  - " << cls << std::endl;
        }
        
        if (!result.warnings.empty()) {
            std::cout << "\n警告:" << std::endl;
            for (const auto& warning : result.warnings) {
                std::cout << "  ⚠ " << warning << std::endl;
            }
        }
    } else {
        std::cout << "✗ JavaScript编译失败:" << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "  错误: " << error << std::endl;
        }
    }
}

int main() {
    try {
        std::cout << "🚀 ANTLR CSS和JavaScript编译器集成测试" << std::endl;
        std::cout << "使用ANTLR 4.13.1解析器\n" << std::endl;
        
        testCSSCompiler();
        testJSCompiler();
        
        std::cout << "\n🎉 测试完成!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "测试异常: " << e.what() << std::endl;
        return 1;
    }
}