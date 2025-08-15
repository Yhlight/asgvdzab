#include "scanner/unified_scanner.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace chtl {

CHTLUnifiedScanner::CHTLUnifiedScanner(const CompileConfig& config) 
    : config_(config) {
}

ScanResult CHTLUnifiedScanner::scan(const std::string& source, const std::string& filename) {
    reset();
    state_.source = source;
    state_.filename = filename;
    
    try {
        preprocessSource();
        extractSegments();
    } catch (const std::exception& e) {
        reportError("扫描过程中发生异常: " + std::string(e.what()));
        result_.success = false;
    }
    
    return result_;
}

ScanResult CHTLUnifiedScanner::scanFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        ScanResult result;
        result.success = false;
        result.errors.push_back("无法打开文件: " + filename);
        return result;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return scan(buffer.str(), filename);
}

void CHTLUnifiedScanner::setConfig(const CompileConfig& config) {
    config_ = config;
}

const CompileConfig& CHTLUnifiedScanner::getConfig() const {
    return config_;
}

void CHTLUnifiedScanner::reset() {
    state_ = ScannerState{};
    result_ = ScanResult{};
}

void CHTLUnifiedScanner::preprocessSource() {
    // 预处理：移除不被生成器识别的注释（// 和 /* */）
    std::string processed;
    processed.reserve(state_.source.size());
    
    size_t pos = 0;
    while (pos < state_.source.size()) {
        if (pos + 1 < state_.source.size()) {
            // 处理单行注释 //
            if (state_.source[pos] == '/' && state_.source[pos + 1] == '/') {
                // 跳过直到行尾
                while (pos < state_.source.size() && state_.source[pos] != '\n') {
                    pos++;
                }
                if (pos < state_.source.size()) {
                    processed += '\n'; // 保留换行符以维持行号
                    pos++;
                }
                continue;
            }
            
            // 处理多行注释 /* */
            if (state_.source[pos] == '/' && state_.source[pos + 1] == '*') {
                pos += 2;
                while (pos + 1 < state_.source.size()) {
                    if (state_.source[pos] == '*' && state_.source[pos + 1] == '/') {
                        pos += 2;
                        break;
                    }
                    if (state_.source[pos] == '\n') {
                        processed += '\n'; // 保留换行符以维持行号
                    }
                    pos++;
                }
                continue;
            }
        }
        
        processed += state_.source[pos];
        pos++;
    }
    
    state_.source = processed;
}

void CHTLUnifiedScanner::extractSegments() {
    state_.position = 0;
    state_.line = 1;
    state_.column = 1;
    
    while (!isAtEnd()) {
        char ch = peek();
        
        if (isWhitespace(ch)) {
            advance();
            continue;
        }
        
        // 检查关键字
        std::string keyword = checkKeywordAt(state_.position);
        if (!keyword.empty()) {
            Position startPos = getCurrentPosition();
            
            // 跳过关键字
            for (size_t i = 0; i < keyword.length(); ++i) {
                advance();
            }
            
            skipWhitespace();
            
            // 检查是否有参数（如 @Style DefaultText）
            std::string parameter;
            if (!isAtEnd() && peek() != '{') {
                parameter = readIdentifier();
                skipWhitespace();
            }
            
            // 期望找到 '{'
            if (isAtEnd() || peek() != '{') {
                reportError("期望找到 '{' 在关键字 '" + keyword + "' 之后", getCurrentPosition());
                advance();
                continue;
            }
            
            // 提取块内容
            Position blockStart = getCurrentPosition();
            advance(); // 跳过 '{'
            
            int braceCount = 1;
            size_t contentStart = state_.position;
            
            while (!isAtEnd() && braceCount > 0) {
                char c = advance();
                if (c == '{') {
                    braceCount++;
                } else if (c == '}') {
                    braceCount--;
                }
            }
            
            if (braceCount > 0) {
                reportError("未找到匹配的 '}' 对于关键字 '" + keyword + "'", startPos);
                continue;
            }
            
            // 提取内容（不包括最后的 '}'）
            size_t contentEnd = state_.position - 1;
            std::string content = state_.source.substr(contentStart, contentEnd - contentStart);
            
            Position endPos = getCurrentPosition();
            
            // 分析块类型并创建代码片段
            CodeSegmentType type = analyzeBlockType(keyword, content);
            CodeSegment segment = createSegment(type, content, startPos, endPos, parameter);
            
            // 添加关键字信息到属性
            segment.attributes["keyword"] = keyword;
            if (!parameter.empty()) {
                segment.attributes["parameter"] = parameter;
            }
            
            result_.segments.push_back(segment);
            continue;
        }
        
        // 检查是否为HTML元素或文本
        if (std::isalpha(ch) || ch == '_') {
            Position startPos = getCurrentPosition();
            std::string identifier = readIdentifier();
            
            skipWhitespace();
            
            if (!isAtEnd() && peek() == '{') {
                // 这是一个HTML元素
                advance(); // 跳过 '{'
                
                int braceCount = 1;
                size_t contentStart = state_.position;
                
                while (!isAtEnd() && braceCount > 0) {
                    char c = advance();
                    if (c == '{') {
                        braceCount++;
                    } else if (c == '}') {
                        braceCount--;
                    }
                }
                
                if (braceCount > 0) {
                    reportError("未找到匹配的 '}' 对于元素 '" + identifier + "'", startPos);
                    continue;
                }
                
                size_t contentEnd = state_.position - 1;
                std::string content = state_.source.substr(contentStart, contentEnd - contentStart);
                
                Position endPos = getCurrentPosition();
                
                CodeSegment segment = createSegment(CodeSegmentType::CHTL, content, startPos, endPos, identifier);
                segment.attributes["element"] = identifier;
                
                result_.segments.push_back(segment);
            } else {
                // 可能是属性或其他内容，暂时作为CHTL处理
                Position endPos = getCurrentPosition();
                CodeSegment segment = createSegment(CodeSegmentType::CHTL, identifier, startPos, endPos);
                result_.segments.push_back(segment);
            }
            continue;
        }
        
        // 处理其他字符
        advance();
    }
}

