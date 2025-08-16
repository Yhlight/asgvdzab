#include "scanner/CHTLUnifiedScanner.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <iostream>

namespace chtl {

// 私有实现类
class CHTLUnifiedScanner::Impl {
public:
    std::vector<std::string> errors;
    std::unordered_map<std::string, std::string> options;
    ScanStats stats;
    
    // 正则表达式模式
    std::regex chtlPattern;
    std::regex chtlJsPattern;
    std::regex cssPattern;
    std::regex jsPattern;
    std::regex commentPattern;
    
    Impl() {
        // 初始化正则表达式模式
        chtlPattern = std::regex(R"((?:^|\s)(?:html|head|body|div|span|p|h[1-6]|ul|ol|li|table|tr|td|th|form|input|button|select|textarea|a|img|br|hr|meta|title|link|script|style|template|custom|origin|configuration|import|namespace)(?:\s|$))", std::regex::icase);
        chtlJsPattern = std::regex(R"((?:^|\s)(?:script|listen|delegate|animate)(?:\s|$))", std::regex::icase);
        cssPattern = std::regex(R"((?:^|\s)(?:color|background|width|height|margin|padding|border|font|display|position|top|left|right|bottom|z-index|opacity|transform|transition|animation)(?:\s|$))", std::regex::icase);
        jsPattern = std::regex(R"((?:^|\s)(?:function|var|let|const|if|else|for|while|switch|case|break|continue|return|new|class|extends|super|this|console|document|window|addEventListener|querySelector|getElementById)(?:\s|$))", std::regex::icase);
        commentPattern = std::regex(R"(//.*$|/\*[\s\S]*?\*/|--.*$)", std::regex::multiline);
    }
    
    ~Impl() = default;
};

CHTLUnifiedScanner::CHTLUnifiedScanner() : pImpl(std::make_unique<Impl>()) {}

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
    std::vector<CodeFragment> fragments;
    pImpl->stats = ScanStats{};
    
    std::string cleanSource = sourceCode;
    
    // 移除注释
    cleanSource = std::regex_replace(cleanSource, pImpl->commentPattern, "");
    
    std::istringstream stream(cleanSource);
    std::string line;
    size_t lineNumber = 0;
    size_t currentFragmentStart = 0;
    FragmentType currentType = FragmentType::CHTL;
    std::string currentContent;
    
    while (std::getline(stream, line)) {
        lineNumber++;
        pImpl->stats.totalLines++;
        
        // 检测行类型
        FragmentType lineType = detectFragmentType(line, lineNumber, 0);
        
        // 如果类型改变，保存当前片段
        if (lineType != currentType && !currentContent.empty()) {
            if (isValidFragment({currentType, currentContent, 0, 0, 0, 0})) {
                fragments.emplace_back(currentType, currentContent, 
                                     currentFragmentStart, lineNumber - 1, 0, 0);
                updateStats(currentType);
            }
            currentContent.clear();
        }
        
        // 开始新片段
        if (currentContent.empty()) {
            currentFragmentStart = lineNumber;
            currentType = lineType;
        }
        
        currentContent += line + "\n";
    }
    
    // 保存最后一个片段
    if (!currentContent.empty() && isValidFragment({currentType, currentContent, 0, 0, 0, 0})) {
        fragments.emplace_back(currentType, currentContent, 
                             currentFragmentStart, lineNumber, 0, 0);
        updateStats(currentType);
    }
    
    pImpl->stats.totalFragments = fragments.size();
    
    return fragments;
}

CHTLUnifiedScanner::ScanStats CHTLUnifiedScanner::getScanStats() const {
    return pImpl->stats;
}

void CHTLUnifiedScanner::setOptions(const std::unordered_map<std::string, std::string>& options) {
    pImpl->options = options;
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

bool CHTLUnifiedScanner::isValidFragment(const CodeFragment& fragment) {
    return !fragment.content.empty() && fragment.content.length() > 0;
}

FragmentType CHTLUnifiedScanner::detectFragmentType(const std::string& content, size_t line, size_t column) {
    // 检测CHTL JS语法
    if (std::regex_search(content, pImpl->chtlJsPattern)) {
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

std::string CHTLUnifiedScanner::extractFragmentContent(const std::string& source, size_t start, size_t end) {
    if (start >= source.length() || end > source.length() || start >= end) {
        return "";
    }
    return source.substr(start, end - start);
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