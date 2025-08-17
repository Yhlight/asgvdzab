#pragma once

#include <string>
#include <vector>
#include <memory>

namespace CHTL {

/**
 * 代码片段类型枚举
 */
enum class FragmentType {
    CHTL,           // CHTL片段
    CHTL_JS,        // CHTL JS片段  
    CSS,            // CSS片段
    JAVASCRIPT,     // JavaScript片段
    HTML,           // HTML片段（原始嵌入）
    UNKNOWN         // 未知类型
};

/**
 * 片段切割级别
 */
enum class FragmentLevel {
    BLOCK,          // 块级片段（完整的代码块）
    STATEMENT,      // 语句级片段（单个语句）
    EXPRESSION,     // 表达式级片段（单个表达式）
    TOKEN,          // 词法单元级片段（最小单元）
    COMPOSITE       // 复合片段（多个相关片段的组合）
};

/**
 * 代码片段位置信息
 */
struct FragmentPosition {
    size_t startLine;       // 起始行号
    size_t endLine;         // 结束行号
    size_t startColumn;     // 起始列号
    size_t endColumn;       // 结束列号
    size_t startOffset;     // 在源代码中的起始偏移
    size_t endOffset;       // 在源代码中的结束偏移
    
    FragmentPosition() : startLine(0), endLine(0), startColumn(0), 
                        endColumn(0), startOffset(0), endOffset(0) {}
    
    FragmentPosition(size_t sl, size_t el, size_t sc, size_t ec, size_t so, size_t eo)
        : startLine(sl), endLine(el), startColumn(sc), endColumn(ec), 
          startOffset(so), endOffset(eo) {}
};

/**
 * 代码片段基类
 */
class CodeFragment {
public:
    FragmentType type;          // 片段类型
    FragmentLevel level;        // 切割级别
    std::string content;        // 片段内容
    FragmentPosition position;  // 位置信息
    std::string context;        // 上下文信息（如所属的HTML元素）
    bool isComplete;            // 是否为完整片段
    std::vector<std::string> dependencies; // 依赖的其他片段
    
    CodeFragment(FragmentType t, FragmentLevel l, const std::string& c, const FragmentPosition& pos)
        : type(t), level(l), content(c), position(pos), isComplete(true) {}
    
    virtual ~CodeFragment() = default;
    
    // 获取类型名称
    std::string getTypeName() const {
        switch (type) {
            case FragmentType::CHTL: return "CHTL";
            case FragmentType::CHTL_JS: return "CHTL_JS";
            case FragmentType::CSS: return "CSS";
            case FragmentType::JAVASCRIPT: return "JavaScript";
            case FragmentType::HTML: return "HTML";
            default: return "UNKNOWN";
        }
    }
    
    // 获取级别名称
    std::string getLevelName() const {
        switch (level) {
            case FragmentLevel::BLOCK: return "BLOCK";
            case FragmentLevel::STATEMENT: return "STATEMENT";
            case FragmentLevel::EXPRESSION: return "EXPRESSION";
            case FragmentLevel::TOKEN: return "TOKEN";
            case FragmentLevel::COMPOSITE: return "COMPOSITE";
            default: return "UNKNOWN";
        }
    }
};

/**
 * CHTL片段
 */
class CHTLFragment : public CodeFragment {
public:
    bool hasLocalStyle;         // 是否包含局部样式
    bool hasLocalScript;        // 是否包含局部脚本
    std::string elementType;    // 元素类型（如div, span等）
    
    CHTLFragment(const std::string& content, const FragmentPosition& pos, FragmentLevel level = FragmentLevel::BLOCK)
        : CodeFragment(FragmentType::CHTL, level, content, pos), 
          hasLocalStyle(false), hasLocalScript(false) {}
};

/**
 * CHTL JS片段
 */
class CHTLJSFragment : public CodeFragment {
public:
    bool isLocalScript;         // 是否为局部脚本
    std::string parentElement;  // 父元素信息
    
    CHTLJSFragment(const std::string& content, const FragmentPosition& pos, FragmentLevel level = FragmentLevel::BLOCK)
        : CodeFragment(FragmentType::CHTL_JS, level, content, pos), isLocalScript(false) {}
};

/**
 * CSS片段
 */
class CSSFragment : public CodeFragment {
public:
    bool isGlobal;              // 是否为全局样式
    std::string selector;       // 选择器信息
    
    CSSFragment(const std::string& content, const FragmentPosition& pos, FragmentLevel level = FragmentLevel::BLOCK)
        : CodeFragment(FragmentType::CSS, level, content, pos), isGlobal(true) {}
};

/**
 * JavaScript片段
 */
class JSFragment : public CodeFragment {
public:
    bool isGlobal;              // 是否为全局脚本
    
    JSFragment(const std::string& content, const FragmentPosition& pos, FragmentLevel level = FragmentLevel::BLOCK)
        : CodeFragment(FragmentType::JAVASCRIPT, level, content, pos), isGlobal(true) {}
};

/**
 * HTML片段（原始嵌入）
 */
class HTMLFragment : public CodeFragment {
public:
    std::string originType;     // 原始类型（@Html, @Vue等）
    
    HTMLFragment(const std::string& content, const FragmentPosition& pos, FragmentLevel level = FragmentLevel::BLOCK)
        : CodeFragment(FragmentType::HTML, level, content, pos) {}
};

using FragmentPtr = std::shared_ptr<CodeFragment>;
using FragmentList = std::vector<FragmentPtr>;

} // namespace CHTL