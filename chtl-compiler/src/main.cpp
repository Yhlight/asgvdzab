#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "scanner/unified_scanner.h"
#include "dispatcher/compiler_dispatcher.h"
// #include "merger/result_merger.h" // TODO: 实现后取消注释
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
    
    if (argc >= 3) {
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
        
        // 创建扫描器
        ScannerConfig scanner_config;
        scanner_config.preserve_comments = false;
        CHTLUnifiedScanner scanner(scanner_config);
        
        // 扫描代码片段
        std::cout << "Scanning code fragments...\n";
        auto fragments = scanner.scan(input_content, input_file);
        
        if (!scanner.getErrors().empty()) {
            std::cerr << "Scanner errors:\n";
            for (const auto& error : scanner.getErrors()) {
                std::cerr << "  " << error.location.file << ":" 
                         << error.location.line << ":" << error.location.column 
                         << " " << error.message << "\n";
            }
            return 1;
        }
        
        std::cout << "Found " << fragments.size() << " code fragments\n";
        
        // 调试：打印片段信息
        for (size_t i = 0; i < fragments.size(); ++i) {
            std::cout << "Fragment " << i << ": type=" 
                     << static_cast<int>(fragments[i].type) 
                     << ", lines " << fragments[i].start_line 
                     << "-" << fragments[i].end_line 
                     << ", content length=" << fragments[i].content.length() << "\n";
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
        
        // TODO: 实现结果合并器
        // ResultMerger merger;
        // std::string final_output = merger.merge(results);
        
        // 合并结果
        std::string final_output;
        
        // 检查是否已经有完整的HTML结构
        bool has_html_structure = false;
        for (const auto& result : results) {
            if (result.output.find("<html") != std::string::npos) {
                has_html_structure = true;
                break;
            }
        }
        
        if (has_html_structure) {
            // 如果已经有HTML结构，直接使用第一个结果（假设只有一个CHTL文件）
            if (!results.empty()) {
                final_output = results[0].output;
                
                // 将全局样式插入到</head>之前
                if (!results[0].global_styles.empty()) {
                    size_t head_end = final_output.find("</head>");
                    if (head_end != std::string::npos) {
                        final_output.insert(head_end, results[0].global_styles + "\n");
                    }
                }
            }
        } else {
            // 创建HTML结构
            final_output = "<!DOCTYPE html>\n<html>\n<head>\n";
            
            // 收集所有全局样式
            for (const auto& result : results) {
                if (!result.global_styles.empty()) {
                    final_output += result.global_styles;
                }
            }
            
            final_output += "</head>\n<body>\n";
            
            // 收集所有主体内容
            for (const auto& result : results) {
                if (!result.output.empty()) {
                    final_output += result.output;
                }
            }
            
            // 收集所有全局脚本
            for (const auto& result : results) {
                if (!result.global_scripts.empty()) {
                    final_output += result.global_scripts;
                }
            }
            
            final_output += "</body>\n</html>\n";
        }
        
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