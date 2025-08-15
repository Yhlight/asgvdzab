#include "scanner/slice_scanner.hpp"
#include <algorithm>
#include <cctype>
#include <regex>
#include <sstream>

namespace chtl {

SliceScanner::SliceScanner(const CompileConfig& config) 
    : config_(config), initialSliceSize_(1024), maxExpansions_(5) {
}

ScanResult SliceScanner::scanWithSlices(const std::string& source, const std::string& filename) {
    source_ = source;
    filename_ = filename;
    
    ScanResult result;
    
    try {
        // 预处理源代码
        preprocessSource();
        
        // 构建行位置映射
        buildLinePositions();
        
        // 生成初始切片
        auto initialSlices = generateInitialSlices();
        
        // 验证和扩展切片
        auto validatedSlices = validateAndExpandSlices(initialSlices);
        
        // 执行最小单元切割
        std::vector<CodeSlice> refinedSlices;
        for (const auto& slice : validatedSlices) {
            if (slice.suggestedType == CodeSegmentType::CHTL || 
                slice.suggestedType == CodeSegmentType::CHTL_JS) {
                auto minimalSlices = performMinimalUnitSlicing(slice);
                refinedSlices.insert(refinedSlices.end(), minimalSlices.begin(), minimalSlices.end());
            } else {
                refinedSlices.push_back(slice);
            }
        }
        
        // 基于上下文合并切片
        auto mergedSlices = mergeContextualSlices(refinedSlices);
        
        // 转换为代码片段
        result.segments = convertSlicesToSegments(mergedSlices);
        result.success = true;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back("扫描异常: " + std::string(e.what()));
    }
    
    return result;
}

void SliceScanner::setInitialSliceSize(size_t size) {
    initialSliceSize_ = size;
}

void SliceScanner::setMaxExpansions(size_t maxExpansions) {
    maxExpansions_ = maxExpansions;
}

std::vector<CodeSlice> SliceScanner::generateInitialSlices() {
    std::vector<CodeSlice> slices;
    
    size_t pos = 0;
    while (pos < source_.length()) {
        size_t sliceEnd = std::min(pos + initialSliceSize_, source_.length());
        
        // 确保不在字符串或注释中间切断
        while (sliceEnd < source_.length() && sliceEnd > pos) {
            char ch = source_[sliceEnd];
            if (ch == '\n' || ch == ';' || ch == '}' || std::isspace(ch)) {
                break;
            }
            sliceEnd++;
        }
        
        if (sliceEnd > pos && pos < source_.length()) {
            size_t length = std::min(sliceEnd - pos, source_.length() - pos);
            std::string content = source_.substr(pos, length);
            sliceEnd = pos + length; // 调整实际结束位置
            
            CodeSlice slice(content, pos, sliceEnd);
            slice.startPos = calculatePosition(pos);
            slice.endPos = calculatePosition(sliceEnd);
            slice.suggestedType = detectCodeType(content);
            slices.push_back(slice);
        }
        
        pos = sliceEnd;
    }
    
    return slices;
}

std::vector<CodeSlice> SliceScanner::validateAndExpandSlices(std::vector<CodeSlice>& slices) {
    std::vector<CodeSlice> result;
    
    for (size_t i = 0; i < slices.size(); ++i) {
        CodeSlice& current = slices[i];
        CodeSlice* next = (i + 1 < slices.size()) ? &slices[i + 1] : nullptr;
        
        size_t expansions = 0;
        while (!isSliceComplete(current, next) && expansions < maxExpansions_) {
            // 计算可扩展的长度
            size_t availableLength = next ? (next->endOffset - current.endOffset) : 
                                           (source_.length() - current.endOffset);
            
            if (availableLength == 0) break;
            
            // 扩展切片
            current = expandSlice(current, availableLength);
            
                         // 更新下一个切片的起始位置
             if (next && current.endOffset < source_.length()) {
                 next->startOffset = current.endOffset;
                 next->startPos = calculatePosition(next->startOffset);
                 
                 // 确保不越界
                 if (next->endOffset > next->startOffset && next->startOffset < source_.length()) {
                     size_t length = std::min(next->endOffset - next->startOffset, 
                                            source_.length() - next->startOffset);
                     next->content = source_.substr(next->startOffset, length);
                     next->endOffset = next->startOffset + length; // 调整结束位置
                 }
             }
            
            expansions++;
        }
        
        current.isComplete = isSliceComplete(current, next);
        result.push_back(current);
    }
    
    return result;
}

bool SliceScanner::isSliceComplete(const CodeSlice& slice, const CodeSlice* nextSlice) {
    const std::string& content = slice.content;
    
    // 检查大括号平衡
    int braceCount = 0;
    bool inString = false;
    char stringDelim = 0;
    
    for (size_t i = 0; i < content.length(); ++i) {
        char ch = content[i];
        
        if (!inString) {
            if (ch == '"' || ch == '\'') {
                inString = true;
                stringDelim = ch;
            } else if (ch == '{') {
                braceCount++;
            } else if (ch == '}') {
                braceCount--;
            }
        } else {
            if (ch == stringDelim && (i == 0 || content[i-1] != '\\')) {
                inString = false;
            }
        }
    }
    
    // 如果大括号不平衡，需要扩展
    if (braceCount != 0) {
        return false;
    }
    
    // 检查是否在CHTL选择器中间被截断
    if (content.find("{{") != std::string::npos) {
        size_t openPos = content.rfind("{{");
        size_t closePos = content.find("}}", openPos);
        if (closePos == std::string::npos) {
            // 检查下一个切片是否包含闭合
            if (nextSlice && nextSlice->content.find("}}") != std::string::npos) {
                return false;
            }
        }
    }
    
    // 检查是否在CHTL箭头操作符中间被截断
    if (content.find("->") != std::string::npos || 
        (content.back() == '-' && nextSlice && nextSlice->content.front() == '>')) {
        return false;
    }
    
    return true;
}

CodeSlice SliceScanner::expandSlice(const CodeSlice& slice, size_t availableLength) {
    // 智能扩展：寻找合适的截断点
    size_t expandLength = std::min(availableLength, initialSliceSize_ / 2);
    
    // 寻找最佳截断点
    for (size_t i = expandLength; i > 0; --i) {
        size_t checkPos = slice.endOffset + i;
        if (checkPos >= source_.length()) continue;
        
        char ch = source_[checkPos];
        if (ch == '\n' || ch == ';' || ch == '}' || std::isspace(ch)) {
            expandLength = i + 1;
            break;
        }
    }
    
    size_t newEndOffset = std::min(slice.endOffset + expandLength, source_.length());
    size_t contentLength = newEndOffset - slice.startOffset;
    std::string newContent = source_.substr(slice.startOffset, contentLength);
    
    CodeSlice expandedSlice(newContent, slice.startOffset, newEndOffset);
    expandedSlice.startPos = slice.startPos;
    expandedSlice.endPos = calculatePosition(newEndOffset);
    expandedSlice.suggestedType = detectCodeType(newContent);
    
    return expandedSlice;
}

std::vector<CodeSlice> SliceScanner::performMinimalUnitSlicing(const CodeSlice& slice) {
    std::vector<CodeSlice> result;
    
    if (slice.suggestedType == CodeSegmentType::CHTL_JS) {
        // 标记化CHTL JS代码
        auto tokens = tokenizeCHTLJS(slice.content);
        
        size_t currentPos = slice.startOffset;
        for (const auto& token : tokens) {
            size_t tokenStart = slice.startOffset + token.startOffset;
            size_t tokenEnd = slice.startOffset + token.endOffset;
            
                         if (tokenStart < source_.length() && tokenEnd <= source_.length()) {
                std::string tokenContent = source_.substr(tokenStart, tokenEnd - tokenStart);
                CodeSlice tokenSlice(tokenContent, tokenStart, tokenEnd);
                tokenSlice.startPos = calculatePosition(tokenStart);
                tokenSlice.endPos = calculatePosition(tokenEnd);
                tokenSlice.suggestedType = CodeSegmentType::CHTL_JS;
                
                result.push_back(tokenSlice);
                currentPos = tokenEnd;
            }
        }
    } else {
        // 对于CHTL，保持原始切片
        result.push_back(slice);
    }
    
    return result;
}

std::vector<CodeSlice> SliceScanner::mergeContextualSlices(const std::vector<CodeSlice>& slices) {
    std::vector<CodeSlice> result;
    
    if (slices.empty()) return result;
    
    CodeSlice current = slices[0];
    
    for (size_t i = 1; i < slices.size(); ++i) {
        const CodeSlice& next = slices[i];
        
        if (shouldMergeSlices(current, next)) {
            current = mergeSlices(current, next);
        } else {
            result.push_back(current);
            current = next;
        }
    }
    
    result.push_back(current);
    return result;
}

bool SliceScanner::shouldMergeSlices(const CodeSlice& current, const CodeSlice& next) {
    // 合并连续的CHTL JS片段
    if (current.suggestedType == CodeSegmentType::CHTL_JS && 
        next.suggestedType == CodeSegmentType::CHTL_JS) {
        
        // 检查是否相邻
        if (next.startOffset <= current.endOffset + 10) { // 允许小间隙
            return true;
        }
    }
    
    // 合并连续的CHTL片段（在同一个作用域内）
    if (current.suggestedType == CodeSegmentType::CHTL && 
        next.suggestedType == CodeSegmentType::CHTL) {
        
        // 简单的启发式：如果下一个片段很小，可能是被过度切割了
        if (next.content.length() < 50 && next.startOffset <= current.endOffset + 5) {
            return true;
        }
    }
    
    return false;
}

CodeSlice SliceScanner::mergeSlices(const CodeSlice& first, const CodeSlice& second) {
    size_t newStartOffset = first.startOffset;
    size_t newEndOffset = second.endOffset;
    
    size_t contentLength = std::min(newEndOffset - newStartOffset, source_.length() - newStartOffset);
    std::string newContent = source_.substr(newStartOffset, contentLength);
    
    CodeSlice merged(newContent, newStartOffset, newEndOffset);
    merged.startPos = first.startPos;
    merged.endPos = second.endPos;
    merged.isComplete = true;
    
    // 重新检测类型
    merged.suggestedType = detectCodeType(newContent);
    
    return merged;
}

std::vector<CodeSegment> SliceScanner::convertSlicesToSegments(const std::vector<CodeSlice>& slices) {
    std::vector<CodeSegment> segments;
    
    for (const auto& slice : slices) {
        CodeSegment segment(slice.suggestedType, slice.content, 
                           Range{slice.startPos, slice.endPos});
        
        // 提取上下文信息
        if (slice.suggestedType == CodeSegmentType::CHTL) {
            // 提取元素名称作为上下文
            std::string trimmed = slice.content;
            size_t start = trimmed.find_first_not_of(" \t\n\r");
            if (start != std::string::npos) {
                trimmed = trimmed.substr(start);
                
                // 提取第一个单词作为元素名
                size_t end = 0;
                while (end < trimmed.length() && 
                       (std::isalnum(trimmed[end]) || trimmed[end] == '_')) {
                    end++;
                }
                
                if (end > 0) {
                    segment.context = trimmed.substr(0, end);
                }
            }
        }
        
        segments.push_back(segment);
    }
    
    return segments;
}

CodeSegmentType SliceScanner::detectCodeType(const std::string& content) {
    // 检查CHTL JS特征
    if (CHTLJSFeatureDetector::hasAnyCHTLJSFeature(content)) {
        return CodeSegmentType::CHTL_JS;
    }
    
    // 检查CHTL语法
    if (isCHTLSyntaxStart(content)) {
        return CodeSegmentType::CHTL;
    }
    
    // 检查CSS语法
    if (isCSSSyntax(content)) {
        return CodeSegmentType::CSS;
    }
    
    // 检查JavaScript语法
    if (isJavaScriptSyntax(content)) {
        return CodeSegmentType::JAVASCRIPT;
    }
    
    return CodeSegmentType::UNKNOWN;
}

bool SliceScanner::isCHTLSyntaxStart(const std::string& content) {
    // 移除前导空白
    std::string trimmed = content;
    size_t start = trimmed.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return false;
    }
    trimmed = trimmed.substr(start);
    
