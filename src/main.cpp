#include "../include/scanner/CHTLUnifiedScanner.h"
#include "../include/dispatcher/CompilerDispatcher.h"
#include "../include/merger/ResultMerger.h"
#include "../include/common/Utils.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace CHTL;

void printUsage() {
    std::cout << "CHTL编译器 v1.0.0" << std::endl;
    std::cout << "用法: chtl_compiler [选项] <输入文件>" << std::endl;
    std::cout << std::endl;
    std::cout << "选项:" << std::endl;
    std::cout << "  -o, --output <文件>    指定输出文件 (默认: output.html)" << std::endl;
    std::cout << "  -d, --debug           启用调试模式" << std::endl;
    std::cout << "  -m, --minify          压缩输出" << std::endl;
    std::cout << "  -f, --format          格式化输出" << std::endl;
    std::cout << "  -h, --help            显示此帮助信息" << std::endl;
    std::cout << "  --dump-tokens         输出Token信息" << std::endl;
    std::cout << "  --dump-fragments      输出代码片段信息" << std::endl;
    std::cout << "  --dump-ast            输出AST信息" << std::endl;
    std::cout << std::endl;
    std::cout << "示例:" << std::endl;
    std::cout << "  chtl_compiler input.chtl" << std::endl;
    std::cout << "  chtl_compiler -o output.html -d input.chtl" << std::endl;
}

void runTest() {
    std::cout << "=== CHTL编译器测试 ===" << std::endl;
    
    // 创建测试CHTL代码
    std::string test_chtl = R"(
// CHTL测试代码
html
{
    head
    {
        text
        {
            "CHTL测试页面"
        }
    }
    
    body
    {
        div
        {
            id: container;
            class: main-content;
            
            style
            {
                .box
                {
                    width: 300px;
                    height: 200px;
                    background-color: #f0f0f0;
                    border: 1px solid #ccc;
                    padding: 20px;
                }
                
                &:hover
                {
                    background-color: #e0e0e0;
                }
            }
            
            script
            {
                {{.box}}->addEventListener('click', () => {
                    console.log('Box clicked!');
                });
                
                vir testObj = listen({
                    click: () => {
                        alert('Listen clicked!');
                    },
                    mouseenter: function() {
                        console.log('Mouse entered');
                    }
                });
            }
            
            h1
            {
                text
                {
                    "欢迎使用CHTL"
                }
            }
            
            p
            {
                text
                {
                    "这是一个CHTL编译器的测试页面。"
                }
            }
        }
    }
}
)";
    
    try {
        // 创建配置
        Configuration config;
        config.debug_mode = true;
        
        // 创建扫描器
        CHTLUnifiedScanner scanner(test_chtl);
        scanner.setConfiguration(config);
        
        std::cout << "1. 开始扫描和切割代码片段..." << std::endl;
        auto fragments = scanner.scanAndSplit();
        std::cout << "   切割完成，共 " << fragments.size() << " 个代码片段" << std::endl;
        
        // 输出片段信息
        scanner.dumpFragments(fragments);
        
        // 创建编译器调度器
        std::cout << "\n2. 初始化编译器调度器..." << std::endl;
        CompilerDispatcher dispatcher(config);
        
        std::cout << "3. 开始编译代码片段..." << std::endl;
        auto compile_results = dispatcher.compileFragments(fragments);
        std::cout << "   编译完成，成功: " << dispatcher.getSuccessfulCompileCount() 
                  << ", 失败: " << dispatcher.getFailedCompileCount() << std::endl;
        
        // 输出编译结果统计
        dispatcher.dumpCompileResults();
        
        // 创建结果合并器
        std::cout << "\n4. 开始合并编译结果..." << std::endl;
        ResultMerger merger(config);
        auto final_result = merger.mergeResults(compile_results);
        
        if (final_result->success) {
            std::cout << "   合并成功！" << std::endl;
            
            // 输出最终HTML到文件
            std::string output_file = "test_output.html";
            if (Utils::writeFile(output_file, final_result->output)) {
                std::cout << "   输出文件已保存: " << output_file << std::endl;
            } else {
                std::cout << "   保存输出文件失败！" << std::endl;
            }
            
            // 输出合并结果统计
            merger.dumpMergeResults();
            
        } else {
            std::cout << "   合并失败！" << std::endl;
            for (const auto& error : final_result->errors) {
                std::cout << "   错误: " << error << std::endl;
            }
        }
        
    } catch (const std::exception& e) {
        std::cout << "测试异常: " << e.what() << std::endl;
    }
    
    std::cout << "=== 测试完成 ===" << std::endl;
}

