#include "CHTLJSCompiler.hpp"

CHTLJSCompiler::CHTLJSCompiler() {
}

CHTLJSCompiler::~CHTLJSCompiler() {
}

std::string CHTLJSCompiler::compile(const std::string& source) {
    // 临时实现
    return "<script>" + source + "</script>";
}