    // 检查CHTL关键字
    std::vector<std::string> keywords = {
        "text", "style", "script", 
        "[Template]", "[Custom]", "[Origin]", 
        "[Import]", "[Configuration]", "[Namespace]"
    };
    
    for (const auto& keyword : keywords) {
        if (trimmed.find(keyword) == 0) {
            // 检查关键字后面是否有空白或 {
            size_t nextPos = keyword.length();
            if (nextPos >= trimmed.length() || 
                std::isspace(trimmed[nextPos]) || 
                trimmed[nextPos] == '{') {
                return true;
            }
        }
    }
    
    // 检查HTML元素 (identifier followed by {)
    size_t pos = 0;
    while (pos < trimmed.length() && (std::isalnum(trimmed[pos]) || trimmed[pos] == '_')) {
        pos++;
    }
    
    if (pos > 0) {
        // 跳过空白
        while (pos < trimmed.length() && std::isspace(trimmed[pos])) {
            pos++;
        }
        
        if (pos < trimmed.length() && trimmed[pos] == '{') {
            return true;
        }
    }
    
    return false;
}

bool SliceScanner::isCHTLJSSyntax(const std::string& content) {
    return CHTLJSFeatureDetector::hasAnyCHTLJSFeature(content);
}

bool SliceScanner::isCSSSyntax(const std::string& content) {
    // 使用简单的字符串查找而不是正则表达式
    
    // 检查CSS选择器模式
    if (content.find(".") != std::string::npos && content.find("{") != std::string::npos) {
        return true;
    }
    if (content.find("#") != std::string::npos && content.find("{") != std::string::npos) {
        return true;
    }
    
    // 检查CSS属性模式 property: value;
    size_t colonPos = content.find(":");
    if (colonPos != std::string::npos) {
        size_t semicolonPos = content.find(";", colonPos);
        if (semicolonPos != std::string::npos) {
            return true;
        }
    }
    
    // 检查CSS @规则
    if (content.find("@media") != std::string::npos ||
        content.find("@import") != std::string::npos ||
        content.find("@keyframes") != std::string::npos) {
        return true;
    }
    
    return false;
}

