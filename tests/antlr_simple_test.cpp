#include "compilers/antlr_css_compiler.hpp"
#include <iostream>

using namespace chtl;

int main() {
    try {
        std::cout << "创建ANTLRCSSCompiler..." << std::endl;
        ANTLRCSSCompiler compiler;
        std::cout << "✓ 编译器创建成功" << std::endl;
        
        std::cout << "测试getName()..." << std::endl;
        std::string name = compiler.getName();
        std::cout << "编译器名称: " << name << std::endl;
        
        std::cout << "测试getSupportedTypes()..." << std::endl;
        auto types = compiler.getSupportedTypes();
        std::cout << "支持的类型数量: " << types.size() << std::endl;
        
        std::cout << "测试简单CSS..." << std::endl;
        std::string simpleCSS = ".test { color: red; }";
        
        CSSCompileOptions options;
        options.extractSelectors = false;
        options.extractColors = false;
        options.extractFonts = false;
        options.validateStandardCompliance = false;
        options.validateSyntax = false;
        
        std::cout << "开始编译..." << std::endl;
        auto result = compiler.compileCSS(simpleCSS, options);
        std::cout << "编译完成。结果: " << (result.success ? "成功" : "失败") << std::endl;
        
        if (!result.success) {
            for (const auto& error : result.errors) {
                std::cout << "错误: " << error << std::endl;
            }
        }
        
        std::cout << "✓ 所有基础测试通过" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "异常: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "未知异常" << std::endl;
        return 1;
    }
}