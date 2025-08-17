#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <algorithm>

#include "scanner/CHTLUnifiedScanner.h"
#include "dispatcher/CompilerDispatcher.h"

using namespace CHTL;

void printUsage(const char* programName) {
    std::cout << "用法: " << programName << " <输入文件> [选项]" << std::endl;
    std::cout << "选项:" << std::endl;
    std::cout << "  -o <输出文件>    指定输出文件名（默认: output.html）" << std::endl;
    std::cout << "  -d, --debug      启用调试模式" << std::endl;
    std::cout << "  -h, --help       显示帮助信息" << std::endl;
    std::cout << std::endl;
    std::cout << "示例:" << std::endl;
    std::cout << "  " << programName << " input.chtl" << std::endl;
    std::cout << "  " << programName << " input.chtl -o output.html -d" << std::endl;
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }
    
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    
    return content;
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法创建输出文件: " + filename);
    }
    
    file << content;
}

int main(int argc, char* argv[]) {
    std::cout << "CHTL编译器 v1.0.0" << std::endl;
    std::cout << "基于C++17实现的超文本语言编译器" << std::endl;
    std::cout << std::endl;
    
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string inputFile;
    std::string outputFile = "output.html";
    bool debugMode = false;
    
    // 解析命令行参数
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-d" || arg == "--debug") {
            debugMode = true;
        } else if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (arg[0] != '-') {
            inputFile = arg;
        } else {
            std::cerr << "未知选项: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (inputFile.empty()) {
        std::cerr << "错误: 需要指定输入文件" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        // 读取输入文件
        std::cout << "读取输入文件: " << inputFile << std::endl;
        std::string sourceCode = readFile(inputFile);
        
        if (debugMode) {
            std::cout << "源代码长度: " << sourceCode.length() << " 字符" << std::endl;
        }
        
        // 创建统一扫描器
        auto scanner = std::make_unique<CHTLUnifiedScanner>();
        scanner->setDebugMode(debugMode);
        
        // 扫描代码片段
        std::cout << "扫描代码片段..." << std::endl;
        FragmentList fragments = scanner->scan(sourceCode, inputFile);
        
        // 检查扫描错误
        const auto& scanErrors = scanner->getErrors();
        if (!scanErrors.empty()) {
            std::cerr << "扫描错误:" << std::endl;
            for (const auto& error : scanErrors) {
                std::cerr << "  " << error << std::endl;
            }
        }
        
        if (debugMode) {
            std::cout << "找到 " << fragments.size() << " 个代码片段" << std::endl;
            for (size_t i = 0; i < fragments.size(); ++i) {
                const auto& fragment = fragments[i];
                std::cout << "  片段 " << (i + 1) << ": " << fragment->getTypeName() 
                         << " (行 " << fragment->position.startLine << "-" 
                         << fragment->position.endLine << ")" << std::endl;
            }
        }
        
        // 创建编译器调度器
        auto dispatcher = std::make_unique<CompilerDispatcher>();
        dispatcher->setDebugMode(debugMode);
        
        // 编译代码片段
        std::cout << "编译代码片段..." << std::endl;
        bool compileSuccess = dispatcher->compile(fragments, outputFile);
        
        // 获取编译结果
        const auto& results = dispatcher->getResults();
        const auto& allErrors = dispatcher->getAllErrors();
        const auto& allWarnings = dispatcher->getAllWarnings();
        
        // 显示警告
        if (!allWarnings.empty()) {
            std::cout << "编译警告:" << std::endl;
            for (const auto& warning : allWarnings) {
                std::cout << "  警告: " << warning << std::endl;
            }
        }
        
        // 检查编译错误
        if (!allErrors.empty()) {
            std::cerr << "编译错误:" << std::endl;
            for (const auto& error : allErrors) {
                std::cerr << "  错误: " << error << std::endl;
            }
        }
        
        if (compileSuccess) {
            // 获取最终输出
            const std::string& finalOutput = dispatcher->getFinalOutput();
            
            if (!finalOutput.empty()) {
                // 写入输出文件
                std::cout << "写入输出文件: " << outputFile << std::endl;
                writeFile(outputFile, finalOutput);
                
                std::cout << "编译成功!" << std::endl;
                std::cout << "输出文件大小: " << finalOutput.length() << " 字符" << std::endl;
            } else {
                std::cerr << "错误: 没有生成输出内容" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "编译失败" << std::endl;
            return 1;
        }
        
        if (debugMode) {
            std::cout << "编译统计:" << std::endl;
            std::cout << "  成功的编译结果: " << 
                std::count_if(results.begin(), results.end(), 
                             [](const auto& r) { return r->success; }) << std::endl;
            std::cout << "  失败的编译结果: " << 
                std::count_if(results.begin(), results.end(), 
                             [](const auto& r) { return !r->success; }) << std::endl;
            std::cout << "  总错误数: " << allErrors.size() << std::endl;
            std::cout << "  总警告数: " << allWarnings.size() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "运行时错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}