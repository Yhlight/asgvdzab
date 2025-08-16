#include "JavaScriptCompiler.hpp"

JavaScriptCompiler::JavaScriptCompiler() {
}

JavaScriptCompiler::~JavaScriptCompiler() {
}

std::string JavaScriptCompiler::compile(const std::string& source) {
    // 临时实现
    return "<script>" + source + "</script>";
}