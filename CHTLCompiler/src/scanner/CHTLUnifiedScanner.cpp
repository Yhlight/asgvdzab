#include "scanner/CHTLUnifiedScanner.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <iostream>
#include <stack>
#include <cctype>

namespace chtl {

// 私有实现类
class CHTLUnifiedScanner::Impl {
public:
    ScannerConfig config;
    std::vector<std::string> errors;
    ScanStats stats;
    
    // 正则表达式模式
    std::regex chtlPattern;
    std::regex chtlJsPattern;
    std::regex cssPattern;
    std::regex jsPattern;
    std::regex commentPattern;
    
    // CHTL和CHTL JS的语法模式
    std::regex chtlElementPattern;
    std::regex chtlJsSelectorPattern;
    std::regex chtlJsMethodPattern;
    
    Impl(const ScannerConfig& cfg) : config(cfg) {
        stats = ScanStats{};
        initializePatterns();
    }
    
    ~Impl() = default;
    
    void initializePatterns() {
        // 基本类型检测模式
        chtlPattern = std::regex(R"((?:^|\s)(?:html|head|body|div|span|p|h[1-6]|ul|ol|li|table|tr|td|th|form|input|button|select|textarea|a|img|br|hr|meta|title|link|script|style|template|custom|origin|configuration|import|namespace)(?:\s|$))", std::regex::icase);
        chtlJsPattern = std::regex(R"((?:^|\s)(?:script|listen|delegate|animate)(?:\s|$))", std::regex::icase);
        cssPattern = std::regex(R"((?:^|\s)(?:color|background|width|height|margin|padding|border|font|display|position|top|left|right|bottom|z-index|opacity|transform|transition|animation)(?:\s|$))", std::regex::icase);
        jsPattern = std::regex(R"((?:^|\s)(?:function|var|let|const|if|else|for|while|switch|case|break|continue|return|new|class|extends|super|this|console|document|window|addEventListener|querySelector|getElementById)(?:\s|$))", std::regex::icase);
        commentPattern = std::regex(R"(//.*$|/\*[\s\S]*?\*/|--.*$)", std::regex::multiline);
        
        // CHTL元素模式
        chtlElementPattern = std::regex(R"((?:^|\s)([a-zA-Z][a-zA-Z0-9]*)(?:\s|$))");
        
        // CHTL JS选择器和方法模式
        chtlJsSelectorPattern = std::regex(R"(\{\{[^}]+\}\})");
        chtlJsMethodPattern = std::regex(R"(\{\{[^}]+\}\}->[a-zA-Z_][a-zA-Z0-9_]*)");
    }
};

CHTLUnifiedScanner::CHTLUnifiedScanner(const ScannerConfig& config) 
    : pImpl(std::make_unique<Impl>(config)) {}

CHTLUnifiedScanner::~CHTLUnifiedScanner() = default;

std::vector<CodeFragment> CHTLUnifiedScanner::scanFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        addError("无法打开文件: " + filePath);
        return {};
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return scanString(buffer.str());
}

std::vector<CodeFragment> CHTLUnifiedScanner::scanString(const std::string& sourceCode) {
    pImpl->stats = ScanStats{};
    
    // 移除注释
    std::string cleanSource = std::regex_replace(sourceCode, pImpl->commentPattern, "");
    
    // 计算总行数
    pImpl->stats.totalLines = std::count(cleanSource.begin(), cleanSource.end(), '\n') + 1;
    
    // 执行智能扫描
    return performSmartScanning(cleanSource);
}

