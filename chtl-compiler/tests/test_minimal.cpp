#include <iostream>
#include <string>
#include "compiler/chtljs/chtljs_compiler.h"
#include "utils/common.h"

using namespace chtl;

int main() {
    CHTLJSCompiler compiler;
    
    // 最简单的代码
    {
        std::cout << "测试：分号\n";
        std::cout << "输入代码：;\n";
        
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL_JS;
        fragment.content = ";";
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
    
    // 数字
    {
        std::cout << "测试：数字\n";
        std::cout << "输入代码：123;\n";
        
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL_JS;
        fragment.content = "123;";
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
    
    // 标识符
    {
        std::cout << "测试：标识符\n";
        std::cout << "输入代码：x;\n";
        
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL_JS;
        fragment.content = "x;";
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