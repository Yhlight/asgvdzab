#pragma once

#include <string>

class CSSCompiler {
public:
    CSSCompiler();
    ~CSSCompiler();
    
    std::string compile(const std::string& source);
};