#include <iostream>
#include <memory>

// 包含必要的头文件
#include "src/Core/Token.cpp"
#include "src/Core/Context.cpp"
#include "src/Scanner/UnifiedScanner.cpp"
#include "src/Compiler/CompilerDispatcher.cpp"
#include "src/Utils/FileUtils.cpp"

int main() {
    using namespace CHTL;
    
    std::cout << "测试CHTL编译器调度器..." << std::endl;
    
    try {
        // 创建编译上下文
        auto context = std::make_shared<Core::Context>();
        
        // 设置编译选项
        Core::CompileOptions options;
        options.debugMode = true;
        context->setOptions(options);
        context->setCurrentFile("test.chtl");
        
        // 创建编译器调度器
        auto dispatcher = Compiler::CompilerFactory::createDispatcher();
        dispatcher->enableDebug(true);
        
        // 测试CHTL代码
        std::string testCode = R"(
html
{
    head
    {
        
    }
    
    body
    {
        div
        {
            id: main-container;
            class: container;
            
            style
            {
                width: 100%;
                max-width: 1200px;
                margin: 0 auto;
                
                .box
                {
                    background-color: #f0f0f0;
                    padding: 20px;
                    border-radius: 5px;
                }
            }
            
            script
            {
                {{.box}}.addEventListener('click', function() {
                    console.log('Box clicked!');
                });
            }
            
            text
            {
                欢迎使用CHTL编译器！
            }
        }
    }
}
)";
        
        std::cout << "\n编译测试代码..." << std::endl;
        std::cout << "源码长度: " << testCode.size() << " 字节" << std::endl;
        
        // 编译源码
        auto result = dispatcher->compileSource(testCode, "test.chtl", *context);
        
        if (result && result->success) {
            std::cout << "\n编译成功!" << std::endl;
            std::cout << "输出长度: " << result->output.size() << " 字节" << std::endl;
            
            // 显示编译结果的前500个字符
            std::string preview = result->output;
            if (preview.size() > 500) {
                preview = preview.substr(0, 500) + "...";
            }
            
            std::cout << "\n编译结果预览:" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            std::cout << preview << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            
            // 显示统计信息
            const auto& stats = dispatcher->getStats();
            std::cout << "\n编译统计:" << std::endl;
            std::cout << "  总片段数: " << stats.totalFragments << std::endl;
            std::cout << "  成功编译: " << stats.successfulCompiles << std::endl;
            std::cout << "  失败编译: " << stats.failedCompiles << std::endl;
            std::cout << "  总耗时: " << stats.totalTime.count() << "ms" << std::endl;
            
            // 保存结果到文件
            try {
                Utils::FileUtils::writeFile("output/test_output.html", result->output);
                std::cout << "\n结果已保存到: output/test_output.html" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "\n保存文件失败: " << e.what() << std::endl;
            }
            
        } else {
            std::cout << "\n编译失败!" << std::endl;
            
            if (result) {
                for (const auto& error : result->errors) {
                    std::cout << "错误: " << error->message << std::endl;
                }
            }
            
            const auto& errors = context->getErrors();
            for (const auto& error : errors) {
                std::cout << "上下文错误: " << error->file << ":" 
                         << error->line << ":" << error->column 
                         << " " << error->message << std::endl;
            }
            
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n编译器测试完成!" << std::endl;
    return 0;
}