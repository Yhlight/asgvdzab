#include <iostream>
#include <string>
#include "compiler/chtljs/chtljs_compiler.h"
#include "utils/common.h"

using namespace chtl;

int main() {
    CHTLJSCompiler compiler;
    
    // 测试1：简单选择器
    {
        std::cout << "测试1：简单选择器\n";
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL_JS;
        fragment.content = "{{testDiv}};";
        fragment.start_line = 1;
        fragment.end_line = 1;
        
        CompilerOptions options;
        auto result = compiler.compile(fragment, options);
        
        if (result.success) {
            std::cout << "编译成功！\n";
            std::cout << "输出：\n" << result.output << "\n";
        } else {
            std::cout << "编译失败！\n";
            for (const auto& error : result.errors) {
                std::cout << "错误：" << error << "\n";
            }
        }
        std::cout << "\n";
    }
    
    // 测试2：类选择器
    {
        std::cout << "测试2：类选择器\n";
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL_JS;
        fragment.content = "var element = {{.myClass}};";
        fragment.start_line = 1;
        fragment.end_line = 1;
        
        CompilerOptions options;
        auto result = compiler.compile(fragment, options);
        
        if (result.success) {
            std::cout << "编译成功！\n";
            std::cout << "输出：\n" << result.output << "\n";
        } else {
            std::cout << "编译失败！\n";
            for (const auto& error : result.errors) {
                std::cout << "错误：" << error << "\n";
            }
        }
        std::cout << "\n";
    }
    
    // 测试3：成员访问
    {
        std::cout << "测试3：成员访问\n";
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL_JS;
        fragment.content = "{{#myId}}.style.display = 'block';";
        fragment.start_line = 1;
        fragment.end_line = 1;
        
        CompilerOptions options;
        auto result = compiler.compile(fragment, options);
        
        if (result.success) {
            std::cout << "编译成功！\n";
            std::cout << "输出：\n" << result.output << "\n";
        } else {
            std::cout << "编译失败！\n";
            for (const auto& error : result.errors) {
                std::cout << "错误：" << error << "\n";
            }
        }
        std::cout << "\n";
    }
    
    // 测试4：箭头操作符
    {
        std::cout << "测试4：箭头操作符\n";
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL_JS;
        fragment.content = "{{.btn}}->addEventListener('click', function() { alert('test'); });";
        fragment.start_line = 1;
        fragment.end_line = 1;
        
        CompilerOptions options;
        auto result = compiler.compile(fragment, options);
        
        if (result.success) {
            std::cout << "编译成功！\n";
            std::cout << "输出：\n" << result.output << "\n";
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