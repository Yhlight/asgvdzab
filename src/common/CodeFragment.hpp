#pragma once

#include <string>
#include "FragmentType.hpp"

/**
 * 代码片段
 */
struct CodeFragment {
    FragmentType type;      // 片段类型
    std::string content;    // 片段内容
    size_t startLine;       // 起始行号
    size_t startColumn;     // 起始列号
    size_t endLine;         // 结束行号
    size_t endColumn;       // 结束列号
    
    CodeFragment(FragmentType t, const std::string& c, 
                 size_t sl, size_t sc, size_t el, size_t ec)
        : type(t), content(c), 
          startLine(sl), startColumn(sc), 
          endLine(el), endColumn(ec) {}
};