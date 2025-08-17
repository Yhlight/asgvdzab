#include <iostream>
#include <string>
#include "compiler/chtljs/chtljs_compiler.h"
#include "utils/common.h"

using namespace chtl;

int main() {
    CHTLJSCompiler compiler;
    
    // 简单的var声明测试
    {
        std::cout << "测试：var声明\n";
        std::cout << "输入代码：var x = 1;\n";
        
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL_JS;
        fragment.content = "var x = 1;";
        fragment.start_line = 1;
        fragment.end_line = 1;
        
        CompilerOptions options;
        auto result = compiler.compile(fragment, options);
        
        if (result.success) {
            std::cout << "编译成功！\n";
            std::cout << "输出：" << result.output << "\n";
        } else {
            std::cout << "编译失败！\n";
            for (const auto& error : result.errors) {
                std::cout << "错误：" << error << "\n";
            }
        }
        std::cout << "\n";
    }
    
    // var声明带选择器
    {
        std::cout << "测试：var声明带选择器\n";
        std::cout << "输入代码：var element = {{.myClass}};\n";
        
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL_JS;
        fragment.content = "var element = {{.myClass}};";
        fragment.start_line = 1;
        fragment.end_line = 1;
        
        CompilerOptions options;
        auto result = compiler.compile(fragment, options);
        
        if (result.success) {
            std::cout << "编译成功！\n";
            std::cout << "输出：" << result.output << "\n";
        } else {
            std::cout << "编译失败！\n";
            for (const auto& error : result.errors) {
                std::cout << "错误：" << error << "\n";
            }
        }
        std::cout << "\n";
    }
    
    return 0;
}