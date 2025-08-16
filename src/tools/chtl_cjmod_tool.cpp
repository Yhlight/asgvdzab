#include "compiler/cjmod_compiler_extension.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        chtl::CjmodCommandLineInterface cli;
        return cli.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "❌ 运行时错误: " << e.what() << std::endl;
        return 2;
    } catch (...) {
        std::cerr << "❌ 未知错误" << std::endl;
        return 3;
    }
}