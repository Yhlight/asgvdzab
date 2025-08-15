#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "scanner/unified_scanner.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include "common/types.hpp"

using namespace chtl;

/**
 * 显示帮助信息
 */
void showHelp(const std::string& programName) {
    std::cout << "CHTL编译器 v1.0.0\n";
    std::cout << "用法: " << programName << " [选项] <输入文件>\n\n";
    std::cout << "选项:\n";
    std::cout << "  -h, --help           显示此帮助信息\n";
    std::cout << "  -o, --output <文件>  指定输出文件（默认: 输入文件名.html）\n";
    std::cout << "  -d, --debug          启用调试模式\n";
    std::cout << "  -v, --verbose        详细输出\n";
    std::cout << "  --stats              显示编译统计信息\n\n";
    std::cout << "示例:\n";
    std::cout << "  " << programName << " example.chtl\n";
    std::cout << "  " << programName << " -o output.html -d example.chtl\n";
}

/**
 * 命令行参数结构
 */
struct CommandLineArgs {
    std::string inputFile;
    std::string outputFile;
    bool debugMode = false;
    bool verbose = false;
    bool showStats = false;
    bool showHelp = false;
};

/**
 * 解析命令行参数
 */
CommandLineArgs parseCommandLine(int argc, char* argv[]) {
    CommandLineArgs args;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            args.showHelp = true;
        } else if (arg == "-d" || arg == "--debug") {
            args.debugMode = true;
        } else if (arg == "-v" || arg == "--verbose") {
            args.verbose = true;
        } else if (arg == "--stats") {
            args.showStats = true;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                args.outputFile = argv[++i];
            } else {
                std::cerr << "错误: " << arg << " 需要一个参数\n";
                args.showHelp = true;
            }
        } else if (arg.length() > 0 && arg[0] == '-') {
            std::cerr << "错误: 未知选项 " << arg << "\n";
            args.showHelp = true;
        } else {
            if (args.inputFile.empty()) {
                args.inputFile = arg;
            } else {
                std::cerr << "错误: 只能指定一个输入文件\n";
                args.showHelp = true;
            }
        }
    }
    
    return args;
}

/**
 * 生成默认输出文件名
 */
std::string generateOutputFileName(const std::string& inputFile) {
    size_t lastDot = inputFile.find_last_of('.');
    if (lastDot != std::string::npos) {
        return inputFile.substr(0, lastDot) + ".html";
    }
    return inputFile + ".html";
}

/**
 * 输出编译统计信息
 */
void showCompileStatistics(const CompilerDispatcher& dispatcher) {
    auto stats = dispatcher.getCompileStatistics();
    
    std::cout << "\n=== 编译统计信息 ===\n";
    for (const auto& [key, value] : stats) {
        std::cout << key << ": " << value << "\n";
    }
}

/**
 * 输出扫描结果详情
 */
void showScanDetails(const ScanResult& scanResult, bool verbose) {
    if (verbose) {
        std::cout << "=== 扫描结果 ===\n";
        std::cout << "成功: " << (scanResult.success ? "是" : "否") << "\n";
        std::cout << "代码片段数量: " << scanResult.segments.size() << "\n";
        
        for (size_t i = 0; i < scanResult.segments.size(); ++i) {
            const auto& segment = scanResult.segments[i];
            std::cout << "\n片段 " << (i + 1) << ":\n";
            std::cout << "  类型: " << toString(segment.type) << "\n";
            std::cout << "  位置: " << segment.range.start.line << ":" << segment.range.start.column 
                      << " - " << segment.range.end.line << ":" << segment.range.end.column << "\n";
            std::cout << "  内容长度: " << segment.content.length() << " 字符\n";
            if (!segment.context.empty()) {
                std::cout << "  上下文: " << segment.context << "\n";
            }
        }
        
        if (!scanResult.warnings.empty()) {
            std::cout << "\n警告:\n";
            for (const auto& warning : scanResult.warnings) {
                std::cout << "  " << warning << "\n";
            }
        }
        
        if (!scanResult.errors.empty()) {
            std::cout << "\n错误:\n";
            for (const auto& error : scanResult.errors) {
                std::cout << "  " << error << "\n";
            }
        }
    }
}

/**
 * 输出编译结果详情
 */
void showCompileDetails(const CompileResult& compileResult, bool verbose) {
    if (verbose) {
        std::cout << "\n=== 编译结果 ===\n";
        std::cout << "成功: " << (compileResult.success ? "是" : "否") << "\n";
        std::cout << "输出长度: " << compileResult.output.length() << " 字符\n";
        
        if (!compileResult.metadata.empty()) {
            std::cout << "\n元数据:\n";
            for (const auto& [key, value] : compileResult.metadata) {
                std::cout << "  " << key << ": " << value << "\n";
            }
        }
        
        if (!compileResult.warnings.empty()) {
            std::cout << "\n警告:\n";
            for (const auto& warning : compileResult.warnings) {
                std::cout << "  " << warning << "\n";
            }
        }
        
        if (!compileResult.errors.empty()) {
            std::cout << "\n错误:\n";
            for (const auto& error : compileResult.errors) {
                std::cout << "  " << error << "\n";
            }
        }
    }
}

/**
 * 主函数
 */
int main(int argc, char* argv[]) {
    auto args = parseCommandLine(argc, argv);
    
    if (args.showHelp || args.inputFile.empty()) {
        showHelp(argv[0]);
        return args.showHelp ? 0 : 1;
    }
    
    // 生成输出文件名
    if (args.outputFile.empty()) {
        args.outputFile = generateOutputFileName(args.inputFile);
    }
    
    if (args.verbose) {
        std::cout << "输入文件: " << args.inputFile << "\n";
        std::cout << "输出文件: " << args.outputFile << "\n";
        std::cout << "调试模式: " << (args.debugMode ? "开启" : "关闭") << "\n";
    }
    
    try {
        // 创建编译配置
        CompileConfig config;
        config.debugMode = args.debugMode;
        
        // 创建扫描器
        CHTLUnifiedScanner scanner(config);
        
        // 扫描输入文件
        auto scanResult = scanner.scanFileWithSliceEngine(args.inputFile);
        
        showScanDetails(scanResult, args.verbose);
        
        if (!scanResult.success) {
            std::cerr << "扫描失败!\n";
            for (const auto& error : scanResult.errors) {
                std::cerr << "错误: " << error << "\n";
            }
            return 1;
        }
        
        // 创建编译器调度器
        CompilerDispatcher dispatcher;
        dispatcher.setGlobalConfig(config);
        dispatcher.setDebugMode(args.debugMode);
        
        // 编译代码片段
        auto compileResult = dispatcher.compile(scanResult.segments, config);
        
        showCompileDetails(compileResult, args.verbose);
        
        if (!compileResult.success) {
            std::cerr << "编译失败!\n";
            for (const auto& error : compileResult.errors) {
                std::cerr << "错误: " << error << "\n";
            }
            return 1;
        }
        
        // 写入输出文件
        std::ofstream outFile(args.outputFile);
        if (!outFile.is_open()) {
            std::cerr << "无法创建输出文件: " << args.outputFile << "\n";
            return 1;
        }
        
        outFile << compileResult.output;
        outFile.close();
        
        std::cout << "编译成功! 输出文件: " << args.outputFile << "\n";
        
        if (args.showStats) {
            showCompileStatistics(dispatcher);
        }
        
        // 显示警告（如果有）
        if (!compileResult.warnings.empty()) {
            std::cout << "\n警告:\n";
            for (const auto& warning : compileResult.warnings) {
                std::cout << "  " << warning << "\n";
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "异常: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}