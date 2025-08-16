#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

#include "scanner/unified_scanner.h"
#include "dispatcher/compiler_dispatcher.h"
#include "merger/result_merger.h"
#include "common/types.h"

// 版本信息
const char* VERSION = "1.0.0";
const char* PROGRAM_NAME = "chtlc";

// 显示帮助信息
void showHelp() {
    std::cout << "CHTL Compiler v" << VERSION << "\n\n";
    std::cout << "Usage: " << PROGRAM_NAME << " [options] <input-file>\n\n";
    std::cout << "Options:\n";
    std::cout << "  -o, --output <file>      Specify output file (default: output.html)\n";
    std::cout << "  -m, --minify             Minify CSS and JavaScript code\n";
    std::cout << "  -d, --debug              Enable debug mode\n";
    std::cout << "  -p, --parallel           Use parallel compilation\n";
    std::cout << "  -t, --title <title>      Set HTML page title\n";
    std::cout << "  -h, --help               Show this help message\n";
    std::cout << "  -v, --version            Show version information\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "  " << PROGRAM_NAME << " input.chtl\n";
    std::cout << "  " << PROGRAM_NAME << " -o index.html -m input.chtl\n";
    std::cout << "  " << PROGRAM_NAME << " --title \"My Page\" --minify input.chtl\n";
}

// 显示版本信息
void showVersion() {
    std::cout << "CHTL Compiler v" << VERSION << "\n";
    std::cout << "Copyright (c) 2024 CHTL Team\n";
    std::cout << "MIT License\n";
}

// 读取文件内容
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 写入文件
void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot write to file: " + filename);
    }
    
    file << content;
    file.close();
}

// 解析命令行参数
struct CommandLineOptions {
    std::string inputFile;
    std::string outputFile = "output.html";
    std::string title = "CHTL Output";
    bool minify = false;
    bool debug = false;
    bool parallel = false;
    bool showHelp = false;
    bool showVersion = false;
};

CommandLineOptions parseCommandLine(int argc, char* argv[]) {
    CommandLineOptions options;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            options.showHelp = true;
            return options;
        } else if (arg == "-v" || arg == "--version") {
            options.showVersion = true;
            return options;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                options.outputFile = argv[++i];
            } else {
                throw std::runtime_error("Missing argument for " + arg);
            }
        } else if (arg == "-t" || arg == "--title") {
            if (i + 1 < argc) {
                options.title = argv[++i];
            } else {
                throw std::runtime_error("Missing argument for " + arg);
            }
        } else if (arg == "-m" || arg == "--minify") {
            options.minify = true;
        } else if (arg == "-d" || arg == "--debug") {
            options.debug = true;
        } else if (arg == "-p" || arg == "--parallel") {
            options.parallel = true;
        } else if (arg[0] != '-') {
            options.inputFile = arg;
        } else {
            throw std::runtime_error("Unknown option: " + arg);
        }
    }
    
    if (options.inputFile.empty() && !options.showHelp && !options.showVersion) {
        throw std::runtime_error("No input file specified");
    }
    
    return options;
}

int main(int argc, char* argv[]) {
    try {
        // 解析命令行参数
        CommandLineOptions options = parseCommandLine(argc, argv);
        
        // 处理帮助和版本信息
        if (options.showHelp) {
            showHelp();
            return 0;
        }
        
        if (options.showVersion) {
            showVersion();
            return 0;
        }
        
        // 读取输入文件
        std::string sourceCode = readFile(options.inputFile);
        
        // 创建编译器组件
        auto scanner = std::make_unique<chtl::scanner::UnifiedScanner>();
        auto dispatcher = std::make_unique<chtl::dispatcher::CompilerDispatcher>();
        auto merger = std::make_unique<chtl::merger::ResultMerger>();
        
        // 设置调度策略
        if (options.parallel) {
            dispatcher->setStrategy(chtl::dispatcher::DispatchStrategy::PARALLEL);
        }
        
        // 设置编译选项
        chtl::CompilerOptions compilerOptions;
        compilerOptions.minify = options.minify;
        compilerOptions.debugMode = options.debug;
        compilerOptions.outputPath = options.outputFile;
        
        // 扫描代码
        if (options.debug) {
            std::cout << "Scanning CHTL source code...\n";
        }
        
        auto segments = scanner->scan(sourceCode, options.inputFile);
        
        if (!scanner->getErrors().empty()) {
            std::cerr << "Scanner errors:\n";
            for (const auto& error : scanner->getErrors()) {
                std::cerr << "  " << error << "\n";
            }
            return 1;
        }
        
        if (options.debug) {
            std::cout << "Found " << segments.size() << " code segments\n";
        }
        
        // 编译代码片段
        if (options.debug) {
            std::cout << "Compiling code segments...\n";
        }
        
        auto dispatchResult = dispatcher->dispatch(segments, compilerOptions);
        
        if (!dispatchResult.success) {
            std::cerr << "Compilation errors:\n";
            for (const auto& error : dispatchResult.errors) {
                std::cerr << "  " << error << "\n";
            }
            return 1;
        }
        
        if (!dispatchResult.warnings.empty()) {
            std::cerr << "Compilation warnings:\n";
            for (const auto& warning : dispatchResult.warnings) {
                std::cerr << "  " << warning << "\n";
            }
        }
        
        // 合并结果
        if (options.debug) {
            std::cout << "Merging compilation results...\n";
        }
        
        chtl::merger::HTMLOutputOptions htmlOptions;
        htmlOptions.title = options.title;
        htmlOptions.prettyPrint = !options.minify;
        
        std::string htmlOutput = merger->merge(dispatchResult, htmlOptions);
        
        if (!merger->getErrors().empty()) {
            std::cerr << "Merge errors:\n";
            for (const auto& error : merger->getErrors()) {
                std::cerr << "  " << error << "\n";
            }
            return 1;
        }
        
        // 写入输出文件
        if (options.debug) {
            std::cout << "Writing output to " << options.outputFile << "...\n";
        }
        
        writeFile(options.outputFile, htmlOutput);
        
        // 显示统计信息
        if (options.debug) {
            auto stats = dispatcher->getStatistics();
            std::cout << "\nCompilation statistics:\n";
            std::cout << "  Total segments: " << stats.totalSegments << "\n";
            std::cout << "  Total time: " << stats.totalTime << "s\n";
            for (const auto& [type, count] : stats.segmentCounts) {
                std::cout << "  " << static_cast<int>(type) << ": " << count << " segments\n";
            }
        }
        
        std::cout << "Compilation successful. Output written to " << options.outputFile << "\n";
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}