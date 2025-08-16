#include "CSSCompiler.hpp"

CSSCompiler::CSSCompiler() {
}

CSSCompiler::~CSSCompiler() {
}

std::string CSSCompiler::compile(const std::string& source) {
    // 临时实现：直接返回包装后的CSS
    return "<style>" + source + "</style>";
}