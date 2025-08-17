#include "compilers/antlr_css_compiler.hpp"
#include <iostream>

using namespace chtl;

int main() {
    try {
        std::cout << "=== 最小编译器测试 ===" << std::endl;
        
        std::cout << "创建编译器..." << std::endl;
        ANTLRCSSCompiler compiler;
        std::cout << "✓ 编译器创建成功" << std::endl;
        
        std::string simpleCSS = ".test { color: red; }";
        std::cout << "测试CSS: " << simpleCSS << std::endl;
        
        CSSCompileOptions options;
        options.extractSelectors = false;  // 先关闭可能有问题的特性
        options.extractColors = false;
        options.validateStandardCompliance = false;
        
        std::cout << "开始编译..." << std::endl;
        auto result = compiler.compileCSS(simpleCSS, options);
        
        if (result.success) {
            std::cout << "✓ 编译成功!" << std::endl;
            std::cout << "输出CSS: " << result.css << std::endl;
        } else {
            std::cout << "✗ 编译失败" << std::endl;
            for (const auto& error : result.errors) {
                std::cout << "错误: " << error << std::endl;
            }
        }
        
        return result.success ? 0 : 1;
        
    } catch (const std::exception& e) {
        std::cerr << "异常: " << e.what() << std::endl;
        return 1;
    }
}