int main(int argc, char* argv[]) {
    // 解析命令行参数
    std::string input_file;
    std::string output_file = "output.html";
    bool debug_mode = false;
    bool minify_output = false;
    bool format_output = false;
    bool dump_tokens = false;
    bool dump_fragments = false;
    bool dump_ast = false;
    bool run_test_mode = false;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage();
            return 0;
        } else if (arg == "-d" || arg == "--debug") {
            debug_mode = true;
        } else if (arg == "-m" || arg == "--minify") {
            minify_output = true;
        } else if (arg == "-f" || arg == "--format") {
            format_output = true;
        } else if (arg == "--dump-tokens") {
            dump_tokens = true;
        } else if (arg == "--dump-fragments") {
            dump_fragments = true;
        } else if (arg == "--dump-ast") {
            dump_ast = true;
        } else if (arg == "--test") {
            run_test_mode = true;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                std::cerr << "错误: -o/--output 需要指定文件名" << std::endl;
                return 1;
            }
        } else if (arg[0] != '-') {
            input_file = arg;
        } else {
            std::cerr << "错误: 未知选项 " << arg << std::endl;
            printUsage();
            return 1;
        }
    }
    
    // 如果是测试模式
    if (run_test_mode) {
        runTest();
        return 0;
    }
    
    // 检查输入文件
    if (input_file.empty()) {
        std::cerr << "错误: 请指定输入文件" << std::endl;
        printUsage();
        return 1;
    }
    
    if (!Utils::fileExists(input_file)) {
        std::cerr << "错误: 输入文件不存在: " << input_file << std::endl;
        return 1;
    }
    
    try {
        // 读取输入文件
        std::cout << "读取输入文件: " << input_file << std::endl;
        std::string source_code = Utils::readFile(input_file);
        
        if (source_code.empty()) {
            std::cerr << "错误: 无法读取输入文件或文件为空" << std::endl;
            return 1;
        }
        
        // 创建配置
        Configuration config;
        config.debug_mode = debug_mode;
        
        // 创建扫描器
        CHTLUnifiedScanner scanner(source_code);
        scanner.setConfiguration(config);
        
        std::cout << "开始扫描和切割代码片段..." << std::endl;
        auto fragments = scanner.scanAndSplit();
        std::cout << "切割完成，共 " << fragments.size() << " 个代码片段" << std::endl;
        
        if (dump_fragments) {
            scanner.dumpFragments(fragments);
        }
        
        if (dump_tokens) {
            auto tokens = scanner.tokenize();
            scanner.dumpTokens(tokens);
        }
        
        // 创建编译器调度器
        std::cout << "初始化编译器调度器..." << std::endl;
        CompilerDispatcher dispatcher(config);
        
        std::cout << "开始编译代码片段..." << std::endl;
        auto compile_results = dispatcher.compileFragments(fragments);
        std::cout << "编译完成，成功: " << dispatcher.getSuccessfulCompileCount() 
                  << ", 失败: " << dispatcher.getFailedCompileCount() << std::endl;
        
        if (debug_mode) {
            dispatcher.dumpCompileResults();
        }
        
        if (dump_ast) {
            // 输出各编译器的AST信息
            if (dispatcher.getCHTLCompiler()) {
                dispatcher.getCHTLCompiler()->dumpAST();
            }
            if (dispatcher.getCHTLJSCompiler()) {
                dispatcher.getCHTLJSCompiler()->dumpAST();
            }
        }
        
        // 创建结果合并器
        std::cout << "开始合并编译结果..." << std::endl;
        ResultMerger merger(config);
        
        if (minify_output) {
            merger.setMinifyOutput(true);
        }
        if (format_output) {
            merger.setFormatOutput(true);
        }
        
        auto final_result = merger.mergeResults(compile_results);
        
        if (final_result->success) {
            std::cout << "合并成功！" << std::endl;
            
            // 输出最终HTML到文件
            if (Utils::writeFile(output_file, final_result->output)) {
                std::cout << "输出文件已保存: " << output_file << std::endl;
            } else {
                std::cerr << "错误: 保存输出文件失败！" << std::endl;
                return 1;
            }
            
            if (debug_mode) {
                merger.dumpMergeResults();
            }
            
        } else {
            std::cerr << "合并失败！" << std::endl;
            for (const auto& error : final_result->errors) {
                std::cerr << "错误: " << error << std::endl;
            }
            return 1;
        }
        
        // 输出警告信息
        if (!final_result->warnings.empty()) {
            std::cout << "\n警告信息:" << std::endl;
            for (const auto& warning : final_result->warnings) {
                std::cout << "警告: " << warning << std::endl;
            }
        }
        
        std::cout << "编译完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "编译异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}