std::vector<CodeFragment> CHTLUnifiedScanner::performSmartScanning(const std::string& sourceCode) {
    std::vector<CodeFragment> fragments;
    size_t currentPos = 0;
    size_t lineNumber = 1;
    size_t columnNumber = 1;
    
    while (currentPos < sourceCode.length()) {
        // 创建初始切片
        size_t sliceEnd = std::min(currentPos + pImpl->config.initialSliceSize, sourceCode.length());
        std::string slice = sourceCode.substr(currentPos, sliceEnd - currentPos);
        
        // 检测片段类型
        FragmentType detectedType = detectFragmentType(slice, lineNumber, columnNumber);
        
        // 创建初始片段
        CodeFragment fragment(detectedType, slice, lineNumber, lineNumber, columnNumber, columnNumber);
        
        // 检查是否需要扩增
        if (needsFragmentExpansion(slice, detectedType)) {
            auto expandedFragments = expandFragmentIfNeeded(sourceCode, fragment, currentPos);
            fragments.insert(fragments.end(), expandedFragments.begin(), expandedFragments.end());
        } else {
            // 检查片段是否完整
            if (isFragmentComplete(slice, detectedType)) {
                fragment.isComplete = true;
                
                // 如果启用智能切割，进行二次切割
                if (pImpl->config.enableSmartSplitting) {
                    auto tokens = splitFragmentIntoTokens(fragment);
                    fragments.insert(fragments.end(), tokens.begin(), tokens.end());
                    pImpl->stats.totalSplits += tokens.size();
                } else {
                    fragments.push_back(fragment);
                }
            } else {
                // 片段不完整，尝试扩增
                fragment.needsExpansion = true;
                auto expandedFragments = expandFragmentIfNeeded(sourceCode, fragment, currentPos);
                fragments.insert(fragments.end(), expandedFragments.begin(), expandedFragments.end());
            }
        }
        
        // 更新位置信息
        size_t linesInSlice = std::count(slice.begin(), slice.end(), '\n');
        lineNumber += linesInSlice;
        if (linesInSlice > 0) {
            size_t lastNewline = slice.find_last_of('\n');
            columnNumber = slice.length() - lastNewline;
        } else {
            columnNumber += slice.length();
        }
        
        currentPos = sliceEnd;
    }
    
    pImpl->stats.totalFragments = fragments.size();
    
    return fragments;
}

std::vector<CodeFragment> CHTLUnifiedScanner::expandFragmentIfNeeded(const std::string& source, 
                                                                   CodeFragment& fragment, 
                                                                   size_t& currentPos) {
    std::vector<CodeFragment> expandedFragments;
    size_t expansionCount = 0;
    
    while (fragment.needsExpansion && 
           fragment.content.length() < pImpl->config.maxSliceSize &&
           currentPos + fragment.content.length() < source.length()) {
        
        // 向前扩增
        size_t newEnd = std::min(currentPos + fragment.content.length() + pImpl->config.expansionStep, source.length());
        fragment.content = source.substr(currentPos, newEnd - currentPos);
        
        // 检查扩增后的片段是否完整
        if (isFragmentComplete(fragment.content, fragment.type)) {
            fragment.isComplete = true;
            fragment.needsExpansion = false;
            break;
        }
        
        expansionCount++;
        if (expansionCount > 10) { // 防止无限扩增
            addError("片段扩增次数过多，可能存在语法错误");
            break;
        }
    }
    
    if (expansionCount > 0) {
        pImpl->stats.totalExpansions += expansionCount;
    }
    
    // 如果启用智能切割，进行二次切割
    if (pImpl->config.enableSmartSplitting && fragment.isComplete) {
        auto tokens = splitFragmentIntoTokens(fragment);
        expandedFragments.insert(expandedFragments.end(), tokens.begin(), tokens.end());
    } else {
        expandedFragments.push_back(fragment);
    }
    
    return expandedFragments;
}

FragmentType CHTLUnifiedScanner::detectFragmentType(const std::string& content, size_t line, size_t column) {
    // 检测CHTL JS语法（优先级最高）
    if (std::regex_search(content, pImpl->chtlJsPattern) || 
        std::regex_search(content, pImpl->chtlJsSelectorPattern)) {
        return FragmentType::CHTL_JS;
    }
    
    // 检测CSS语法
    if (std::regex_search(content, pImpl->cssPattern)) {
        return FragmentType::CSS;
    }
    
    // 检测JavaScript语法
    if (std::regex_search(content, pImpl->jsPattern)) {
        return FragmentType::JAVASCRIPT;
    }
    
    // 检测CHTL语法
    if (std::regex_search(content, pImpl->chtlPattern)) {
        return FragmentType::CHTL;
    }
    
    // 默认为CHTL
    return FragmentType::CHTL;
}

