#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <fstream>

// CHTL核心头文件
#include "Scanner/UnifiedScanner.h"
#include "Dispatcher/CompilerDispatcher.h"
#include "Merger/ResultMerger.h"
#include "Common/FragmentTypes.h"
#include "Common/CompilerInterface.h"

using namespace Chtl;
using namespace std;

/**
 * 打印程序信息
 */
void PrintBanner() {
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
void PrintUsage(const string& programName) {
    cout << "使用方法:" << endl;
    cout << "  " << programName << " <输入文件> [选项]" << endl;
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
    cout << "  " << programName << " input.chtl -o output.html -p" << endl;
    cout << "  " << programName << " input.chtl --debug --minify" << endl;
}

/**
 * 解析命令行参数
 */
struct CompileOptions {
    string InputFile;
    string OutputFile = "output.html";
    bool DebugMode = false;
    bool ParallelMode = false;
    bool Minify = false;
    size_t ThreadCount = 4;
    bool ShowHelp = false;
};

CompileOptions ParseArguments(int argc, char* argv[]) {
    CompileOptions options;
    
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            options.ShowHelp = true;
        } else if (arg == "-d" || arg == "--debug") {
            options.DebugMode = true;
        } else if (arg == "-p" || arg == "--parallel") {
            options.ParallelMode = true;
        } else if (arg == "-m" || arg == "--minify") {
            options.Minify = true;
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            options.OutputFile = argv[++i];
        } else if ((arg == "-t" || arg == "--threads") && i + 1 < argc) {
            options.ThreadCount = stoul(argv[++i]);
        } else if (options.InputFile.empty() && !arg.empty() && arg[0] != '-') {
            options.InputFile = arg;
        }
    }
    
    return options;
}

/**
 * 创建编译器配置
 */
CompilerConfig CreateCompilerConfig(const CompileOptions& options) {
    CompilerConfig config;
    config.DebugMode = options.DebugMode;
    config.Optimize = !options.DebugMode;
    config.MinifyCss = options.Minify;
    config.MinifyJs = options.Minify;
    return config;
}

/**
 * 创建合并器配置
 */
MergeConfig CreateMergeConfig(const CompileOptions& options) {
    MergeConfigBuilder builder;
    return builder
        .WithMinifyHtml(options.Minify)
        .WithPrettyPrint(!options.Minify)
        .WithMinifyCss(options.Minify)
        .WithMinifyJs(options.Minify)
        .WithIncludeGenerator(true)
        .Build();
}

/**
 * 主编译流程
 */
int Compile(const CompileOptions& options) {
    try {
        auto startTime = chrono::high_resolution_clock::now();
        
        cout << "开始编译CHTL文件: " << options.InputFile << endl;
        
        // 步骤1: 创建统一扫描器
        cout << "1. 初始化精准代码切割器..." << endl;
        auto scanner = ScannerFactory::CreateScanner();
        if (options.DebugMode) {
            scanner->SetDebugMode(true);
        }
        
        // 步骤2: 扫描源代码，切割成片段
        cout << "2. 扫描源代码并切割成片段..." << endl;
        auto fragments = scanner->ScanFile(options.InputFile);
        if (!fragments || fragments->IsEmpty()) {
            cerr << "错误: 无法读取或解析输入文件" << endl;
            return 1;
        }
        
        cout << "   发现 " << fragments->GetFragmentCount() << " 个代码片段" << endl;
        cout << "   - CHTL片段: " << fragments->GetChtlFragments().size() << " 个" << endl;
        cout << "   - CHTL JS片段: " << fragments->GetChtlJsFragments().size() << " 个" << endl;
        cout << "   - CSS片段: " << fragments->GetCssFragments().size() << " 个" << endl;
        cout << "   - JavaScript片段: " << fragments->GetJsFragments().size() << " 个" << endl;
        
        // 步骤3: 创建编译器调度器
        cout << "3. 初始化编译器调度器..." << endl;
        auto dispatcher = options.ParallelMode ? 
            DispatcherFactory::CreateParallelDispatcher(options.ThreadCount) :
            DispatcherFactory::CreateDispatcher();
        
        // 配置调度器
        auto compilerConfig = CreateCompilerConfig(options);
        dispatcher->SetCompilerConfig(compilerConfig);
        
        // 配置结果合并器
        auto mergeConfig = CreateMergeConfig(options);
        auto merger = MergerFactory::CreateHtmlMerger(mergeConfig);
        dispatcher->SetResultMerger(std::move(merger));
        
        // 步骤4: 调度编译
        cout << "4. 调度编译各类型片段..." << endl;
        if (options.ParallelMode) {
            cout << "   使用并行编译模式 (线程数: " << options.ThreadCount << ")" << endl;
        }
        
        auto compileResult = dispatcher->Dispatch(*fragments);
        if (!compileResult || !compileResult->IsSuccess()) {
            cerr << "编译失败!" << endl;
            if (compileResult && compileResult->HasErrors()) {
                for (const auto& error : compileResult->GetErrors()) {
                    cerr << "错误: " << error << endl;
                }
            }
            return 1;
        }
        
        // 步骤5: 输出结果
        cout << "5. 生成最终HTML文件..." << endl;
        
        // 写入文件
        ofstream outputStream(options.OutputFile);
        if (!outputStream.is_open()) {
            cerr << "错误: 无法创建输出文件 " << options.OutputFile << endl;
            return 1;
        }
        
        outputStream << compileResult->GetContent();
        outputStream.close();
        
        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
        
        cout << "编译完成!" << endl;
        cout << "输出文件: " << options.OutputFile << endl;
        cout << "编译时间: " << duration.count() << " 毫秒" << endl;
        
        // 显示统计信息
        if (options.DebugMode) {
            cout << endl << "=== 编译统计信息 ===" << endl;
            cout << "总编译次数: " << dispatcher->GetTotalCompilations() << endl;
            cout << "成功编译: " << dispatcher->GetSuccessfulCompilations() << endl;
            cout << "失败编译: " << dispatcher->GetFailedCompilations() << endl;
            cout << "平均编译时间: " << dispatcher->GetAverageCompileTime() << " 毫秒" << endl;
            
            if (compileResult->HasWarnings()) {
                cout << endl << "=== 警告信息 ===" << endl;
                for (const auto& warning : compileResult->GetWarnings()) {
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
    PrintBanner();
    
    // 解析命令行参数
    auto options = ParseArguments(argc, argv);
    
    // 显示帮助信息
    if (options.ShowHelp || argc == 1) {
        PrintUsage(argv[0]);
        return 0;
    }
    
    // 检查输入文件
    if (options.InputFile.empty()) {
        cerr << "错误: 未指定输入文件" << endl;
        PrintUsage(argv[0]);
        return 1;
    }
    
    // 开始编译
    return Compile(options);
}