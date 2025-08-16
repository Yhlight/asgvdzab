#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer/chtl_lexer.hpp"
#include "ast/chtl_ast.hpp"
#include "generators/chtl_generator.hpp"
#include "core/integrated_compiler_system.hpp"

using namespace chtl;

/**
 * 读取文件内容
 */
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * 写入文件内容
 */
void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法创建文件: " + filename);
    }
    
    file << content;
}

/**
 * 获取输出文件名
 */
std::string getOutputFilename(const std::string& inputFile) {
    size_t lastDot = inputFile.find_last_of('.');
    if (lastDot == std::string::npos) {
        return inputFile + ".html";
    }
    
    return inputFile.substr(0, lastDot) + ".html";
}

/**
 * 显示使用帮助
 */
void showUsage(const char* programName) {
    std::cout << "用法: " << programName << " <input.chtl> [options]\n";
    std::cout << "\n选项:\n";
    std::cout << "  -h, --help     显示此帮助信息\n";
    std::cout << "  -v, --verbose  显示详细信息\n";
    std::cout << "  -o <file>      指定输出文件\n";
    std::cout << "\n示例:\n";
    std::cout << "  " << programName << " example.chtl\n";
    std::cout << "  " << programName << " example.chtl -o output.html\n";
    std::cout << "  " << programName << " example.chtl -v\n";
}

int main(int argc, char* argv[]) {
    try {
        // 解析命令行参数
        if (argc < 2) {
            showUsage(argv[0]);
            return 1;
        }
        
        std::string inputFile;
        std::string outputFile;
        bool verbose = false;
        
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg == "-h" || arg == "--help") {
                showUsage(argv[0]);
                return 0;
            } else if (arg == "-v" || arg == "--verbose") {
                verbose = true;
            } else if (arg == "-o" && i + 1 < argc) {
                outputFile = argv[++i];
            } else if (inputFile.empty()) {
                inputFile = arg;
            } else {
                std::cerr << "错误: 未知参数 " << arg << std::endl;
                showUsage(argv[0]);
                return 1;
            }
        }
        
        if (inputFile.empty()) {
            std::cerr << "错误: 未指定输入文件" << std::endl;
            showUsage(argv[0]);
            return 1;
        }
        
        if (outputFile.empty()) {
            outputFile = getOutputFilename(inputFile);
        }
        
        if (verbose) {
            std::cout << "输入文件: " << inputFile << std::endl;
            std::cout << "输出文件: " << outputFile << std::endl;
            std::cout << std::endl;
        }
        
        // 读取输入文件
        std::string sourceCode = readFile(inputFile);
        
        if (verbose) {
            std::cout << "源代码长度: " << sourceCode.length() << " 字符" << std::endl;
        }
        
        // 创建集成编译器系统
        auto compilerSystem = CompilerSystemFactory::createDefaultSystem();
        
        if (verbose) {
            std::cout << "编译器系统初始化完成" << std::endl;
        }
        
        // 执行编译
        auto result = compilerSystem->compile(sourceCode);
        
        if (verbose) {
            std::cout << "编译完成" << std::endl;
            std::cout << "生成HTML长度: " << result.output.length() << " 字符" << std::endl;
            
            if (!result.errors.empty()) {
                std::cout << "编译警告/错误: " << result.errors.size() << " 个" << std::endl;
                for (const auto& error : result.errors) {
                    std::cout << "  - " << error << std::endl;
                }
            }
        }
        
        // 写入输出文件
        writeFile(outputFile, result.output);
        
        std::cout << "编译成功! 输出文件: " << outputFile << std::endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
}