bool CHTLUnifiedScanner::isFragmentComplete(const std::string& content, FragmentType type) {
    switch (type) {
        case FragmentType::CHTL:
            return isCHTLFragmentComplete(content);
        case FragmentType::CHTL_JS:
            return isCHTLJSFragmentComplete(content);
        case FragmentType::CSS:
            return isCSSFragmentComplete(content);
        case FragmentType::JAVASCRIPT:
            return isJavaScriptFragmentComplete(content);
        default:
            return true;
    }
}

bool CHTLUnifiedScanner::isCHTLFragmentComplete(const std::string& content) {
    // 检查括号是否平衡
    if (!isBalancedBrackets(content, 0, content.length())) {
        return false;
    }
    
    // 检查是否以完整的元素结束
    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.find('{') != std::string::npos) {
            // 检查是否有对应的结束括号
            size_t openCount = std::count(line.begin(), line.end(), '{');
            size_t closeCount = std::count(line.begin(), line.end(), '}');
            if (openCount != closeCount) {
                return false;
            }
        }
    }
    
    return true;
}

bool CHTLUnifiedScanner::isCHTLJSFragmentComplete(const std::string& content) {
    // 检查CHTL JS语法是否完整
    if (std::regex_search(content, pImpl->chtlJsSelectorPattern)) {
        // 检查选择器是否完整
        std::smatch match;
        std::string::const_iterator searchStart(content.cbegin());
        while (std::regex_search(searchStart, content.cend(), match, pImpl->chtlJsSelectorPattern)) {
            std::string selector = match.str();
            if (selector.length() < 4 || selector.find("}}") == std::string::npos) {
                return false;
            }
            searchStart = match.suffix().first;
        }
    }
    
    // 检查语句是否完整
    return isCompleteStatement(content, 0, content.length());
}

bool CHTLUnifiedScanner::isCSSFragmentComplete(const std::string& content) {
    // 检查CSS语法是否完整
    if (!isBalancedBrackets(content, 0, content.length())) {
        return false;
    }
    
    // 检查选择器和属性是否完整
    std::istringstream stream(content);
    std::string line;
    bool inRule = false;
    
    while (std::getline(stream, line)) {
        if (line.find('{') != std::string::npos) {
            inRule = true;
        }
        if (line.find('}') != std::string::npos) {
            inRule = false;
        }
        if (inRule && line.find(':') != std::string::npos && line.find(';') == std::string::npos) {
            return false; // 属性不完整
        }
    }
    
    return !inRule; // 所有规则都应该闭合
}

bool CHTLUnifiedScanner::isJavaScriptFragmentComplete(const std::string& content) {
    // 检查JavaScript语法是否完整
    if (!isBalancedBrackets(content, 0, content.length())) {
        return false;
    }
    
    // 检查语句是否完整
    return isCompleteStatement(content, 0, content.length());
}

bool CHTLUnifiedScanner::needsFragmentExpansion(const std::string& content, FragmentType type) {
    // 检查片段是否需要扩增
    if (content.length() < pImpl->config.minFragmentSize) {
        return true;
    }
    
    // 检查语法是否完整
    if (!isFragmentComplete(content, type)) {
        return true;
    }
    
    // 检查是否在合适的位置截断
    size_t lastSafePoint = findPreviousSafeCutPoint(content, content.length());
    if (lastSafePoint < content.length() * 0.8) { // 如果安全截断点太靠前，需要扩增
        return true;
    }
    
    return false;
}

std::vector<CodeFragment> CHTLUnifiedScanner::splitFragmentIntoTokens(const CodeFragment& fragment) {
    std::vector<CodeFragment> tokens;
    
    if (fragment.type == FragmentType::CHTL_JS) {
        // 对CHTL JS进行智能切割
        tokens = splitCHTLJSIntoTokens(fragment);
    } else if (fragment.type == FragmentType::CHTL) {
        // 对CHTL进行智能切割
        tokens = splitCHTLIntoTokens(fragment);
    } else {
        // 其他类型保持原样
        tokens.push_back(fragment);
    }
    
    return tokens;
}

