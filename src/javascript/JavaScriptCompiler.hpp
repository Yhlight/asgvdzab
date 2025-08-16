#pragma once

#include <string>

class JavaScriptCompiler {
public:
    JavaScriptCompiler();
    ~JavaScriptCompiler();
    
    std::string compile(const std::string& source);
};