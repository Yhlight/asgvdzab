#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <iomanip>
#include <regex>
#include <fstream>
#include <sstream>

#include "scanner/CHTLUnifiedScanner.h"
#include "dispatcher/CompilerDispatcher.h"
#include "compiler/CHTLCompiler.h"
#include "compiler/CHTLJSCompiler.h"
#include "compiler/CSSCompiler.h"
#include "compiler/JavaScriptCompiler.h"

using namespace chtl;

void printUsage() {
    std::cout << "CHTL编译器 v1.0.0\n";
    std::cout << "用法: chtl_compiler <输入文件> [输出文件] [选项]\n";
    std::cout << "\n选项:\n";
    std::cout << "  -h, --help     显示此帮助信息\n";
    std::cout << "  -v, --version  显示版本信息\n";
    std::cout << "  -o <文件>      指定输出文件\n";
    std::cout << "  -d, --debug    启用调试模式\n";
    std::cout << "  -m, --minify   压缩输出\n";
    std::cout << "  -f, --format   格式化输出\n";
    std::cout << "\n示例:\n";
    std::cout << "  chtl_compiler input.chtl\n";
    std::cout << "  chtl_compiler input.chtl -o output.html\n";
    std::cout << "  chtl_compiler input.chtl -o output.html -m -f\n";
}

void printVersion() {
    std::cout << "CHTL编译器 v1.0.0\n";
    std::cout << "基于C++17实现\n";
    std::cout << "支持CHTL、CHTL JS、CSS、JavaScript编译\n";
}

void printStats(const CHTLUnifiedScanner::ScanStats& scanStats, 
                const CompilerDispatcher::CompilationStats& compileStats) {
    std::cout << "\n=== 编译统计信息 ===\n";
    
    std::cout << "扫描统计:\n";
    std::cout << "  总片段数: " << scanStats.totalFragments << "\n";
    std::cout << "  CHTL片段: " << scanStats.chtlFragments << "\n";
    std::cout << "  CHTL JS片段: " << scanStats.chtlJsFragments << "\n";
    std::cout << "  CSS片段: " << scanStats.cssFragments << "\n";
    std::cout << "  JavaScript片段: " << scanStats.javascriptFragments << "\n";
    std::cout << "  总行数: " << scanStats.totalLines << "\n";
    
    std::cout << "\n编译统计:\n";
    std::cout << "  总片段数: " << compileStats.totalFragments << "\n";
    std::cout << "  成功编译: " << compileStats.successfulCompilations << "\n";
    std::cout << "  编译失败: " << compileStats.failedCompilations << "\n";
    std::cout << "  总错误数: " << compileStats.totalErrors << "\n";
    std::cout << "  总警告数: " << compileStats.totalWarnings << "\n";
    std::cout << "  总编译时间: " << compileStats.totalTime.count() << "ms\n";
}