std::vector<CodeFragment> CHTLUnifiedScanner::splitCHTLJSIntoTokens(const CodeFragment& fragment) {
    std::vector<CodeFragment> tokens;
    std::string content = fragment.content;
    size_t pos = 0;
    
    while (pos < content.length()) {
        // 查找下一个token边界
        size_t tokenEnd = findNextCHTLJSTokenBoundary(content, pos);
        if (tokenEnd == std::string::npos) {
            tokenEnd = content.length();
        }
        
        // 提取token
        std::string token = content.substr(pos, tokenEnd - pos);
        if (!token.empty()) {
            CodeFragment tokenFragment(fragment.type, token, 
                                    fragment.startLine, fragment.endLine,
                                    fragment.startColumn + pos, fragment.startColumn + tokenEnd);
            tokenFragment.isComplete = true;
            tokens.push_back(tokenFragment);
        }
        
        pos = tokenEnd;
    }
    
    return tokens;
}

std::vector<CodeFragment> CHTLUnifiedScanner::splitCHTLIntoTokens(const CodeFragment& fragment) {
    std::vector<CodeFragment> tokens;
    std::string content = fragment.content;
    size_t pos = 0;
    
    while (pos < content.length()) {
        // 查找下一个token边界
        size_t tokenEnd = findNextCHTLTokenBoundary(content, pos);
        if (tokenEnd == std::string::npos) {
            tokenEnd = content.length();
        }
        
        // 提取token
        std::string token = content.substr(pos, tokenEnd - pos);
        if (!token.empty()) {
            CodeFragment tokenFragment(fragment.type, token, 
                                    fragment.startLine, fragment.endLine,
                                    fragment.startColumn + pos, fragment.startColumn + tokenEnd);
            tokenFragment.isComplete = true;
            tokens.push_back(tokenFragment);
        }
        
        pos = tokenEnd;
    }
    
    return tokens;
}

size_t CHTLUnifiedScanner::findNextCHTLJSTokenBoundary(const std::string& content, size_t pos) {
    // 查找CHTL JS的token边界
    // 例如：{{box}}->click 应该在 {{box}}-> 和 click 之间分割
    
    // 跳过空白字符
    while (pos < content.length() && std::isspace(content[pos])) {
        pos++;
    }
    
    if (pos >= content.length()) {
        return std::string::npos;
    }
    
    // 检查是否是增强选择器
    if (content[pos] == '{' && pos + 1 < content.length() && content[pos + 1] == '{') {
        // 查找选择器结束
        size_t endPos = content.find("}}", pos);
        if (endPos != std::string::npos) {
            endPos += 2; // 包含 }}
            
            // 检查是否有方法调用
            if (endPos < content.length() && content[endPos] == '-' && 
                endPos + 1 < content.length() && content[endPos + 1] == '>') {
                endPos += 2; // 包含 ->
                
                // 查找方法名结束
                while (endPos < content.length() && 
                       (std::isalnum(content[endPos]) || content[endPos] == '_')) {
                    endPos++;
                }
            }
            
            return endPos;
        }
    }
    
    // 检查是否是其他CHTL JS语法
    if (std::isalpha(content[pos])) {
        // 查找标识符结束
        while (pos < content.length() && 
               (std::isalnum(content[pos]) || content[pos] == '_')) {
            pos++;
        }
        return pos;
    }
    
    // 其他情况，向前移动一个字符
    return pos + 1;
}

size_t CHTLUnifiedScanner::findNextCHTLTokenBoundary(const std::string& content, size_t pos) {
    // 查找CHTL的token边界
    // 例如：div { text { "Hello" } } 应该在合适的位置分割
    
    // 跳过空白字符
    while (pos < content.length() && std::isspace(content[pos])) {
        pos++;
    }
    
    if (pos >= content.length()) {
        return std::string::npos;
    }
    
    // 检查是否是元素名
    if (std::isalpha(content[pos])) {
        // 查找元素名结束
        while (pos < content.length() && 
               (std::isalnum(content[pos]) || content[pos] == '_')) {
            pos++;
        }
        
        // 检查是否有属性
        while (pos < content.length() && std::isspace(content[pos])) {
            pos++;
        }
        
        if (pos < content.length() && content[pos] == '{') {
            // 找到开始括号，需要找到对应的结束括号
            return findMatchingBrace(content, pos);
        }
        
        return pos;
    }
    
    // 其他情况，向前移动一个字符
    return pos + 1;
}

