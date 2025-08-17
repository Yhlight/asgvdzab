#include <iostream>
#include <string>
#include <memory>
#include <chrono>

// CHTL核心头文件
#include "scanner/unified_scanner.h"
#include "dispatcher/compiler_dispatcher.h"
#include "merger/result_merger.h"
#include "common/fragment_types.h"
#include "common/compiler_interface.h"

using namespace chtl;
using namespace std;

/**
 * 打印程序信息
 */
void printBanner() {
    cout << "========================================" << endl;
    cout << "    CHTL编译器 v1.0.0" << endl;
    cout << "    基于C++17的超文本语言编译器" << endl;
    cout << "    Copyright (c) 2024 CHTL Team" << endl;
    cout << "    开源协议: MIT License" << endl;
    cout << "========================================" << endl;
    cout << endl;
}

/**
 * 打印使用说明
 */
void printUsage(const string& program_name) {
    cout << "使用方法:" << endl;
    cout << "  " << program_name << " <输入文件> [选项]" << endl;
    cout << endl;
    cout << "选项:" << endl;
    cout << "  -o, --output <文件>     指定输出文件 (默认: output.html)" << endl;
    cout << "  -d, --debug             启用调试模式" << endl;
    cout << "  -p, --parallel          启用并行编译" << endl;
    cout << "  -m, --minify            压缩输出" << endl;
    cout << "  -t, --threads <数量>    指定线程数 (默认: 4)" << endl;
    cout << "  -h, --help              显示此帮助信息" << endl;
    cout << endl;
    cout << "示例:" << endl;
    cout << "  " << program_name << " input.chtl -o output.html -p" << endl;
    cout << "  " << program_name << " input.chtl --debug --minify" << endl;
}

/**
 * 解析命令行参数
 */
struct CompileOptions {
    string input_file;
    string output_file = "output.html";
    bool debug_mode = false;
    bool parallel_mode = false;
    bool minify = false;
    size_t thread_count = 4;
    bool show_help = false;
};

CompileOptions parseArguments(int argc, char* argv[]) {
    CompileOptions options;
    
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            options.show_help = true;
        } else if (arg == "-d" || arg == "--debug") {
            options.debug_mode = true;
        } else if (arg == "-p" || arg == "--parallel") {
            options.parallel_mode = true;
        } else if (arg == "-m" || arg == "--minify") {
            options.minify = true;
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            options.output_file = argv[++i];
        } else if ((arg == "-t" || arg == "--threads") && i + 1 < argc) {
            options.thread_count = stoul(argv[++i]);
        } else if (options.input_file.empty() && !arg.empty() && arg[0] != '-') {
            options.input_file = arg;
        }
    }
    
    return options;
}

/**
 * 创建编译器配置
 */
CompilerConfig createCompilerConfig(const CompileOptions& options) {
    CompilerConfig config;
    config.debug_mode = options.debug_mode;
    config.optimize = !options.debug_mode;
    config.minify_css = options.minify;
    config.minify_js = options.minify;
    return config;
}

/**
 * 创建合并器配置
 */
MergeConfig createMergeConfig(const CompileOptions& options) {
    MergeConfigBuilder builder;
    return builder
        .withMinifyHTML(options.minify)
        .withPrettyPrint(!options.minify)
        .withMinifyCSS(options.minify)
        .withMinifyJS(options.minify)
        .withIncludeGenerator(true)
        .build();
}

/**
 * 主编译流程
 */
