#include "../../include/Scanner/UnifiedScanner.h"
#include "../../include/Core/Token.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>

namespace CHTL {
namespace Scanner {

using namespace Core;

CHTLUnifiedScanner::CHTLUnifiedScanner(const ScannerConfig& config) 
    : config_(config) {
    initializePatterns();
    reset();
}

void CHTLUnifiedScanner::initializePatterns() {
    try {
        // CHTL块模式: 匹配 [Template], [Custom], [Origin] 等
        chtlBlockPattern_ = std::regex(R"(\[(Template|Custom|Origin|Import|Namespace|Configuration)\])");
        
        // CHTL JS模式: 匹配 {{...}} 选择器
        chtlJsPattern_ = std::regex(R"(\{\{[^}]*\}\})");
        
        // CSS块模式: 匹配 style { ... }
        cssBlockPattern_ = std::regex(R"(style\s*\{)");
        
        // JS块模式: 匹配 script { ... }
        jsBlockPattern_ = std::regex(R"(script\s*\{)");
        
        // 选择器模式: 匹配 {{selector}}
        selectorPattern_ = std::regex(R"(\{\{([^}]+)\}\})");
        
        // 注释模式: 匹配 // ... 和 /* ... */ 和 -- ...
        commentPattern_ = std::regex(R"((//[^\n]*)|(/\*.*?\*/)|(--[^\n]*))");
        
    } catch (const std::exception& e) {
        addError("Failed to initialize regex patterns: " + std::string(e.what()), Position{});
    }
}

CodeFragmentList CHTLUnifiedScanner::scanSource(const String& source, const String& filename) {
    reset();
    state_.currentFragment = source;
    
    if (config_.enableDebug) {
        debugLog("开始扫描源码: " + filename + " (" + std::to_string(source.size()) + " 字节)");
    }
    
    CodeFragmentList fragments;
    size_t pos = 0;
    size_t sourceSize = source.size();
    
    while (pos < sourceSize) {
        // 检测当前位置的片段类型
        FragmentType type = detectFragmentType(source, pos);
        
        if (config_.enableDebug) {
            debugLog("位置 " + std::to_string(pos) + " 检测到片段类型: " + std::to_string(static_cast<int>(type)));
        }
        
        // 计算初始切割位置
        size_t endPos = std::min(pos + config_.maxFragmentSize, sourceSize);
        
        // 查找最佳切割位置
        size_t optimalEnd = findOptimalCutPosition(source, pos, endPos);
        
        // 检查是否需要扩展
        if (needsExpansion(source, optimalEnd)) {
            optimalEnd = expandFragment(source, pos, optimalEnd);
        }
        
        // 创建片段
        String fragmentContent = source.substr(pos, optimalEnd - pos);
        Position startPos(state_.line, state_.column, pos, filename);
        
        // 更新位置信息
        for (size_t i = pos; i < optimalEnd; ++i) {
            updatePosition(source[i]);
        }
        
        Position endPos_pos(state_.line, state_.column, optimalEnd, filename);
        
        auto fragment = createFragment(type, fragmentContent, startPos, endPos_pos);
        fragments.push_back(fragment);
        
        pos = optimalEnd;
        
        // 防止无限循环
        if (optimalEnd == pos) {
            pos++;
        }
    }
    
    // 如果启用了最小单元切割，进行二次处理
    if (config_.generateTokens) {
        CodeFragmentList refinedFragments;
        for (const auto& fragment : fragments) {
            if (fragment->type == FragmentType::CHTL || fragment->type == FragmentType::CHTL_JS) {
                auto units = splitToMinimalUnits(fragment->content, fragment->type);
                auto merged = mergeContextualFragments(units, fragment->type, 
                    Position(fragment->lineNumber, fragment->columnNumber, fragment->startPos, filename));
                refinedFragments.insert(refinedFragments.end(), merged.begin(), merged.end());
            } else {
                refinedFragments.push_back(fragment);
            }
        }
        fragments = refinedFragments;
    }
    
    if (config_.enableDebug) {
        debugLog("扫描完成，共生成 " + std::to_string(fragments.size()) + " 个片段");
    }
    
    return fragments;
}

FragmentType CHTLUnifiedScanner::detectFragmentType(const String& content, size_t startPos) const {
    if (startPos >= content.size()) {
        return FragmentType::UNKNOWN;
    }
    
    String sample = content.substr(startPos, std::min(size_t(100), content.size() - startPos));
    
    // 检测CHTL块
    if (std::regex_search(sample, chtlBlockPattern_)) {
        return FragmentType::CHTL;
    }
    
    // 检测CHTL JS选择器
    if (std::regex_search(sample, chtlJsPattern_)) {
        return FragmentType::CHTL_JS;
    }
    
    // 检测CSS块
    if (std::regex_search(sample, cssBlockPattern_)) {
        return FragmentType::CSS;
    }
    
    // 检测JS块
    if (std::regex_search(sample, jsBlockPattern_)) {
        return FragmentType::JAVASCRIPT;
    }
    
    // 检测CHTL关键字
    if (isCHTLKeyword(sample)) {
        return FragmentType::CHTL;
    }
    
    // 检测CHTL JS关键字
    if (isCHTLJSKeyword(sample)) {
        return FragmentType::CHTL_JS;
    }
    
    // 默认为HTML
    return FragmentType::HTML;
}

bool CHTLUnifiedScanner::needsExpansion(const String& content, size_t cutPos) const {
    if (cutPos >= content.size()) {
        return false;
    }
    
    // 检查是否在字符串中间
    if (cutPos > 0 && isStringDelimiter(content[cutPos - 1]) && !isStringDelimiter(content[cutPos])) {
        return true;
    }
    
    // 检查是否在注释中间
    if (cutPos > 1 && content.substr(cutPos - 2, 2) == "//") {
        return true;
    }
    
    // 检查是否在CHTL块中间
    if (isCHTLBlock(content, cutPos)) {
        return true;
    }
    
    // 检查括号平衡
    String fragment = content.substr(0, cutPos);
    if (!areBracketsBalanced(fragment)) {
        return true;
    }
    
    return false;
}

size_t CHTLUnifiedScanner::findOptimalCutPosition(const String& content, size_t startPos, size_t maxPos) const {
    size_t pos = maxPos;
    
    // 向后查找合适的切割点
    while (pos > startPos && !isValidCutPosition(content, pos, detectFragmentType(content, startPos))) {
        pos--;
    }
    
    // 如果找不到合适位置，使用原始位置
    if (pos <= startPos) {
        pos = std::min(startPos + config_.minFragmentSize, content.size());
    }
    
    return pos;
}

size_t CHTLUnifiedScanner::expandFragment(const String& source, size_t startPos, size_t currentEnd) const {
    size_t expandedEnd = currentEnd;
    size_t maxExpand = std::min(currentEnd + config_.expandSize, source.size());
    
    // 逐步扩展直到找到合适的边界
    while (expandedEnd < maxExpand && needsExpansion(source, expandedEnd)) {
        expandedEnd++;
    }
    
    return expandedEnd;
}

std::vector<String> CHTLUnifiedScanner::splitToMinimalUnits(const String& content, FragmentType type) const {
    std::vector<String> units;
    
    if (type == FragmentType::CHTL) {
        auto chtlUnits = extractCHTLUnits(content);
        for (const auto& unit : chtlUnits) {
            units.push_back(unit.content);
        }
    } else if (type == FragmentType::CHTL_JS) {
        auto chtlJsUnits = extractCHTLJSUnits(content);
        for (const auto& unit : chtlJsUnits) {
            units.push_back(unit.content);
        }
    } else {
        // 对于其他类型，不进行最小单元切割
        units.push_back(content);
    }
    
    return units;
}

CodeFragmentList CHTLUnifiedScanner::mergeContextualFragments(
    const std::vector<String>& units, 
    FragmentType type,
    const Position& startPos) const {
    
    CodeFragmentList fragments;
    
    if (units.empty()) {
        return fragments;
    }
    
    // 简单合并策略：相邻的小单元合并
    String currentMerged = units[0];
    Position currentStart = startPos;
    
    for (size_t i = 1; i < units.size(); ++i) {
        // 如果当前合并的内容还不够大，继续合并
        if (currentMerged.size() < config_.minFragmentSize && 
            currentMerged.size() + units[i].size() < config_.maxFragmentSize) {
            currentMerged += units[i];
        } else {
            // 创建片段
            Position endPos = currentStart;
            endPos.offset += currentMerged.size();
            auto fragment = createFragment(type, currentMerged, currentStart, endPos);
            fragments.push_back(fragment);
            
            // 开始新的合并
            currentMerged = units[i];
            currentStart = endPos;
        }
    }
    
    // 处理最后一个片段
    if (!currentMerged.empty()) {
        Position endPos = currentStart;
        endPos.offset += currentMerged.size();
        auto fragment = createFragment(type, currentMerged, currentStart, endPos);
        fragments.push_back(fragment);
    }
    
    return fragments;
}

// 辅助函数实现

void CHTLUnifiedScanner::reset() {
    state_ = ScanState{};
    errors_.clear();
}

void CHTLUnifiedScanner::updatePosition(char ch) {
    state_.position++;
    if (ch == '\n') {
        state_.line++;
        state_.column = 1;
    } else {
        state_.column++;
    }
}

void CHTLUnifiedScanner::addError(const String& message, const Position& pos) {
    auto error = std::make_shared<ErrorInfo>();
    error->message = message;
    error->line = pos.line;
    error->column = pos.column;
    error->file = pos.filename;
    errors_.push_back(error);
}

CodeFragmentPtr CHTLUnifiedScanner::createFragment(FragmentType type, 
                                                   const String& content,
                                                   const Position& start,
                                                   const Position& end) const {
    auto fragment = std::make_shared<CodeFragment>();
    fragment->type = type;
    fragment->content = content;
    fragment->startPos = start.offset;
    fragment->endPos = end.offset;
    fragment->lineNumber = start.line;
    fragment->columnNumber = start.column;
    return fragment;
}

bool CHTLUnifiedScanner::isWhitespace(char ch) const {
    return std::isspace(ch);
}

bool CHTLUnifiedScanner::isAlpha(char ch) const {
    return std::isalpha(ch);
}

bool CHTLUnifiedScanner::isDigit(char ch) const {
    return std::isdigit(ch);
}

bool CHTLUnifiedScanner::isAlnum(char ch) const {
    return std::isalnum(ch);
}

bool CHTLUnifiedScanner::isIdentifierStart(char ch) const {
    return isAlpha(ch) || ch == '_' || ch == '@';
}

bool CHTLUnifiedScanner::isIdentifierPart(char ch) const {
    return isAlnum(ch) || ch == '_' || ch == '-';
}

bool CHTLUnifiedScanner::isCHTLKeyword(const String& word) const {
    static const std::vector<String> keywords = {
        "text", "style", "script", "[Template]", "[Custom]", "[Origin]",
        "[Import]", "[Namespace]", "[Configuration]", "@Style", "@Element",
        "@Var", "@Html", "@JavaScript", "@Chtl", "@CJmod", "inherit",
        "delete", "insert", "from", "as", "except"
    };
    
    return std::find(keywords.begin(), keywords.end(), word) != keywords.end();
}

bool CHTLUnifiedScanner::isCHTLJSKeyword(const String& word) const {
    static const std::vector<String> keywords = {
        "vir", "listen", "animate", "delegate"
    };
    
    return std::find(keywords.begin(), keywords.end(), word) != keywords.end();
}

bool CHTLUnifiedScanner::isCHTLSelector(const String& content, size_t pos) const {
    if (pos + 1 >= content.size()) return false;
    return content.substr(pos, 2) == "{{";
}

bool CHTLUnifiedScanner::isCHTLBlock(const String& content, size_t pos) const {
    if (pos >= content.size()) return false;
    
    // 检查是否在CHTL块标记中
    size_t start = pos > 10 ? pos - 10 : 0;
    String sample = content.substr(start, pos - start + 20);
    
    return std::regex_search(sample, chtlBlockPattern_);
}

bool CHTLUnifiedScanner::isValidCutPosition(const String& content, size_t pos, FragmentType type) const {
    if (pos >= content.size()) return true;
    
    char ch = content[pos];
    
    // 在空白字符处切割是安全的
    if (isWhitespace(ch)) return true;
    
    // 在分隔符处切割是安全的
    if (ch == ';' || ch == '}' || ch == ')' || ch == ']') return true;
    
    // 检查特定类型的切割规则
    switch (type) {
        case FragmentType::CHTL:
        case FragmentType::CHTL_JS:
            // 在CHTL关键字边界切割
            return !isIdentifierPart(ch);
        default:
            return true;
    }
}

std::vector<CHTLUnifiedScanner::CHTLUnit> CHTLUnifiedScanner::extractCHTLUnits(const String& content) const {
    std::vector<CHTLUnit> units;
    size_t pos = 0;
    
    while (pos < content.size()) {
        // 跳过空白字符
        while (pos < content.size() && isWhitespace(content[pos])) {
            pos++;
        }
        
        if (pos >= content.size()) break;
        
        size_t start = pos;
        
        // 识别不同类型的单元
        if (content[pos] == '[') {
            // 块标记 [Template], [Custom] 等
            while (pos < content.size() && content[pos] != ']') {
                pos++;
            }
            if (pos < content.size()) pos++; // 包含 ]
        } else if (content[pos] == '@') {
            // @Style, @Element 等
            while (pos < content.size() && isIdentifierPart(content[pos])) {
                pos++;
            }
        } else if (isIdentifierStart(content[pos])) {
            // 标识符
            while (pos < content.size() && isIdentifierPart(content[pos])) {
                pos++;
            }
        } else {
            // 单字符
            pos++;
        }
        
        if (pos > start) {
            CHTLUnit unit;
            unit.content = content.substr(start, pos - start);
            unit.startPos = start;
            unit.endPos = pos;
            unit.type = TokenType::IDENTIFIER; // 临时设置
            units.push_back(unit);
        }
    }
    
    return units;
}

std::vector<CHTLUnifiedScanner::CHTLUnit> CHTLUnifiedScanner::extractCHTLJSUnits(const String& content) const {
    std::vector<CHTLUnit> units;
    size_t pos = 0;
    
    while (pos < content.size()) {
        // 跳过空白字符
        while (pos < content.size() && isWhitespace(content[pos])) {
            pos++;
        }
        
        if (pos >= content.size()) break;
        
        size_t start = pos;
        
        // 识别CHTL JS特殊语法
        if (pos + 1 < content.size() && content.substr(pos, 2) == "{{") {
            // 选择器 {{...}}
            pos += 2;
            while (pos + 1 < content.size() && content.substr(pos, 2) != "}}") {
                pos++;
            }
            if (pos + 1 < content.size()) pos += 2; // 包含 }}
        } else if (pos + 1 < content.size() && content.substr(pos, 2) == "->") {
            // 箭头操作符
            pos += 2;
        } else if (isIdentifierStart(content[pos])) {
            // 标识符
            while (pos < content.size() && isIdentifierPart(content[pos])) {
                pos++;
            }
        } else {
            // 单字符
            pos++;
        }
        
        if (pos > start) {
            CHTLUnit unit;
            unit.content = content.substr(start, pos - start);
            unit.startPos = start;
            unit.endPos = pos;
            unit.type = TokenType::IDENTIFIER; // 临时设置
            units.push_back(unit);
        }
    }
    
    return units;
}

bool CHTLUnifiedScanner::isStringDelimiter(char ch) const {
    return ch == '"' || ch == '\'';
}

bool CHTLUnifiedScanner::areBracketsBalanced(const String& content) const {
    int braceCount = 0;
    int parenCount = 0;
    int bracketCount = 0;
    bool inString = false;
    char stringDelim = '\0';
    
    for (size_t i = 0; i < content.size(); ++i) {
        char ch = content[i];
        
        if (inString) {
            if (ch == stringDelim && (i == 0 || content[i-1] != '\\')) {
                inString = false;
            }
            continue;
        }
        
        if (isStringDelimiter(ch)) {
            inString = true;
            stringDelim = ch;
            continue;
        }
        
        switch (ch) {
            case '{': braceCount++; break;
            case '}': braceCount--; break;
            case '(': parenCount++; break;
            case ')': parenCount--; break;
            case '[': bracketCount++; break;
            case ']': bracketCount--; break;
        }
    }
    
    return braceCount == 0 && parenCount == 0 && bracketCount == 0;
}

void CHTLUnifiedScanner::debugLog(const String& message) const {
    if (config_.enableDebug) {
        std::cout << "[Scanner] " << message << std::endl;
    }
}

// 扫描器工厂实现

std::unique_ptr<CHTLUnifiedScanner> ScannerFactory::createScanner(const ScannerConfig& config) {
    return std::make_unique<CHTLUnifiedScanner>(config);
}

ScannerConfig ScannerFactory::getDefaultConfig() {
    return ScannerConfig{};
}

ScannerConfig ScannerFactory::getDebugConfig() {
    ScannerConfig config;
    config.enableDebug = true;
    return config;
}

ScannerConfig ScannerFactory::getOptimizedConfig() {
    ScannerConfig config;
    config.maxFragmentSize = 16384;
    config.minFragmentSize = 128;
    config.expandSize = 512;
    return config;
}

} // namespace Scanner
} // namespace CHTL