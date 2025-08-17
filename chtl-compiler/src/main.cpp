#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "scanner/unified_scanner.h"
#include "dispatcher/compiler_dispatcher.h"
#include "merger/result_merger.h"
#include "utils/common.h"

using namespace chtl;

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
}

// 打印使用说明
void printUsage(const char* program) {
    std::cout << "Usage: " << program << " <input.chtl> [output.html]\n";
    std::cout << "       " << program << " <input.chtl> -o <output.html>\n";
    std::cout << "  Compiles a CHTL file to HTML\n";
    std::cout << "  If output file is not specified, uses input filename with .html extension\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string input_file = argv[1];
    std::string output_file;
    
    // 检查是否有-o选项
    if (argc >= 4 && std::string(argv[2]) == "-o") {
        output_file = argv[3];
    } else if (argc >= 3 && std::string(argv[2]) != "-o") {
        output_file = argv[2];
    } else {
        // 生成输出文件名
        size_t dot_pos = input_file.find_last_of('.');
        if (dot_pos != std::string::npos) {
            output_file = input_file.substr(0, dot_pos) + ".html";
        } else {
            output_file = input_file + ".html";
        }
    }
    
    try {
        // 读取输入文件
        std::cout << "Reading " << input_file << "...\n";
        std::string input_content = readFile(input_file);
        
        // 临时方案：直接将整个文件作为一个CHTL片段
        std::vector<CodeFragment> fragments;
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL;
        fragment.content = input_content;
        fragment.location.file_path = input_file;
        fragment.location.start_line = 1;
        fragment.location.end_line = std::count(input_content.begin(), input_content.end(), '\n') + 1;
        fragments.push_back(fragment);
        
        /*
        // 创建扫描器
        // 创建扫描器
        ScannerConfig scan_config;
        scan_config.enable_intelligent_slicing = true;
        scan_config.initial_slice_size = 4096; // 增加初始切片大小
        CHTLUnifiedScanner scanner(scan_config);
        
        // 扫描代码片段
        std::cout << "Scanning code fragments...\n";
        auto fragments = scanner.scan(input_content);
        */
        
        if (fragments.empty()) {
            std::cerr << "No code fragments found!\n";
            return 1;
        }
        
        std::cout << "Found " << fragments.size() << " code fragments\n";
        
        // 调试：打印片段信息
        for (size_t i = 0; i < fragments.size(); ++i) {
            const char* type_name = "UNKNOWN";
            switch (fragments[i].type) {
                case CodeFragmentType::CHTL: type_name = "CHTL"; break;
                case CodeFragmentType::CHTL_JS: type_name = "CHTL_JS"; break;
                case CodeFragmentType::CSS: type_name = "CSS"; break;
                case CodeFragmentType::JAVASCRIPT: type_name = "JAVASCRIPT"; break;
                case CodeFragmentType::HTML_RAW: type_name = "HTML_RAW"; break;
            }
            
            std::cout << "Fragment " << i << ": type=" << type_name
                     << ", lines " << fragments[i].location.start_line 
                     << "-" << fragments[i].location.end_line 
                     << ", content length=" << fragments[i].content.length() << "\n";
            
            // 打印片段内容的前50个字符
            if (fragments[i].content.length() > 0) {
                std::cout << "  Content: " 
                         << fragments[i].content.substr(0, std::min(size_t(50), fragments[i].content.length())) 
                         << (fragments[i].content.length() > 50 ? "..." : "") << "\n";
            }
        }
        
        // 创建编译器调度器
        DispatcherConfig dispatcher_config;
        dispatcher_config.enable_parallel_compilation = true;
        CompilerDispatcher dispatcher(dispatcher_config);
        
        // 编译选项
        CompilerOptions options;
        options.debug_mode = false;
        options.minimize_output = false;
        
        // 调度编译
        std::cout << "Compiling fragments...\n";
        auto results = dispatcher.dispatch(fragments, options);
        
        // 检查编译错误
        bool has_errors = false;
        for (size_t i = 0; i < results.size(); ++i) {
            if (!results[i].success) {
                has_errors = true;
                std::cerr << "Compilation errors in fragment " << i << ":\n";
                for (const auto& error : results[i].errors) {
                    std::cerr << "  " << error << "\n";
                }
            }
        }
        
        if (has_errors) {
            return 1;
        }
        
        // 使用结果合并器
        ResultMerger merger;
        MergeOptions merge_options;
        merge_options.prettify = true;
        merge_options.inline_styles = true;
        merge_options.inline_scripts = true;
        
        std::string final_output = merger.merge(fragments, results, merge_options);
        
        // 写入输出文件
        std::cout << "Writing " << output_file << "...\n";
        writeFile(output_file, final_output);
        
        std::cout << "Compilation successful!\n";
        
        // 打印统计信息
        auto stats = dispatcher.getStatistics();
        std::cout << "\nStatistics:\n";
        std::cout << "  Total compilations: " << stats.total_compilations << "\n";
        std::cout << "  Cache hits: " << stats.cache_hits << "\n";
        std::cout << "  Cache misses: " << stats.cache_misses << "\n";
        std::cout << "  Average compilation time: " << stats.average_compilation_time_ms << " ms\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}