bool SliceScanner::isJavaScriptSyntax(const std::string& content) {
    // 使用简单的字符串查找
    
    // 检查函数声明
    if (content.find("function ") != std::string::npos) {
        return true;
    }
    
    // 检查变量声明
    if (content.find("var ") != std::string::npos ||
        content.find("let ") != std::string::npos ||
        content.find("const ") != std::string::npos) {
        return true;
    }
    
    // 检查控制流语句
    if (content.find("if (") != std::string::npos ||
        content.find("for (") != std::string::npos ||
        content.find("while (") != std::string::npos) {
        return true;
    }
    
    // 检查return语句
    if (content.find("return ") != std::string::npos) {
        return true;
    }
    
    // 检查箭头函数
    if (content.find("=>") != std::string::npos) {
        return true;
    }
    
    // 检查console.log等常见JS方法
    if (content.find("console.") != std::string::npos ||
        content.find("document.") != std::string::npos ||
        content.find("window.") != std::string::npos) {
        return true;
    }
    
    return false;
}

std::vector<CHTLToken> SliceScanner::tokenizeCHTLJS(const std::string& content) {
    std::vector<CHTLToken> tokens;
    
    // 使用更简单的字符串匹配而不是复杂的正则表达式
    size_t pos = 0;
    while (pos < content.length()) {
        bool matched = false;
        
        // 检查CHTL选择器 {{...}}
        if (pos + 1 < content.length() && content[pos] == '{' && content[pos + 1] == '{') {
            size_t end = content.find("}}", pos + 2);
            if (end != std::string::npos) {
                end += 2; // 包含 }}
                std::string tokenValue = content.substr(pos, end - pos);
                tokens.emplace_back(CHTLToken::CHTL_SELECTOR, tokenValue, pos, end);
                pos = end;
                matched = true;
            }
        }
        // 检查箭头操作符 ->
        else if (pos + 1 < content.length() && content[pos] == '-' && content[pos + 1] == '>') {
            tokens.emplace_back(CHTLToken::CHTL_ARROW, "->", pos, pos + 2);
            pos += 2;
            matched = true;
        }
        // 检查关键字
        else if (std::isalpha(content[pos])) {
            size_t start = pos;
            while (pos < content.length() && (std::isalnum(content[pos]) || content[pos] == '_')) {
                pos++;
            }
            std::string word = content.substr(start, pos - start);
            
            CHTLToken::Type tokenType = CHTLToken::JS_IDENTIFIER;
            if (word == "listen") {
                tokenType = CHTLToken::CHTL_LISTEN;
            } else if (word == "delegate") {
                tokenType = CHTLToken::CHTL_DELEGATE;
            } else if (word == "animate") {
                tokenType = CHTLToken::CHTL_ANIMATE;
            }
            
            tokens.emplace_back(tokenType, word, start, pos);
            matched = true;
        }
        
        if (!matched) {
            pos++;
        }
    }
    
    return tokens;
}