size_t CHTLUnifiedScanner::findMatchingBrace(const std::string& content, size_t startPos) {
    std::stack<char> braces;
    size_t pos = startPos;
    
    while (pos < content.length()) {
        if (content[pos] == '{') {
            braces.push('{');
        } else if (content[pos] == '}') {
            if (braces.empty()) {
                return pos + 1;
            }
            braces.pop();
            if (braces.empty()) {
                return pos + 1;
            }
        }
        pos++;
    }
    
    return content.length();
}

bool CHTLUnifiedScanner::isBalancedBrackets(const std::string& content, size_t start, size_t end) {
    std::stack<char> braces;
    
    for (size_t i = start; i < end && i < content.length(); i++) {
        if (content[i] == '{' || content[i] == '(' || content[i] == '[') {
            braces.push(content[i]);
        } else if (content[i] == '}') {
            if (braces.empty() || braces.top() != '{') return false;
            braces.pop();
        } else if (content[i] == ')') {
            if (braces.empty() || braces.top() != '(') return false;
            braces.pop();
        } else if (content[i] == ']') {
            if (braces.empty() || braces.top() != '[') return false;
            braces.pop();
        }
    }
    
    return braces.empty();
}

bool CHTLUnifiedScanner::isCompleteStatement(const std::string& content, size_t start, size_t end) {
    // 检查语句是否完整
    std::string statement = content.substr(start, end - start);
    
    // 移除空白字符
    statement.erase(std::remove_if(statement.begin(), statement.end(), ::isspace), statement.end());
    
    if (statement.empty()) return true;
    
    // 检查是否以分号、大括号等结束
    if (statement.back() == ';' || statement.back() == '}' || statement.back() == '{') {
        return true;
    }
    
    // 检查括号是否平衡
    return isBalancedBrackets(statement, 0, statement.length());
}

size_t CHTLUnifiedScanner::findNextSafeCutPoint(const std::string& content, size_t start, size_t end) {
    // 查找下一个安全的切割点
    for (size_t i = start; i < end && i < content.length(); i++) {
        if (content[i] == ';' || content[i] == '}' || content[i] == '\n') {
            return i + 1;
        }
    }
    return end;
}

size_t CHTLUnifiedScanner::findPreviousSafeCutPoint(const std::string& content, size_t start) {
    // 查找前一个安全的切割点
    for (int i = static_cast<int>(start) - 1; i >= 0; i--) {
        if (content[i] == ';' || content[i] == '{' || content[i] == '\n') {
            return static_cast<size_t>(i) + 1;
        }
    }
    return 0;
}

CHTLUnifiedScanner::ScanStats CHTLUnifiedScanner::getScanStats() const {
    return pImpl->stats;
}

void CHTLUnifiedScanner::setOptions(const std::unordered_map<std::string, std::string>& options) {
    // 设置扫描选项
    for (const auto& option : options) {
        if (option.first == "initialSliceSize") {
            pImpl->config.initialSliceSize = std::stoul(option.second);
        } else if (option.first == "maxSliceSize") {
            pImpl->config.maxSliceSize = std::stoul(option.second);
        } else if (option.first == "expansionStep") {
            pImpl->config.expansionStep = std::stoul(option.second);
        } else if (option.first == "minFragmentSize") {
            pImpl->config.minFragmentSize = std::stoul(option.second);
        } else if (option.first == "enableContextAnalysis") {
            pImpl->config.enableContextAnalysis = (option.second == "true");
        } else if (option.first == "enableSmartSplitting") {
            pImpl->config.enableSmartSplitting = (option.second == "true");
        }
    }
}

std::vector<std::string> CHTLUnifiedScanner::getErrors() const {
    return pImpl->errors;
}

void CHTLUnifiedScanner::clearErrors() {
    pImpl->errors.clear();
}

void CHTLUnifiedScanner::addError(const std::string& error) {
    pImpl->errors.push_back(error);
}

void CHTLUnifiedScanner::updateStats(FragmentType type) {
    switch (type) {
        case FragmentType::CHTL:
            pImpl->stats.chtlFragments++;
            break;
        case FragmentType::CHTL_JS:
            pImpl->stats.chtlJsFragments++;
            break;
        case FragmentType::CSS:
            pImpl->stats.cssFragments++;
            break;
        case FragmentType::JAVASCRIPT:
            pImpl->stats.javascriptFragments++;
            break;
    }
}

} // namespace chtl