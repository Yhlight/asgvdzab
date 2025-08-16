#include <iostream>
#include <fstream>
#include <sstream>
#include "chtl/CHTLContext.hpp"
#include "chtl/CHTLLexer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "用法: " << argv[0] << " <chtl文件>" << std::endl;
        return 1;
    }
    
    // 读取文件
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << argv[1] << std::endl;
        return 1;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    // 创建上下文和词法分析器
    chtl::CHTLContext context;
    context.setCurrentFile(argv[1]);
    chtl::CHTLLexer lexer(context);
    
    // 进行词法分析
    auto tokens = lexer.tokenize(source);
    
    // 输出Token
    std::cout << "Token列表:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }
    
    // 输出错误信息
    if (context.hasErrors()) {
        std::cout << "\n错误:" << std::endl;
        for (const auto& error : context.getErrors()) {
            std::cout << error << std::endl;
        }
    }
    
    // 输出统计信息
    std::cout << "\n统计信息:" << std::endl;
    std::cout << "总Token数: " << tokens.size() << std::endl;
    
    return 0;
}