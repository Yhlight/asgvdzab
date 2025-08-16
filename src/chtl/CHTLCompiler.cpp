#include "CHTLCompiler.hpp"

CHTLCompiler::CHTLCompiler() {
    // TODO: 初始化lexer, parser, generator
    // 暂时注释掉，避免incomplete type错误
    // lexer_ = std::make_unique<CHTLLexer>();
    // parser_ = std::make_unique<CHTLParser>();
    // generator_ = std::make_unique<CHTLGenerator>();
}

CHTLCompiler::~CHTLCompiler() {
}

std::string CHTLCompiler::compile(const std::string& source) {
    // 临时实现：直接返回源代码
    // TODO: 实现完整的编译流程
    return source;
}