int main(int argc, char* argv[]) {
    // 解析命令行参数
    std::string inputFile;
    std::string outputFile;
    bool debugMode = false;
    bool minify = false;
    bool format = false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage();
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            printVersion();
            return 0;
        } else if (arg == "-o") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                std::cerr << "错误: -o 选项需要指定文件名\n";
                return 1;
            }
        } else if (arg == "-d" || arg == "--debug") {
            debugMode = true;
        } else if (arg == "-m" || arg == "--minify") {
            minify = true;
        } else if (arg == "-f" || arg == "--format") {
            format = true;
        } else if (arg[0] != '-') {
            if (inputFile.empty()) {
                inputFile = arg;
            } else if (outputFile.empty()) {
                outputFile = arg;
            }
        }
    }
    
    // 检查输入文件
    if (inputFile.empty()) {
        std::cerr << "错误: 请指定输入文件\n";
        printUsage();
        return 1;
    }
    
    // 设置默认输出文件
    if (outputFile.empty()) {
        outputFile = inputFile.substr(0, inputFile.find_last_of('.')) + ".html";
    }
    
    if (debugMode) {
        std::cout << "调试模式已启用\n";
        std::cout << "输入文件: " << inputFile << "\n";
        std::cout << "输出文件: " << outputFile << "\n";
        std::cout << "压缩输出: " << (minify ? "是" : "否") << "\n";
        std::cout << "格式化输出: " << (format ? "是" : "否") << "\n";
    }
    
    try {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // 创建CHTL统一扫描器
        CHTLUnifiedScanner scanner;
        
        if (debugMode) {
            std::cout << "\n开始扫描CHTL源代码...\n";
        }
        
        // 扫描源代码文件
        auto fragments = scanner.scanFile(inputFile);
        
        if (fragments.empty()) {
            std::cerr << "错误: 无法扫描源代码文件或文件为空\n";
            return 1;
        }
        
        if (debugMode) {
            std::cout << "扫描完成，共发现 " << fragments.size() << " 个代码片段\n";
        }
        
        // 创建编译器调度器
        CompilerDispatcher dispatcher;
        
        // 注册各种编译器
        dispatcher.registerCompiler(FragmentType::CHTL, std::make_unique<CHTLCompiler>());
        dispatcher.registerCompiler(FragmentType::CHTL_JS, std::make_unique<CHTLJSCompiler>());
        dispatcher.registerCompiler(FragmentType::CSS, std::make_unique<CSSCompiler>());
        dispatcher.registerCompiler(FragmentType::JAVASCRIPT, std::make_unique<JavaScriptCompiler>());
        
        if (debugMode) {
            std::cout << "编译器注册完成，开始编译...\n";
        }
        
        // 编译所有片段
        auto results = dispatcher.compileAllFragments(fragments);
        
        if (debugMode) {
            std::cout << "编译完成，开始合并结果...\n";
        }
        
        // 合并编译结果
        std::string htmlOutput = dispatcher.mergeResults(results);
        
        // 应用输出选项
        if (minify) {
            // 压缩HTML输出
            std::regex whitespaceRegex(R"(\s+)");
            htmlOutput = std::regex_replace(htmlOutput, whitespaceRegex, " ");
            htmlOutput = std::regex_replace(htmlOutput, std::regex(R"(>\s+<)"), "><");
        }
        
        if (format && !minify) {
            // 格式化HTML输出
            std::string formatted;
            std::istringstream stream(htmlOutput);
            std::string line;
            int indentLevel = 0;
            
            while (std::getline(stream, line)) {
                if (line.find("</") == 0) {
                    indentLevel = std::max(0, indentLevel - 1);
                }
                
                if (!line.empty()) {
                    formatted += std::string(indentLevel * 2, ' ') + line + "\n";
                }
                
                if (line.find("<") == 0 && line.find("</") != 0 && line.find("/>") == std::string::npos) {
                    indentLevel++;
                }
            }
            htmlOutput = formatted;
        }
        
        // 写入输出文件
        std::ofstream output(outputFile);
        if (!output.is_open()) {
            std::cerr << "错误: 无法创建输出文件: " << outputFile << "\n";
            return 1;
        }
        
        output << htmlOutput;
        output.close();
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        // 获取统计信息
        auto scanStats = scanner.getScanStats();
        auto compileStats = dispatcher.getCompilationStats();
        
        // 显示结果
        std::cout << "\n编译完成！\n";
        std::cout << "输出文件: " << outputFile << "\n";
        std::cout << "总耗时: " << duration.count() << "ms\n";
        
        if (debugMode) {
            printStats(scanStats, compileStats);
        }
        
        // 检查是否有错误
        auto errors = scanner.getErrors();
        auto compileErrors = dispatcher.getErrors();
        
        if (!errors.empty() || !compileErrors.empty()) {
            std::cout << "\n=== 警告和错误 ===\n";
            
            for (const auto& error : errors) {
                std::cout << "扫描警告: " << error << "\n";
            }
            
            for (const auto& error : compileErrors) {
                std::cout << "编译警告: " << error << "\n";
            }
        }
        
        std::cout << "\nCHTL源代码已成功编译为HTML文件！\n";
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}