std::string CHTLUnifiedScanner::checkKeywordAt(size_t position) {
    if (position >= state_.source.size()) {
        return "";
    }
    
    // 检查各种关键字模式
    std::vector<std::string> keywords = {
        "[Template]", "[Custom]", "[Origin]", "[Import]", "[Configuration]", "[Namespace]",
        "style", "script", "text", "@Style", "@Element", "@Var", "@Html", "@JavaScript",
        "@Chtl", "@CJmod", "inherit", "delete", "insert", "except"
    };
    
    for (const auto& keyword : keywords) {
        if (position + keyword.length() <= state_.source.size()) {
            std::string substr = state_.source.substr(position, keyword.length());
            if (substr == keyword) {
                // 检查关键字后面是否为分隔符（空格、换行、{等）
                size_t nextPos = position + keyword.length();
                if (nextPos >= state_.source.size() || 
                    std::isspace(state_.source[nextPos]) || 
                    state_.source[nextPos] == '{') {
                    return keyword;
                }
            }
        }
    }
    
    return "";
}

CodeSegmentType CHTLUnifiedScanner::analyzeBlockType(const std::string& keyword, const std::string& content) {
    if (keyword == "[Template]") {
        return CodeSegmentType::TEMPLATE;
    } else if (keyword == "[Custom]") {
        return CodeSegmentType::CUSTOM;
    } else if (keyword == "[Origin]") {
        return CodeSegmentType::ORIGIN;
    } else if (keyword == "[Import]") {
        return CodeSegmentType::IMPORT;
    } else if (keyword == "[Configuration]") {
        return CodeSegmentType::CONFIGURATION;
    } else if (keyword == "[Namespace]") {
        return CodeSegmentType::NAMESPACE;
    } else if (keyword == "style") {
        return CodeSegmentType::LOCAL_STYLE;
    } else if (keyword == "script") {
        // 需要进一步分析script内容来判断是CHTL JS还是标准JS
        if (content.find("{{") != std::string::npos || content.find("->") != std::string::npos) {
            return CodeSegmentType::CHTL_JS;
        } else {
            return CodeSegmentType::JAVASCRIPT;
        }
    } else if (keyword == "text") {
        return CodeSegmentType::TEXT;
    } else {
        return CodeSegmentType::CHTL;
    }
}

CodeSegment CHTLUnifiedScanner::createSegment(CodeSegmentType type, const std::string& content,
                                             const Position& startPos, const Position& endPos,
                                             const std::string& context) {
    CodeSegment segment(type, content, Range{startPos, endPos});
    segment.context = context;
    return segment;
}

void CHTLUnifiedScanner::reportError(const std::string& message, const Position& position) {
    std::string errorMsg = state_.filename.empty() ? "" : state_.filename + ":";
    errorMsg += std::to_string(position.line == 0 ? state_.line : position.line) + ":" +
                std::to_string(position.column == 0 ? state_.column : position.column) + ": " + message;
    result_.errors.push_back(errorMsg);
    result_.success = false;
}

void CHTLUnifiedScanner::reportWarning(const std::string& message, const Position& position) {
    std::string warningMsg = state_.filename.empty() ? "" : state_.filename + ":";
    warningMsg += std::to_string(position.line == 0 ? state_.line : position.line) + ":" +
                  std::to_string(position.column == 0 ? state_.column : position.column) + ": " + message;
    result_.warnings.push_back(warningMsg);
}

Position CHTLUnifiedScanner::getCurrentPosition() const {
    return Position{state_.line, state_.column, state_.position};
}

void CHTLUnifiedScanner::skipWhitespace() {
    while (!isAtEnd() && isWhitespace(peek())) {
        advance();
    }
}

bool CHTLUnifiedScanner::isWhitespace(char ch) const {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

bool CHTLUnifiedScanner::isIdentifierChar(char ch) const {
    return std::isalnum(ch) || ch == '_' || ch == '-';
}

std::string CHTLUnifiedScanner::readIdentifier() {
    std::string identifier;
    while (!isAtEnd() && isIdentifierChar(peek())) {
        identifier += advance();
    }
    return identifier;
}

bool CHTLUnifiedScanner::match(const std::string& str) {
    if (state_.position + str.length() > state_.source.size()) {
        return false;
    }
    
    for (size_t i = 0; i < str.length(); ++i) {
        if (state_.source[state_.position + i] != str[i]) {
            return false;
        }
    }
    
    // 匹配成功，移动位置
    for (size_t i = 0; i < str.length(); ++i) {
        advance();
    }
    
    return true;
}

char CHTLUnifiedScanner::peek(size_t offset) const {
    size_t pos = state_.position + offset;
    if (pos >= state_.source.size()) {
        return '\0';
    }
    return state_.source[pos];
}

char CHTLUnifiedScanner::advance() {
    if (isAtEnd()) {
        return '\0';
    }
    
    char ch = state_.source[state_.position];
    state_.position++;
    
    if (ch == '\n') {
        state_.line++;
        state_.column = 1;
    } else {
        state_.column++;
    }
    
    return ch;
}

bool CHTLUnifiedScanner::isAtEnd() const {
    return state_.position >= state_.source.size();
}

} // namespace chtl