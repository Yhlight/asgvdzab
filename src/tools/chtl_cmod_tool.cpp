#include "compiler/cmod_compiler_extension.hpp"
#include <iostream>

/**
 * CHTL Cmod工具 - 独立的命令行工具
 * 严格按照CHTL语法文档实现Cmod体系
 */

int main(int argc, char* argv[]) {
    try {
        chtl::CmodCommandLineInterface cli;
        return cli.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "❌ 运行时错误: " << e.what() << std::endl;
        return 2;
    } catch (...) {
        std::cerr << "❌ 未知错误" << std::endl;
        return 3;
    }
}