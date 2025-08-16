#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "dispatcher/CompilerDispatcher.hpp"
#include "utils/CommandLineParser.hpp"

namespace fs = std::filesystem;

void printUsage(const std::string& programName) {
    std::cout << "CHTL Compiler v1.0.0\n\n";
    std::cout << "Usage: " << programName << " [options] <input-file>\n\n";
    std::cout << "Options:\n";
    std::cout << "  -o, --output <dir>     指定输出目录\n";
    std::cout << "  -h, --help             显示帮助信息\n";
    std::cout << "  -v, --version          显示版本信息\n";
    std::cout << "  --debug                启用调试模式\n";
    std::cout << "  --pack-cmod <dir>      打包CMOD模块\n";
    std::cout << "  --unpack-cmod <file>   解包CMOD文件\n";
    std::cout << "  --pack-cjmod <dir>     打包CJMOD模块\n";
    std::cout << "  --unpack-cjmod <file>  解包CJMOD文件\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    CommandLineParser parser(argc, argv);
    
    if (parser.hasFlag("--help") || parser.hasFlag("-h")) {
        printUsage(argv[0]);
        return 0;
    }
    
    if (parser.hasFlag("--version") || parser.hasFlag("-v")) {
        std::cout << "CHTL Compiler v1.0.0\n";
        std::cout << "Built with C++17\n";
        return 0;
    }
    
    // 处理CMOD打包/解包
    if (parser.hasOption("--pack-cmod")) {
        // TODO: 实现CMOD打包
        std::cerr << "CMOD打包功能尚未实现\n";
        return 1;
    }
    
    if (parser.hasOption("--unpack-cmod")) {
        // TODO: 实现CMOD解包
        std::cerr << "CMOD解包功能尚未实现\n";
        return 1;
    }
    
    // 获取输入文件
    auto inputFiles = parser.getPositionalArgs();
    if (inputFiles.empty()) {
        std::cerr << "错误：未指定输入文件\n";
        printUsage(argv[0]);
        return 1;
    }
    
    std::string inputFile = inputFiles[0];
    if (!fs::exists(inputFile)) {
        std::cerr << "错误：输入文件不存在：" << inputFile << "\n";
        return 1;
    }
    
    // 获取输出目录
    std::string outputDir = parser.getOption("--output", parser.getOption("-o", "output"));
    
    // 创建输出目录
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }
    
    // 设置调试模式
    bool debugMode = parser.hasFlag("--debug");
    
    try {
        // 创建编译器调度器
        CompilerDispatcher dispatcher(debugMode);
        
        // 编译文件
        std::cout << "正在编译：" << inputFile << "\n";
        bool success = dispatcher.compile(inputFile, outputDir);
        
        if (success) {
            std::cout << "编译成功！输出目录：" << outputDir << "\n";
            return 0;
        } else {
            std::cerr << "编译失败！\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "错误：" << e.what() << "\n";
        return 1;
    }
}