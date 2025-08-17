#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <memory>

#include "Core/Types.h"
#include "Core/Context.h"
#include "Scanner/UnifiedScanner.h"
#include "Compiler/CompilerDispatcher.h"

using namespace CHTL;

// 命令行选项结构
struct CommandLineOptions {
    std::string inputFile;
    std::string outputFile;
    std::string outputDir = "./output";
    std::string moduleDir = "./modules";
    bool debugMode = false;
    bool verbose = false;
    bool showHelp = false;
    bool showVersion = false;
    bool generateSourceMap = false;
    bool minifyOutput = false;
    bool enableOptimization = true;
    std::vector<std::string> includePaths;
};

// 解析命令行参数
CommandLineOptions parseCommandLine(int argc, char* argv[]) {
    CommandLineOptions options;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            options.showHelp = true;
        } else if (arg == "-v" || arg == "--version") {
            options.showVersion = true;
        } else if (arg == "-d" || arg == "--debug") {
            options.debugMode = true;
        } else if (arg == "--verbose") {
            options.verbose = true;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                options.outputFile = argv[++i];
            }
        } else if (arg == "--output-dir") {
            if (i + 1 < argc) {
                options.outputDir = argv[++i];
            }
        } else if (arg == "--module-dir") {
            if (i + 1 < argc) {
                options.moduleDir = argv[++i];
            }
        } else if (arg == "--source-map") {
            options.generateSourceMap = true;
        } else if (arg == "--minify") {
            options.minifyOutput = true;
        } else if (arg == "--no-optimization") {
            options.enableOptimization = false;
        } else if (arg == "-I" || arg == "--include") {
            if (i + 1 < argc) {
                options.includePaths.push_back(argv[++i]);
            }
        } else if (arg.starts_with("-")) {
            std::cerr << "Unknown option: " << arg << std::endl;
            options.showHelp = true;
        } else {
            // 输入文件
            if (options.inputFile.empty()) {
                options.inputFile = arg;
            }
        }
    }
    
    return options;
}

// 显示帮助信息
void showHelp() {
    std::cout << "CHTL Compiler - CHTL超文本语言编译器\n\n";
    std::cout << "用法: chtl_compiler [选项] <输入文件>\n\n";
    std::cout << "选项:\n";
    std::cout << "  -h, --help              显示此帮助信息\n";
    std::cout << "  -v, --version           显示版本信息\n";
    std::cout << "  -d, --debug             启用调试模式\n";
    std::cout << "  --verbose               启用详细输出\n";
    std::cout << "  -o, --output <文件>     指定输出文件\n";
    std::cout << "  --output-dir <目录>     指定输出目录 (默认: ./output)\n";
    std::cout << "  --module-dir <目录>     指定模块目录 (默认: ./modules)\n";
    std::cout << "  --source-map            生成源码映射文件\n";
    std::cout << "  --minify                压缩输出\n";
    std::cout << "  --no-optimization       禁用优化\n";
    std::cout << "  -I, --include <路径>    添加包含路径\n\n";
    std::cout << "示例:\n";
    std::cout << "  chtl_compiler input.chtl\n";
    std::cout << "  chtl_compiler -d -o output.html input.chtl\n";
    std::cout << "  chtl_compiler --minify --source-map input.chtl\n";
}

// 显示版本信息
void showVersion() {
    std::cout << "CHTL Compiler v1.0.0\n";
    std::cout << "基于C++17构建的CHTL超文本语言编译器\n";
    std::cout << "Copyright (c) 2024 CHTL Team\n";
    std::cout << "License: MIT\n";
}

// 读取文件内容
std::string readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("无法打开文件: " + filename);
    }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::string content(size, '\0');
    file.read(&content[0], size);
    
    return content;
}

// 写入文件内容
void writeFile(const std::string& filename, const std::string& content) {
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("无法创建文件: " + filename);
    }
    
    file.write(content.c_str(), content.size());
}

// 生成输出文件名
std::string generateOutputFilename(const std::string& inputFile, 
                                  const std::string& outputFile,
                                  const std::string& outputDir) {
    if (!outputFile.empty()) {
        return outputFile;
    }
    
    std::filesystem::path inputPath(inputFile);
    std::string baseName = inputPath.stem().string();
    
    std::filesystem::path outputPath(outputDir);
    outputPath /= baseName + ".html";
    
    return outputPath.string();
}

