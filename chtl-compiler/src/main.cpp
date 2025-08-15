#include "chtl_compiler.h"
#include <iostream>
#include <vector>
#include <string>

void printUsage(const char* programName) {
    std::cout << "CHTL编译器 v" << chtl::CHTLCompilerSystem::getVersion() << "\n";
    std::cout << "用法: " << programName << " [选项] <输入文件>\n\n";
    std::cout << "选项:\n";
    std::cout << "  -o <文件>      指定输出文件 (默认: input.html)\n";
    std::cout << "  -m, --minify   压缩输出HTML\n";
    std::cout << "  -f, --format   格式化输出HTML (默认)\n";
    std::cout << "  -v, --verbose  显示详细日志\n";
    std::cout << "  -q, --quiet    静默模式\n";
    std::cout << "  -h, --help     显示帮助信息\n";
    std::cout << "  --version      显示版本信息\n\n";
    std::cout << "示例:\n";
    std::cout << "  " << programName << " input.chtl\n";
    std::cout << "  " << programName << " -o output.html input.chtl\n";
    std::cout << "  " << programName << " -m -o dist/index.html src/index.chtl\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    // 解析命令行参数
    std::string inputFile;
    std::string outputFile;
    bool minify = false;
    bool format = true;
    bool verbose = false;
    bool quiet = false;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--version") {
            std::cout << chtl::CHTLCompilerSystem::getBuildInfo() << "\n";
            return 0;
        } else if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (arg == "-m" || arg == "--minify") {
            minify = true;
            format = false;
        } else if (arg == "-f" || arg == "--format") {
            format = true;
            minify = false;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
            quiet = false;
        } else if (arg == "-q" || arg == "--quiet") {
            quiet = true;
            verbose = false;
        } else if (arg[0] != '-') {
            inputFile = arg;
        } else {
            std::cerr << "未知选项: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (inputFile.empty()) {
        std::cerr << "错误: 未指定输入文件\n";
        printUsage(argv[0]);
        return 1;
    }
    
    // 设置输出文件名
    if (outputFile.empty()) {
        // 默认输出文件名
        size_t dotPos = inputFile.find_last_of('.');
        if (dotPos != std::string::npos) {
            outputFile = inputFile.substr(0, dotPos) + ".html";
        } else {
            outputFile = inputFile + ".html";
        }
    }
    
    // 配置编译器
    chtl::CHTLSystemConfig config;
    config.outputPath = outputFile;
    config.writeToFile = true;
    
    // 设置合并器配置
    config.mergerConfig.minifyHTML = minify;
    config.mergerConfig.formatOutput = format;
    
    // 设置日志级别
    if (quiet) {
        config.logLevel = chtl::CHTLSystemConfig::NONE;
    } else if (verbose) {
        config.logLevel = chtl::CHTLSystemConfig::DEBUG;
    } else {
        config.logLevel = chtl::CHTLSystemConfig::INFO;
    }
    
    // 创建编译器实例
    chtl::CHTLCompilerSystem compiler(config);
    
    // 设置日志回调
    if (!quiet) {
        compiler.setLogCallback([](const std::string& level, const std::string& message) {
            if (level == "ERROR") {
                std::cerr << "[错误] " << message << "\n";
            } else if (level == "WARNING") {
                std::cerr << "[警告] " << message << "\n";
            } else {
                std::cout << "[" << level << "] " << message << "\n";
            }
        });
    }
    
    // 编译文件
    if (!quiet) {
        std::cout << "正在编译 " << inputFile << " -> " << outputFile << "\n";
    }
    
    auto result = compiler.compileFile(inputFile);
    
    // 输出结果
    if (result.success) {
        if (!quiet) {
            std::cout << "\n编译成功!\n";
            std::cout << "统计信息:\n";
            std::cout << "  输入大小: " << result.stats.inputSize << " 字节\n";
            std::cout << "  输出大小: " << result.stats.outputSize << " 字节\n";
            std::cout << "  片段数量: " << result.stats.fragmentCount << "\n";
            std::cout << "  总用时: " << result.stats.totalTime.count() << " ms\n";
            std::cout << "    扫描: " << result.stats.scanTime.count() << " ms\n";
            std::cout << "    编译: " << result.stats.compileTime.count() << " ms\n";
            std::cout << "    合并: " << result.stats.mergeTime.count() << " ms\n";
            
            if (!result.warnings.empty()) {
                std::cout << "\n警告 (" << result.warnings.size() << "):\n";
                for (const auto& warning : result.warnings) {
                    std::cout << "  - " << warning << "\n";
                }
            }
        }
        return 0;
    } else {
        if (!quiet) {
            std::cerr << "\n编译失败!\n";
            std::cerr << "错误 (" << result.errors.size() << "):\n";
            for (const auto& error : result.errors) {
                std::cerr << "  - " << error << "\n";
            }
        }
        return 1;
    }
}