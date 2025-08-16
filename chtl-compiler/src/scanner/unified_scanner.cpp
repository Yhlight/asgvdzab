#include "scanner/unified_scanner.h"
#include "common/utils.h"
#include <algorithm>
#include <cctype>
#include <set>
#include <regex>

namespace chtl {
namespace scanner {

// CHTL关键字集合
static const std::set<std::string> CHTL_KEYWORDS = {
    "text", "style", "script", "inherit", "delete", "insert",
    "after", "before", "replace", "at", "top", "bottom",
    "from", "as", "except", "Template", "Custom", "Import",
    "Namespace", "Origin", "Configuration"
};

// HTML元素名集合（常见元素）
static const std::set<std::string> HTML_ELEMENTS = {
    "html", "head", "body", "div", "span", "p", "a", "img",
    "ul", "ol", "li", "table", "tr", "td", "th", "form",
    "input", "button", "textarea", "select", "option",
    "h1", "h2", "h3", "h4", "h5", "h6", "header", "footer",
    "nav", "section", "article", "aside", "main", "br", "hr"
};

UnifiedScanner::UnifiedScanner() : defaultSliceSize_(1024) {
    clearDiagnostics();
}

UnifiedScanner::~UnifiedScanner() = default;

std::vector<CodeSegment> UnifiedScanner::scan(const std::string& source, 
                                             const std::string& filename) {
    context_ = ScannerContext();
    context_.currentFile = filename;
    clearDiagnostics();
    
    // 第一阶段：初步切片
    auto slices = performInitialSlicing(source);
    
    // 第二阶段：基于最小单元的精确切割
    auto segments = performPreciseSlicing(source, slices);
    
    return segments;
}

std::vector<SliceInfo> UnifiedScanner::performInitialSlicing(const std::string& source) {
    std::vector<SliceInfo> slices;
    size_t position = 0;
    
    while (position < source.length()) {
        // 确定切片结束位置
        size_t sliceEnd = std::min(position + defaultSliceSize_, source.length());
        
        // 检测切片类型
        CodeSegmentType type = detectSegmentType(source, position);
        
        // 创建切片
        SliceInfo slice(position, sliceEnd, type);
        
        // 检查切片边界是否合理
        if (sliceEnd < source.length() && !isSliceBoundaryValid(source, sliceEnd)) {
            slice.needsExpansion = true;
            expandSlice(source, slice);
        }
        
        slices.push_back(slice);
        position = slice.end;
    }
    
    return slices;
}

bool UnifiedScanner::isSliceBoundaryValid(const std::string& source, size_t position) {
    if (position >= source.length()) return true;
    
    // 检查是否在字符串中
    bool inString = false;
    char stringChar = '\0';
    for (size_t i = 0; i < position; ++i) {
        if (!inString && (source[i] == '"' || source[i] == '\'' || source[i] == '`')) {
            inString = true;
            stringChar = source[i];
        } else if (inString && source[i] == stringChar && (i == 0 || source[i-1] != '\\')) {
            inString = false;
        }
    }
    if (inString) return false;
    
    // 检查是否在注释中
    if (position > 0) {
        // 单行注释
        for (size_t i = 0; i < position - 1; ++i) {
            if (source[i] == '/' && source[i+1] == '/') {
                // 找到注释开始，检查是否已经结束
                size_t j = i + 2;
                while (j < position && source[j] != '\n') j++;
                if (j >= position) return false; // 仍在注释中
                i = j; // 跳过已处理的注释
            }
        }
        
        // 多行注释
        bool inComment = false;
        for (size_t i = 0; i < position; ++i) {
            if (!inComment && i < position - 1 && source[i] == '/' && source[i+1] == '*') {
                inComment = true;
                i++;
            } else if (inComment && i < position - 1 && source[i] == '*' && source[i+1] == '/') {
                inComment = false;
                i++;
            }
        }
        if (inComment) return false;
    }
    
    // 检查是否在增强选择器中 {{...}}
    if (position >= 2) {
        // 向前查找最近的{{
        for (size_t i = position - 1; i > 0; --i) {
            if (source[i-1] == '{' && source[i] == '{') {
                // 找到{{，检查是否有对应的}}
                size_t j = i + 1;
                int depth = 1;
                while (j < position && depth > 0) {
                    if (j < source.length() - 1 && source[j] == '}' && source[j+1] == '}') {
                        depth--;
                        j += 2;
                    } else {
                        j++;
                    }
                }
                if (depth > 0) return false; // 仍在增强选择器中
                break;
            }
        }
    }
    
    // 检查是否在箭头操作符中间
    if (position > 0 && position < source.length()) {
        if (source[position-1] == '-' && source[position] == '>') {
            return false;
        }
    }
    
    // 检查花括号平衡
    int braceCount = 0;
    for (size_t i = 0; i < position; ++i) {
        if (source[i] == '{') braceCount++;
        else if (source[i] == '}') braceCount--;
    }
    
    // 如果花括号不平衡，尝试找到平衡点
    if (braceCount != 0) {
        // 检查下一个字符是否能帮助平衡
        if (position < source.length()) {
            if ((braceCount > 0 && source[position] == '}') ||
                (braceCount < 0 && source[position] == '{')) {
                return false; // 需要包含下一个字符
            }
        }
    }
    
    return true;
}

void UnifiedScanner::expandSlice(const std::string& source, SliceInfo& slice) {
    // 扩展切片直到找到合理的边界
    size_t maxExpansion = defaultSliceSize_ / 2; // 最多扩展50%
    size_t originalEnd = slice.end;
    
    while (slice.end < source.length() && 
           slice.end < originalEnd + maxExpansion &&
           !isSliceBoundaryValid(source, slice.end)) {
        slice.end++;
    }
    
    // 如果扩展后仍然无效，继续扩展直到找到有效边界
    while (slice.end < source.length() && !isSliceBoundaryValid(source, slice.end)) {
        slice.end++;
    }
}

std::vector<CodeSegment> UnifiedScanner::performPreciseSlicing(const std::string& source,
                                                              const std::vector<SliceInfo>& slices) {
    std::vector<CodeSegment> segments;
    
    for (const auto& slice : slices) {
        std::string content = source.substr(slice.start, slice.end - slice.start);
        
        // 识别最小单元
        auto units = identifyMinimalUnits(content, slice.type);
        
        // 合并连续的最小单元
        auto mergedSegments = mergeMinimalUnits(units, slice, source);
        
        // 添加到结果中
        segments.insert(segments.end(), mergedSegments.begin(), mergedSegments.end());
    }
    
    return segments;
}

std::vector<MinimalUnit> UnifiedScanner::identifyMinimalUnits(const std::string& content,
                                                             CodeSegmentType type) {
    std::vector<MinimalUnit> units;
    size_t position = 0;
    
    while (position < content.length()) {
        MinimalUnit unit;
        unit.position = position;
        
        char ch = content[position];
        
        // 跳过空白
        if (std::isspace(ch)) {
            size_t start = position;
            while (position < content.length() && std::isspace(content[position])) {
                position++;
            }
            unit.type = TokenUnit::WHITESPACE;
            unit.content = content.substr(start, position - start);
            unit.length = position - start;
            units.push_back(unit);
            continue;
        }
        
        // 检查增强选择器 {{...}}
        if (position < content.length() - 1 && ch == '{' && content[position + 1] == '{') {
            unit = extractEnhancedSelector(content, position);
            units.push_back(unit);
            continue;
        }
        
        // 检查箭头操作符 ->
        if (position < content.length() - 1 && ch == '-' && content[position + 1] == '>') {
            unit = extractArrowOperator(content, position);
            units.push_back(unit);
            continue;
        }
        
        // 检查注释
        if (position < content.length() - 1 && ch == '/') {
            if (content[position + 1] == '/') {
                // 单行注释
                size_t start = position;
                position += 2;
                while (position < content.length() && content[position] != '\n') {
                    position++;
                }
                unit.type = TokenUnit::COMMENT;
                unit.content = content.substr(start, position - start);
                unit.length = position - start;
                units.push_back(unit);
                continue;
            } else if (content[position + 1] == '*') {
                // 多行注释
                size_t start = position;
                position += 2;
                while (position < content.length() - 1) {
                    if (content[position] == '*' && content[position + 1] == '/') {
                        position += 2;
                        break;
                    }
                    position++;
                }
                unit.type = TokenUnit::COMMENT;
                unit.content = content.substr(start, position - start);
                unit.length = position - start;
                units.push_back(unit);
                continue;
            }
        }
        
        // 检查字符串
        if (ch == '"' || ch == '\'' || ch == '`') {
            size_t start = position;
            char quote = ch;
            position++;
            while (position < content.length()) {
                if (content[position] == quote && content[position - 1] != '\\') {
                    position++;
                    break;
                }
                position++;
            }
            unit.type = TokenUnit::STRING_LITERAL;
            unit.content = content.substr(start, position - start);
            unit.length = position - start;
            units.push_back(unit);
            continue;
        }
        
        // 检查标识符和关键字
        if (std::isalpha(ch) || ch == '_') {
            size_t start = position;
            while (position < content.length() && 
                   (std::isalnum(content[position]) || content[position] == '_')) {
                position++;
            }
            std::string word = content.substr(start, position - start);
            unit.type = (CHTL_KEYWORDS.find(word) != CHTL_KEYWORDS.end()) ?
                       TokenUnit::KEYWORD : TokenUnit::IDENTIFIER;
            unit.content = word;
            unit.length = position - start;
            units.push_back(unit);
            continue;
        }
        
        // 特殊字符
        switch (ch) {
            case '{':
                unit.type = TokenUnit::BRACE_OPEN;
                unit.content = "{";
                unit.length = 1;
                position++;
                break;
            case '}':
                unit.type = TokenUnit::BRACE_CLOSE;
                unit.content = "}";
                unit.length = 1;
                position++;
                break;
            case '(':
                unit.type = TokenUnit::PAREN_OPEN;
                unit.content = "(";
                unit.length = 1;
                position++;
                break;
            case ')':
                unit.type = TokenUnit::PAREN_CLOSE;
                unit.content = ")";
                unit.length = 1;
                position++;
                break;
            case '.':
                unit.type = TokenUnit::DOT_OPERATOR;
                unit.content = ".";
                unit.length = 1;
                position++;
                break;
            default:
                unit.type = TokenUnit::OTHER;
                unit.content = std::string(1, ch);
                unit.length = 1;
                position++;
                break;
        }
        
        units.push_back(unit);
    }
    
    return units;
}

MinimalUnit UnifiedScanner::extractEnhancedSelector(const std::string& content, size_t& position) {
    MinimalUnit unit;
    unit.type = TokenUnit::ENHANCED_SELECTOR;
    unit.position = position;
    
    size_t start = position;
    position += 2; // 跳过 {{
    
    int depth = 1;
    while (position < content.length() - 1 && depth > 0) {
        if (content[position] == '{' && content[position + 1] == '{') {
            depth++;
            position += 2;
        } else if (content[position] == '}' && content[position + 1] == '}') {
            depth--;
            position += 2;
        } else {
            position++;
        }
    }
    
    unit.content = content.substr(start, position - start);
    unit.length = position - start;
    
    return unit;
}

MinimalUnit UnifiedScanner::extractArrowOperator(const std::string& content, size_t& position) {
    MinimalUnit unit;
    unit.type = TokenUnit::ARROW_OPERATOR;
    unit.position = position;
    unit.content = "->";
    unit.length = 2;
    position += 2;
    
    return unit;
}

std::vector<CodeSegment> UnifiedScanner::mergeMinimalUnits(const std::vector<MinimalUnit>& units,
                                                          const SliceInfo& slice,
                                                          const std::string& source) {
    std::vector<CodeSegment> segments;
    if (units.empty()) return segments;
    
    // 开始合并逻辑
    size_t i = 0;
    while (i < units.size()) {
        CodeSegment segment;
        segment.type = slice.type;
        segment.location.filename = context_.currentFile;
        
        // 跳过前导空白
        while (i < units.size() && units[i].type == TokenUnit::WHITESPACE) {
            i++;
        }
        
        if (i >= units.size()) break;
        
        size_t segmentStart = i;
        size_t absoluteStart = slice.start + units[i].position;
        
        // 确定segment的起始位置
        updateLocation(source, absoluteStart, absoluteStart + units[i].length);
        segment.location.line = context_.lineNumber;
        segment.location.column = context_.columnNumber;
        segment.location.offset = absoluteStart;
        
        // 根据不同情况合并单元
        if (units[i].type == TokenUnit::ENHANCED_SELECTOR) {
            // {{selector}} 开始的序列
            i++;
            
            // 继续包含后续的操作符和调用
            while (i < units.size()) {
                if (units[i].type == TokenUnit::WHITESPACE) {
                    i++;
                    continue;
                }
                
                if (units[i].type == TokenUnit::ARROW_OPERATOR ||
                    units[i].type == TokenUnit::DOT_OPERATOR) {
                    i++;
                    // 包含后面的方法调用
                    while (i < units.size() && 
                           (units[i].type == TokenUnit::WHITESPACE ||
                            units[i].type == TokenUnit::IDENTIFIER ||
                            units[i].type == TokenUnit::PAREN_OPEN ||
                            units[i].type == TokenUnit::PAREN_CLOSE)) {
                        i++;
                    }
                } else {
                    break;
                }
            }
        } else if (units[i].type == TokenUnit::IDENTIFIER && 
                   (units[i].content == "style" || units[i].content == "script")) {
            // style/script 块
            i++;
            int braceDepth = 0;
            bool foundOpenBrace = false;
            
            while (i < units.size()) {
                if (units[i].type == TokenUnit::BRACE_OPEN) {
                    braceDepth++;
                    foundOpenBrace = true;
                } else if (units[i].type == TokenUnit::BRACE_CLOSE) {
                    braceDepth--;
                    if (braceDepth == 0 && foundOpenBrace) {
                        i++;
                        break;
                    }
                }
                i++;
            }
        } else {
            // 其他情况，合并到合理的边界
            i++;
            while (i < units.size() && shouldMergeUnits(units[i-1], units[i])) {
                i++;
            }
        }
        
        // 构建segment内容
        size_t endUnit = std::min(i, units.size());
        if (endUnit > segmentStart) {
            size_t contentStart = units[segmentStart].position;
            size_t contentEnd = units[endUnit-1].position + units[endUnit-1].length;
            segment.content = source.substr(absoluteStart, 
                                          slice.start + contentEnd - absoluteStart);
            
            // 处理特殊的上下文信息
            if (segment.type == CodeSegmentType::CHTL_JS || 
                segment.type == CodeSegmentType::CHTL) {
                segment.parentElement = context_.currentElement;
                segment.className = context_.currentElement + "_auto";
                // ID会在编译时自动生成
            }
            
            segments.push_back(segment);
        }
    }
    
    return segments;
}

bool UnifiedScanner::shouldMergeUnits(const MinimalUnit& prev, const MinimalUnit& curr) {
    // 空白总是可以合并
    if (curr.type == TokenUnit::WHITESPACE) return true;
    
    // 注释独立成段
    if (prev.type == TokenUnit::COMMENT || curr.type == TokenUnit::COMMENT) return false;
    
    // 增强选择器序列
    if (prev.type == TokenUnit::ENHANCED_SELECTOR && 
        (curr.type == TokenUnit::ARROW_OPERATOR || curr.type == TokenUnit::DOT_OPERATOR)) {
        return true;
    }
    
    // 箭头操作符后的标识符
    if ((prev.type == TokenUnit::ARROW_OPERATOR || prev.type == TokenUnit::DOT_OPERATOR) &&
        curr.type == TokenUnit::IDENTIFIER) {
        return true;
    }
    
    // 函数调用
    if (prev.type == TokenUnit::IDENTIFIER && curr.type == TokenUnit::PAREN_OPEN) {
        return true;
    }
    
    // 括号内容
    if (prev.type == TokenUnit::PAREN_OPEN && curr.type != TokenUnit::PAREN_CLOSE) {
        return true;
    }
    
    // 默认不合并
    return false;
}

CodeSegmentType UnifiedScanner::detectSegmentType(const std::string& content, size_t position) {
    // 简单的类型检测逻辑
    // 实际实现中需要更复杂的上下文分析
    
    // 检查是否在style块中
    size_t stylePos = content.rfind("style", position);
    if (stylePos != std::string::npos) {
        size_t bracePos = content.find('{', stylePos);
        if (bracePos != std::string::npos && bracePos < position) {
            // 检查是否是局部style（在元素内）
            if (context_.braceDepth > 0) {
                return CodeSegmentType::CHTL;
            } else {
                return CodeSegmentType::CSS;
            }
        }
    }
    
    // 检查是否在script块中
    size_t scriptPos = content.rfind("script", position);
    if (scriptPos != std::string::npos) {
        size_t bracePos = content.find('{', scriptPos);
        if (bracePos != std::string::npos && bracePos < position) {
            // 检查是否包含CHTL JS特性
            if (content.find("{{", bracePos) != std::string::npos ||
                content.find("->", bracePos) != std::string::npos) {
                return CodeSegmentType::CHTL_JS;
            } else {
                return CodeSegmentType::JAVASCRIPT;
            }
        }
    }
    
    // 检查[Origin]块
    size_t originPos = content.rfind("[Origin]", position);
    if (originPos != std::string::npos) {
        size_t atPos = content.find('@', originPos);
        if (atPos != std::string::npos) {
            std::string type = content.substr(atPos + 1, 10);
            if (type.find("Html") == 0) return CodeSegmentType::HTML_RAW;
            if (type.find("Style") == 0) return CodeSegmentType::CSS;
            if (type.find("JavaScript") == 0) return CodeSegmentType::JAVASCRIPT;
        }
    }
    
    // 默认为CHTL
    return CodeSegmentType::CHTL;
}

void UnifiedScanner::updateLocation(const std::string& content, size_t start, size_t end) {
    for (size_t i = start; i < end && i < content.length(); ++i) {
        if (content[i] == '\n') {
            context_.lineNumber++;
            context_.columnNumber = 1;
        } else {
            context_.columnNumber++;
        }
    }
}

void UnifiedScanner::addError(const std::string& message) {
    errors_.push_back(utils::formatError(context_.currentFile, 
                                        context_.lineNumber, 
                                        context_.columnNumber, 
                                        message));
}

void UnifiedScanner::addWarning(const std::string& message) {
    warnings_.push_back(utils::formatWarning(context_.currentFile, 
                                            context_.lineNumber, 
                                            context_.columnNumber, 
                                            message));
}

} // namespace scanner
} // namespace chtl