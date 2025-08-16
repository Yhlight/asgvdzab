#include "compilers/modern_antlr_css_compiler.hpp"
#include <iostream>

using namespace chtl;

int main() {
    try {
        std::cout << "=== 现代ANTLR CSS编译器测试 ===" << std::endl;
        
        ModernANTLRCSSCompiler compiler;
        std::cout << "✓ " << compiler.getName() << " 创建成功" << std::endl;
        
        // 测试简单CSS
        std::string simpleCSS = ".test { color: red; background: #ffffff; }";
        std::cout << "\n测试CSS: " << simpleCSS << std::endl;
        
        CSSCompileOptions options;
        options.extractSelectors = true;
        options.extractColors = true;
        options.validateStandardCompliance = true;
        
        auto result = compiler.compileCSS(simpleCSS, options);
        
        if (result.success) {
            std::cout << "✓ ANTLR编译成功!" << std::endl;
            std::cout << "选择器数量: " << result.selectors.size() << std::endl;
            std::cout << "规则数量: " << result.rules.size() << std::endl;
            std::cout << "颜色数量: " << result.colors.size() << std::endl;
            
            for (const auto& selector : result.selectors) {
                std::cout << "  选择器: " << selector.originalText 
                          << " (类型: " << (int)selector.type 
                          << ", 权重: " << selector.specificity << ")" << std::endl;
            }
            
            for (const auto& color : result.colors) {
                std::cout << "  颜色: " << color << std::endl;
            }
            
            if (!result.warnings.empty()) {
                std::cout << "警告:" << std::endl;
                for (const auto& warning : result.warnings) {
                    std::cout << "  - " << warning << std::endl;
                }
            }
        } else {
            std::cout << "✗ ANTLR编译失败:" << std::endl;
            for (const auto& error : result.errors) {
                std::cout << "  错误: " << error << std::endl;
            }
        }
        
        return result.success ? 0 : 1;
        
    } catch (const std::exception& e) {
        std::cerr << "测试异常: " << e.what() << std::endl;
        return 1;
    }
}