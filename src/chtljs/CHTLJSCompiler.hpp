#pragma once

#include <string>

class CHTLJSCompiler {
public:
    CHTLJSCompiler();
    ~CHTLJSCompiler();
    
    std::string compile(const std::string& source);
};