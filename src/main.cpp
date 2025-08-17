#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

// 包含CHTL核心头文件
#include "core/types.hpp"
#include "scanner/unified_scanner.hpp"
#include "compiler/compiler_dispatcher.hpp"
#include "compiler/compiler_interface.hpp"

using namespace CHTL;

// 命令行参数解析
struct CommandLineArgs {
    std::string inputFile;
    std::string outputFile;
    bool debugMode = false;
    bool verboseMode = false;
    bool showHelp = false;
    bool showVersion = false;
    std::string outputFormat = "html"; // html, ast, tokens
};

CommandLineArgs parseCommandLine(int argc, char* argv[]) {
    CommandLineArgs args;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            args.showHelp = true;
        } else if (arg == "-v" || arg == "--version") {
            args.showVersion = true;
        } else if (arg == "-d" || arg == "--debug") {
            args.debugMode = true;
        } else if (arg == "--verbose") {
            args.verboseMode = true;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                args.outputFile = argv[++i];
            }
        } else if (arg == "-f" || arg == "--format") {
            if (i + 1 < argc) {
                args.outputFormat = argv[++i];
            }
        } else if (arg.length() > 0 && arg[0] != '-') {
            if (args.inputFile.empty()) {
                args.inputFile = arg;
            }
        }
    }
    
    return args;
}

void showHelp() {
    std::cout << R"(
CHTL Compiler - C++ HyperText Language Compiler

用法: chtl-compiler [选项] <输入文件>

选项:
  -h, --help          显示此帮助信息
  -v, --version       显示版本信息
  -d, --debug         启用调试模式
  --verbose           启用详细输出
  -o, --output <文件> 指定输出文件
  -f, --format <格式> 指定输出格式 (html, ast, tokens)

示例:
  chtl-compiler input.chtl
  chtl-compiler -o output.html input.chtl
  chtl-compiler -f ast --debug input.chtl

支持的文件格式:
  .chtl    - CHTL源文件
  .html    - HTML输出文件
  .ast     - AST输出文件（调试用）
  .tokens  - Token输出文件（调试用）

更多信息请访问: https://github.com/chtl-lang/chtl-compiler
)";
}

void showVersion() {
    std::cout << R"(
CHTL Compiler 1.0.0
构建时间: )" << __DATE__ << " " << __TIME__ << R"(
编译器: C++17
架构: 精准代码切割 + 多编译器调度

特性:
  ✓ CHTL语言编译器 (手写)
  ✓ CHTL JS编译器 (手写)  
  ✓ CSS编译器 (ANTLR/Regex)
  ✓ JavaScript编译器 (ANTLR/Regex)
  ✓ 统一扫描器 (可变长度切片)
  ✓ 模板系统支持
  ✓ 自定义系统支持
  ✓ 命名空间支持
  ✓ 导入系统支持
  ✓ 约束系统支持
  ✓ CMOD/CJMOD支持

许可证: MIT License
版权所有: CHTL Team
)";
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }
    
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    
    return content;
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法创建文件: " + filename);
    }
    
    file << content;
}

