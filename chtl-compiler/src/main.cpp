#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

#include "scanner/scanner.h"
#include "dispatcher/dispatcher.h"
#include "merger/merger.h"

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options] input_file.chtl\n";
    std::cout << "Options:\n";
    std::cout << "  -o, --output <file>     Output file (default: output.html)\n";
    std::cout << "  -d, --debug             Enable debug mode\n";
    std::cout << "  -m, --minify            Minify output\n";
    std::cout << "  -p, --parallel          Enable parallel compilation\n";
    std::cout << "  -t, --threads <n>       Set max threads for parallel compilation\n";
    std::cout << "  -h, --help              Show this help message\n";
    std::cout << "  -v, --version           Show version information\n";
}

void printVersion() {
    std::cout << "CHTL Compiler v1.0.0\n";
    std::cout << "Based on C++17\n";
}

int main(int argc, char* argv[]) {
    // 解析命令行参数
    std::string input_file;
    std::string output_file = "output.html";
    bool debug_mode = false;
    bool minify = false;
    bool parallel = true;
    unsigned int max_threads = 0;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            printVersion();
            return 0;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                std::cerr << "Error: Missing output file argument\n";
                return 1;
            }
        } else if (arg == "-d" || arg == "--debug") {
            debug_mode = true;
        } else if (arg == "-m" || arg == "--minify") {
            minify = true;
        } else if (arg == "-p" || arg == "--parallel") {
            parallel = true;
        } else if (arg == "-t" || arg == "--threads") {
            if (i + 1 < argc) {
                max_threads = std::stoul(argv[++i]);
            } else {
                std::cerr << "Error: Missing threads argument\n";
                return 1;
            }
        } else if (arg[0] != '-') {
            input_file = arg;
        } else {
            std::cerr << "Error: Unknown option " << arg << "\n";
            return 1;
        }
    }
    
    // 检查输入文件
    if (input_file.empty()) {
        std::cerr << "Error: No input file specified\n";
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        // 创建编译器组件
        chtl::CHTLUnifiedScanner scanner;
        chtl::CompilerDispatcher dispatcher;
        chtl::ResultMerger merger;
        
        // 设置调试模式
        if (debug_mode) {
            scanner.setDebugMode(true);
            dispatcher.setDebugMode(true);
            merger.setDebugMode(true);
            std::cout << "Debug mode enabled\n";
        }
        
        // 设置并行编译选项
        dispatcher.setParallelCompilation(parallel);
        if (max_threads > 0) {
            dispatcher.setMaxThreads(max_threads);
        }
        
        std::cout << "Compiling " << input_file << "...\n";
        
        // 步骤1: 扫描源文件
        auto fragments = scanner.scanFile(input_file);
        
        if (scanner.getErrors().size() > 0) {
            std::cerr << "Scanner errors:\n";
            for (const auto& error : scanner.getErrors()) {
                std::cerr << "  " << error << "\n";
            }
            return 1;
        }
        
        std::cout << "Scanned " << fragments.size() << " code fragments\n";
        
        // 步骤2: 调度编译
        chtl::CompileOptions options;
        options.debug_mode = debug_mode;
        options.minify = minify;
        options.output_dir = ".";
        
        auto compile_results = dispatcher.dispatch(fragments, options);
        
        // 检查编译错误
        int failed_count = 0;
        for (const auto& result : compile_results) {
            if (!result.result.success) {
                std::cerr << "Compile error in fragment " << result.fragment_index 
                         << ": " << result.result.error_msg << "\n";
                failed_count++;
            }
        }
        
        if (failed_count > 0) {
            std::cerr << "Compilation failed with " << failed_count << " errors\n";
            return 1;
        }
        
        auto stats = dispatcher.getLastStats();
        std::cout << "Compilation complete in " << stats.total_time_ms << " ms\n";
        
        // 步骤3: 合并结果
        std::string final_output = merger.merge(compile_results, options);
        
        // 写入输出文件
        std::ofstream out_file(output_file);
        if (!out_file.is_open()) {
            std::cerr << "Error: Cannot create output file " << output_file << "\n";
            return 1;
        }
        
        out_file << final_output;
        out_file.close();
        
        auto merge_stats = merger.getLastStats();
        std::cout << "Output written to " << output_file 
                  << " (" << merge_stats.total_size << " bytes)\n";
        
        if (debug_mode) {
            std::cout << "\nDetailed statistics:\n";
            std::cout << "  HTML size: " << merge_stats.html_size << " bytes\n";
            std::cout << "  CSS size: " << merge_stats.css_size << " bytes\n";
            std::cout << "  JS size: " << merge_stats.js_size << " bytes\n";
        }
        
        std::cout << "Success!\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}