Position SliceScanner::calculatePosition(size_t offset) {
    size_t line = 1;
    size_t column = 1;
    
    for (size_t i = 0; i < offset && i < source_.length(); ++i) {
        if (source_[i] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }
    
    return Position{line, column, offset};
}

void SliceScanner::preprocessSource() {
    // 移除单行注释和多行注释，但保留 -- 注释
    std::string processed;
    processed.reserve(source_.size());
    
    size_t i = 0;
    while (i < source_.size()) {
        if (i + 1 < source_.size()) {
            // 处理单行注释 //
            if (source_[i] == '/' && source_[i + 1] == '/') {
                // 跳过直到行尾
                while (i < source_.size() && source_[i] != '\n') {
                    i++;
                }
                if (i < source_.size()) {
                    processed += '\n';
                    i++;
                }
                continue;
            }
            
            // 处理多行注释 /* */
            if (source_[i] == '/' && source_[i + 1] == '*') {
                i += 2;
                while (i + 1 < source_.size()) {
                    if (source_[i] == '*' && source_[i + 1] == '/') {
                        i += 2;
                        break;
                    }
                    if (source_[i] == '\n') {
                        processed += '\n';
                    }
                    i++;
                }
                continue;
            }
        }
        
        processed += source_[i];
        i++;
    }
    
    source_ = processed;
}

void SliceScanner::buildLinePositions() {
    linePositions_.clear();
    linePositions_.push_back(Position{1, 1, 0});
    
    for (size_t i = 0; i < source_.length(); ++i) {
        if (source_[i] == '\n') {
            linePositions_.push_back(Position{linePositions_.size() + 1, 1, i + 1});
        }
    }
}

size_t SliceScanner::skipWhitespace(const std::string& content, size_t pos) {
    while (pos < content.length() && isWhitespace(content[pos])) {
        pos++;
    }
    return pos;
}

bool SliceScanner::isIdentifierChar(char ch) {
    return std::isalnum(ch) || ch == '_' || ch == '$';
}

bool SliceScanner::isWhitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

// CHTLJSFeatureDetector 实现
bool CHTLJSFeatureDetector::hasCHTLSelector(const std::string& content) {
    return content.find("{{") != std::string::npos && content.find("}}") != std::string::npos;
}

bool CHTLJSFeatureDetector::hasCHTLArrow(const std::string& content) {
    return content.find("->") != std::string::npos;
}

bool CHTLJSFeatureDetector::hasCHTLListen(const std::string& content) {
    size_t pos = content.find("listen");
    if (pos != std::string::npos) {
        // 检查前面是否为单词边界
        if (pos == 0 || !std::isalnum(content[pos - 1])) {
            // 检查后面是否有 (
            size_t afterPos = pos + 6; // "listen".length()
            while (afterPos < content.length() && std::isspace(content[afterPos])) {
                afterPos++;
            }
            if (afterPos < content.length() && content[afterPos] == '(') {
                return true;
            }
        }
    }
    return false;
}

bool CHTLJSFeatureDetector::hasCHTLDelegate(const std::string& content) {
    size_t pos = content.find("delegate");
    if (pos != std::string::npos) {
        // 检查前面是否为单词边界
        if (pos == 0 || !std::isalnum(content[pos - 1])) {
            // 检查后面是否有 (
            size_t afterPos = pos + 8; // "delegate".length()
            while (afterPos < content.length() && std::isspace(content[afterPos])) {
                afterPos++;
            }
            if (afterPos < content.length() && content[afterPos] == '(') {
                return true;
            }
        }
    }
    return false;
}

bool CHTLJSFeatureDetector::hasCHTLAnimate(const std::string& content) {
    size_t pos = content.find("animate");
    if (pos != std::string::npos) {
        // 检查前面是否为单词边界
        if (pos == 0 || !std::isalnum(content[pos - 1])) {
            // 检查后面是否有 (
            size_t afterPos = pos + 7; // "animate".length()
            while (afterPos < content.length() && std::isspace(content[afterPos])) {
                afterPos++;
            }
            if (afterPos < content.length() && content[afterPos] == '(') {
                return true;
            }
        }
    }
    return false;
}

bool CHTLJSFeatureDetector::hasAnyCHTLJSFeature(const std::string& content) {
    return hasCHTLSelector(content) || hasCHTLArrow(content) || 
           hasCHTLListen(content) || hasCHTLDelegate(content) || 
           hasCHTLAnimate(content);
}

} // namespace chtl