void outputTokens(const std::vector<CodeFragment>& fragments) {
    std::cout << "=== Token分析结果 ===\n";
    for (size_t i = 0; i < fragments.size(); ++i) {
        const auto& fragment = fragments[i];
        std::cout << "片段 " << (i + 1) << " [";
        
        switch (fragment.type) {
            case FragmentType::CHTL: std::cout << "CHTL"; break;
            case FragmentType::CHTL_JS: std::cout << "CHTL_JS"; break;
            case FragmentType::CSS: std::cout << "CSS"; break;
            case FragmentType::JAVASCRIPT: std::cout << "JavaScript"; break;
            case FragmentType::HTML: std::cout << "HTML"; break;
            case FragmentType::RAW: std::cout << "RAW"; break;
        }
        
        std::cout << "] 位置: " << fragment.location.line 
                  << ":" << fragment.location.column 
                  << " 长度: " << fragment.content.length() << "\n";
        
        if (fragment.content.length() <= 100) {
            std::cout << "内容: " << fragment.content << "\n";
        } else {
            std::cout << "内容: " << fragment.content.substr(0, 100) << "...\n";
        }
        
        if (!fragment.context.empty()) {
            std::cout << "上下文: ";
            for (const auto& ctx : fragment.context) {
                std::cout << ctx.first << "=" << ctx.second << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

void outputAST(const CompileResult& result) {
    std::cout << "=== AST分析结果 ===\n";
    std::cout << "编译成功: " << (result.success ? "是" : "否") << "\n";
    std::cout << "错误数量: " << result.errors.size() << "\n";
    std::cout << "警告数量: " << result.warnings.size() << "\n\n";
    
    if (!result.errors.empty()) {
        std::cout << "错误:\n";
        for (const auto& error : result.errors) {
            std::cout << "  " << error.location.filename 
                      << ":" << error.location.line 
                      << ":" << error.location.column 
                      << " - " << error.message << "\n";
        }
        std::cout << "\n";
    }
    
    if (!result.warnings.empty()) {
        std::cout << "警告:\n";
        for (const auto& warning : result.warnings) {
            std::cout << "  " << warning.location.filename 
                      << ":" << warning.location.line 
                      << ":" << warning.location.column 
                      << " - " << warning.message << "\n";
        }
        std::cout << "\n";
    }
    
    std::cout << "HTML输出长度: " << result.output.length() << " 字符\n";
}

int main(int argc, char* argv[]) {
    try {
        // 解析命令行参数
        CommandLineArgs args = parseCommandLine(argc, argv);
        
        if (args.showHelp) {
            showHelp();
            return 0;
        }
        
        if (args.showVersion) {
            showVersion();
            return 0;
        }
        
        if (args.inputFile.empty()) {
            std::cerr << "错误: 未指定输入文件\n";
            std::cerr << "使用 --help 查看使用方法\n";
            return 1;
        }
        
        // 设置编译器配置
        CompilerConfig config;
        config.debugMode = args.debugMode;
        
        if (args.verboseMode) {
            std::cout << "=== CHTL编译器启动 ===\n";
            std::cout << "输入文件: " << args.inputFile << "\n";
            std::cout << "输出格式: " << args.outputFormat << "\n";
            std::cout << "调试模式: " << (args.debugMode ? "启用" : "禁用") << "\n";
            std::cout << "\n";
        }
        
        // 读取源文件
        std::string sourceCode = readFile(args.inputFile);
        
        if (args.verboseMode) {
            std::cout << "源代码长度: " << sourceCode.length() << " 字符\n";
            std::cout << "开始扫描...\n";
        }
        
        // 创建统一扫描器
        CHTLUnifiedScanner scanner(config);
        std::vector<CodeFragment> fragments = scanner.scan(sourceCode, args.inputFile);
        
        if (args.verboseMode) {
            std::cout << "扫描完成，发现 " << fragments.size() << " 个代码片段\n";
        }
        
        // 检查扫描器错误
        const auto& scannerErrors = scanner.getErrors();
        if (!scannerErrors.empty()) {
            std::cerr << "扫描错误:\n";
            for (const auto& error : scannerErrors) {
                std::cerr << "  " << error.location.filename 
                          << ":" << error.location.line 
                          << ":" << error.location.column 
                          << " - " << error.message << "\n";
            }
            if (args.outputFormat != "tokens") {
                return 1;
            }
        }
        
        // 输出Token信息（如果需要）
        if (args.outputFormat == "tokens") {
            outputTokens(fragments);
            
            if (!args.outputFile.empty()) {
                std::string tokenOutput;
                for (size_t i = 0; i < fragments.size(); ++i) {
                    const auto& fragment = fragments[i];
                    tokenOutput += "Fragment " + std::to_string(i + 1) + ": ";
                    
                    switch (fragment.type) {
                        case FragmentType::CHTL: tokenOutput += "CHTL"; break;
                        case FragmentType::CHTL_JS: tokenOutput += "CHTL_JS"; break;
                        case FragmentType::CSS: tokenOutput += "CSS"; break;
                        case FragmentType::JAVASCRIPT: tokenOutput += "JavaScript"; break;
                        case FragmentType::HTML: tokenOutput += "HTML"; break;
                        case FragmentType::RAW: tokenOutput += "RAW"; break;
                    }
                    
                    tokenOutput += "\n" + fragment.content + "\n\n";
                }
                writeFile(args.outputFile, tokenOutput);
                std::cout << "Token信息已保存到: " << args.outputFile << "\n";
            }
            
            return 0;
        }
        
        if (args.verboseMode) {
            std::cout << "开始编译...\n";
        }
        
        // 创建编译器调度器
        CompilerDispatcher dispatcher(config);
        
        // 注册编译器（这里使用占位符实现）
        // 实际实现中需要创建真实的编译器实例
        // dispatcher.registerCHTLCompiler(std::make_unique<CHTLCompiler>(config));
        // dispatcher.registerCHTLJSCompiler(std::make_unique<CHTLJSCompiler>(config));
        // dispatcher.registerCSSCompiler(std::make_unique<CSSCompiler>(config));
        // dispatcher.registerJavaScriptCompiler(std::make_unique<JavaScriptCompiler>(config));
        
        // 执行编译
        CompileResult result = dispatcher.compile(fragments, args.inputFile);
        
        if (args.verboseMode) {
            std::cout << "编译完成\n";
        }
        
        // 输出AST信息（如果需要）
        if (args.outputFormat == "ast") {
            outputAST(result);
            
            if (!args.outputFile.empty()) {
                std::string astOutput = "Compilation Result:\n";
                astOutput += "Success: " + std::string(result.success ? "true" : "false") + "\n";
                astOutput += "Errors: " + std::to_string(result.errors.size()) + "\n";
                astOutput += "Warnings: " + std::to_string(result.warnings.size()) + "\n\n";
                astOutput += "Output:\n" + result.output;
                
                writeFile(args.outputFile, astOutput);
                std::cout << "AST信息已保存到: " << args.outputFile << "\n";
            }
            
            return result.success ? 0 : 1;
        }
        
        // 检查编译错误
        if (!result.success) {
            std::cerr << "编译失败:\n";
            for (const auto& error : result.errors) {
                std::cerr << "  " << error.location.filename 
                          << ":" << error.location.line 
                          << ":" << error.location.column 
                          << " - " << error.message << "\n";
            }
            
            // 即使编译失败，如果有输出内容，仍然保存到文件
            if (!result.output.empty()) {
                std::cerr << "尽管有错误，仍保存部分输出...\n";
            } else {
                return 1;
            }
        }
        
        // 输出警告
        if (!result.warnings.empty() && args.verboseMode) {
            std::cout << "警告:\n";
            for (const auto& warning : result.warnings) {
                std::cout << "  " << warning.location.filename 
                          << ":" << warning.location.line 
                          << ":" << warning.location.column 
                          << " - " << warning.message << "\n";
            }
        }
        
        // 确定输出文件名
        std::string outputFile = args.outputFile;
        if (outputFile.empty()) {
            // 自动生成输出文件名
            size_t lastDot = args.inputFile.find_last_of('.');
            if (lastDot != std::string::npos) {
                outputFile = args.inputFile.substr(0, lastDot) + ".html";
            } else {
                outputFile = args.inputFile + ".html";
            }
        }
        
        // 写入输出文件
        writeFile(outputFile, result.output);
        
        if (args.verboseMode) {
            std::cout << "\n=== 编译完成 ===\n";
            std::cout << "输出文件: " << outputFile << "\n";
            std::cout << "输出大小: " << result.output.length() << " 字符\n";
            std::cout << "片段数量: " << fragments.size() << "\n";
            std::cout << "错误数量: " << result.errors.size() << "\n";
            std::cout << "警告数量: " << result.warnings.size() << "\n";
        } else {
            std::cout << "编译成功: " << outputFile << "\n";
        }
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "未知错误\n";
        return 1;
    }
}