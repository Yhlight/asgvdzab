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
    
    std::cout << "CHTL编译器演示程序" << std::endl;
    std::cout << "==================" << std::endl;
    
    try {
        // 读取演示文件
        std::string sourceCode = Utils::FileUtils::readFile("demo.chtl");
        
        std::cout << "\n✓ 成功读取演示文件: demo.chtl" << std::endl;
        std::cout << "  文件大小: " << sourceCode.size() << " 字节" << std::endl;
        
        // 创建编译上下文
        auto context = std::make_shared<Core::Context>();
        
        // 设置编译选项
        Core::CompileOptions options;
        options.debugMode = false; // 关闭调试输出以保持演示清洁
        options.outputPath = "./output";
        context->setOptions(options);
        context->setCurrentFile("demo.chtl");
        
        // 创建编译器调度器
        auto dispatcher = Compiler::CompilerFactory::createDispatcher();
        
        std::cout << "\n✓ 编译器调度器初始化完成" << std::endl;
        
        // 开始编译
        std::cout << "\n🚀 开始编译..." << std::endl;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        auto result = dispatcher->compileSource(sourceCode, "demo.chtl", *context);
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        if (result && result->success) {
            std::cout << "✅ 编译成功!" << std::endl;
            
            // 显示统计信息
            const auto& stats = dispatcher->getStats();
            std::cout << "\n📊 编译统计:" << std::endl;
            std::cout << "  ├─ 处理片段: " << stats.totalFragments << " 个" << std::endl;
            std::cout << "  ├─ 成功编译: " << stats.successfulCompiles << " 个" << std::endl;
            std::cout << "  ├─ 失败编译: " << stats.failedCompiles << " 个" << std::endl;
            std::cout << "  └─ 编译耗时: " << duration.count() << "ms" << std::endl;
            
            std::cout << "\n📄 输出信息:" << std::endl;
            std::cout << "  ├─ 输出大小: " << result->output.size() << " 字节" << std::endl;
            
            // 保存结果到文件
            std::string outputFile = "output/demo_output.html";
            Utils::FileUtils::writeFile(outputFile, result->output);
            std::cout << "  └─ 保存位置: " << outputFile << std::endl;
            
            // 显示输出预览
            std::cout << "\n🔍 输出预览 (前300字符):" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            
            std::string preview = result->output;
            if (preview.size() > 300) {
                preview = preview.substr(0, 300) + "...";
            }
            
            // 替换换行符以便更好地显示
            for (char& ch : preview) {
                if (ch == '\n') {
                    std::cout << ch << "  ";
                } else {
                    std::cout << ch;
                }
            }
            
            std::cout << "\n----------------------------------------" << std::endl;
            
            // 检查是否有警告
            const auto& warnings = context->getWarnings();
            if (!warnings.empty()) {
                std::cout << "\n⚠️  编译警告 (" << warnings.size() << "):" << std::endl;
                for (const auto& warning : warnings) {
                    std::cout << "  └─ " << warning->line << ":" << warning->column 
                             << " " << warning->message << std::endl;
                }
            }
            
            std::cout << "\n🎉 演示完成！" << std::endl;
            std::cout << "\n💡 提示: 您可以在浏览器中打开 " << outputFile << " 查看结果" << std::endl;
            
        } else {
            std::cout << "❌ 编译失败!" << std::endl;
            
            if (result) {
                std::cout << "\n编译器错误:" << std::endl;
                for (const auto& error : result->errors) {
                    std::cout << "  └─ " << error->message << std::endl;
                }
            }
            
            const auto& errors = context->getErrors();
            if (!errors.empty()) {
                std::cout << "\n上下文错误:" << std::endl;
                for (const auto& error : errors) {
                    std::cout << "  └─ " << error->file << ":" 
                             << error->line << ":" << error->column 
                             << " " << error->message << std::endl;
                }
            }
            
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 演示失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}