int compile(const CompileOptions& options) {
    try {
        auto start_time = chrono::high_resolution_clock::now();
        
        cout << "开始编译CHTL文件: " << options.input_file << endl;
        
        // 步骤1: 创建统一扫描器
        cout << "1. 初始化精准代码切割器..." << endl;
        auto scanner = ScannerFactory::createScanner();
        if (options.debug_mode) {
            scanner->setDebugMode(true);
        }
        
        // 步骤2: 扫描源代码，切割成片段
        cout << "2. 扫描源代码并切割成片段..." << endl;
        auto fragments = scanner->scanFile(options.input_file);
        if (!fragments || fragments->isEmpty()) {
            cerr << "错误: 无法读取或解析输入文件" << endl;
            return 1;
        }
        
        cout << "   发现 " << fragments->getFragmentCount() << " 个代码片段" << endl;
        cout << "   - CHTL片段: " << fragments->getCHTLFragments().size() << " 个" << endl;
        cout << "   - CHTL JS片段: " << fragments->getCHTLJSFragments().size() << " 个" << endl;
        cout << "   - CSS片段: " << fragments->getCSSFragments().size() << " 个" << endl;
        cout << "   - JavaScript片段: " << fragments->getJSFragments().size() << " 个" << endl;
        
        // 步骤3: 创建编译器调度器
        cout << "3. 初始化编译器调度器..." << endl;
        auto dispatcher = options.parallel_mode ? 
            DispatcherFactory::createParallelDispatcher(options.thread_count) :
            DispatcherFactory::createDispatcher();
        
        // 配置调度器
        auto compiler_config = createCompilerConfig(options);
        dispatcher->setCompilerConfig(compiler_config);
        
        // 配置结果合并器
        auto merge_config = createMergeConfig(options);
        auto merger = MergerFactory::createHTMLMerger(merge_config);
        dispatcher->setResultMerger(std::move(merger));
        
        // 步骤4: 调度编译
        cout << "4. 调度编译各类型片段..." << endl;
        if (options.parallel_mode) {
            cout << "   使用并行编译模式 (线程数: " << options.thread_count << ")" << endl;
        }
        
        auto compile_result = dispatcher->dispatch(*fragments);
        if (!compile_result || !compile_result->isSuccess()) {
            cerr << "编译失败!" << endl;
            if (compile_result && compile_result->hasErrors()) {
                for (const auto& error : compile_result->getErrors()) {
                    cerr << "错误: " << error << endl;
                }
            }
            return 1;
        }
        
        // 步骤5: 输出结果
        cout << "5. 生成最终HTML文件..." << endl;
        
        // 写入文件
        ofstream output_stream(options.output_file);
        if (!output_stream.is_open()) {
            cerr << "错误: 无法创建输出文件 " << options.output_file << endl;
            return 1;
        }
        
        output_stream << compile_result->getContent();
        output_stream.close();
        
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        
        cout << "编译完成!" << endl;
        cout << "输出文件: " << options.output_file << endl;
        cout << "编译时间: " << duration.count() << " 毫秒" << endl;
        
        // 显示统计信息
        if (options.debug_mode) {
            cout << endl << "=== 编译统计信息 ===" << endl;
            cout << "总编译次数: " << dispatcher->getTotalCompilations() << endl;
            cout << "成功编译: " << dispatcher->getSuccessfulCompilations() << endl;
            cout << "失败编译: " << dispatcher->getFailedCompilations() << endl;
            cout << "平均编译时间: " << dispatcher->getAverageCompileTime() << " 毫秒" << endl;
            
            if (compile_result->hasWarnings()) {
                cout << endl << "=== 警告信息 ===" << endl;
                for (const auto& warning : compile_result->getWarnings()) {
                    cout << "警告: " << warning << endl;
                }
            }
        }
        
        return 0;
        
    } catch (const exception& e) {
        cerr << "编译过程中发生异常: " << e.what() << endl;
        return 1;
    }
}

/**
 * 主函数
 */
int main(int argc, char* argv[]) {
    printBanner();
    
    // 解析命令行参数
    auto options = parseArguments(argc, argv);
    
    // 显示帮助信息
    if (options.show_help || argc == 1) {
        printUsage(argv[0]);
        return 0;
    }
    
    // 检查输入文件
    if (options.input_file.empty()) {
        cerr << "错误: 未指定输入文件" << endl;
        printUsage(argv[0]);
        return 1;
    }
    
    // 开始编译
    return compile(options);
}