// 主编译函数
int compileFile(const CommandLineOptions& options) {
    try {
        // 读取输入文件
        std::string sourceCode = readFile(options.inputFile);
        
        if (options.verbose) {
            std::cout << "读取文件: " << options.inputFile 
                      << " (" << sourceCode.size() << " 字节)" << std::endl;
        }
        
        // 创建编译上下文
        auto context = std::make_shared<Core::Context>();
        
        // 设置编译选项
        Core::CompileOptions compileOptions;
        compileOptions.debugMode = options.debugMode;
        compileOptions.generateSourceMap = options.generateSourceMap;
        compileOptions.minifyOutput = options.minifyOutput;
        compileOptions.enableOptimization = options.enableOptimization;
        compileOptions.outputPath = options.outputDir;
        compileOptions.modulePath = options.moduleDir;
        compileOptions.includePaths = options.includePaths;
        
        context->setOptions(compileOptions);
        context->setCurrentFile(options.inputFile);
        
        // 创建编译器调度器
        auto dispatcher = Compiler::CompilerFactory::createDispatcher();
        if (options.debugMode) {
            dispatcher->enableDebug(true);
        }
        
        // 开始编译
        auto startTime = std::chrono::high_resolution_clock::now();
        
        if (options.verbose) {
            std::cout << "开始编译..." << std::endl;
        }
        
        auto result = dispatcher->compileSource(sourceCode, options.inputFile, *context);
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime);
        
        // 检查编译结果
        if (!result || !result->success) {
            std::cerr << "编译失败!" << std::endl;
            
            // 显示错误信息
            const auto& errors = context->getErrors();
            for (const auto& error : errors) {
                std::cerr << "错误: " << error->file << ":" << error->line 
                          << ":" << error->column << " " << error->message << std::endl;
            }
            
            if (result) {
                for (const auto& error : result->errors) {
                    std::cerr << "编译器错误: " << error->message << std::endl;
                }
            }
            
            return 1;
        }
        
        // 生成输出文件名
        std::string outputFilename = generateOutputFilename(
            options.inputFile, options.outputFile, options.outputDir);
        
        // 写入输出文件
        writeFile(outputFilename, result->output);
        
        if (options.verbose) {
            std::cout << "编译成功!" << std::endl;
            std::cout << "输出文件: " << outputFilename << std::endl;
            std::cout << "编译时间: " << duration.count() << "ms" << std::endl;
            
            // 显示统计信息
            const auto& stats = dispatcher->getStats();
            std::cout << "统计信息:" << std::endl;
            std::cout << "  总片段数: " << stats.totalFragments << std::endl;
            std::cout << "  成功编译: " << stats.successfulCompiles << std::endl;
            std::cout << "  失败编译: " << stats.failedCompiles << std::endl;
            
            // 显示警告
            const auto& warnings = context->getWarnings();
            if (!warnings.empty()) {
                std::cout << "警告 (" << warnings.size() << "):" << std::endl;
                for (const auto& warning : warnings) {
                    std::cout << "  " << warning->file << ":" << warning->line 
                              << ":" << warning->column << " " << warning->message << std::endl;
                }
            }
        } else {
            std::cout << "编译完成: " << outputFilename << std::endl;
        }
        
        // 生成源码映射文件
        if (options.generateSourceMap && result->metadata.count("sourceMap")) {
            std::string sourceMapFile = outputFilename + ".map";
            writeFile(sourceMapFile, result->metadata.at("sourceMap"));
            
            if (options.verbose) {
                std::cout << "源码映射: " << sourceMapFile << std::endl;
            }
        }
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
}

int main(int argc, char* argv[]) {
    // 解析命令行参数
    CommandLineOptions options = parseCommandLine(argc, argv);
    
    // 显示帮助信息
    if (options.showHelp) {
        showHelp();
        return 0;
    }
    
    // 显示版本信息
    if (options.showVersion) {
        showVersion();
        return 0;
    }
    
    // 检查输入文件
    if (options.inputFile.empty()) {
        std::cerr << "错误: 未指定输入文件" << std::endl;
        std::cerr << "使用 -h 或 --help 查看帮助信息" << std::endl;
        return 1;
    }
    
    // 检查输入文件是否存在
    if (!std::filesystem::exists(options.inputFile)) {
        std::cerr << "错误: 输入文件不存在: " << options.inputFile << std::endl;
        return 1;
    }
    
    // 开始编译
    return compileFile(options);
}