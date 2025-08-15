#include "scanner/chtl_unified_scanner.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stack>
#include <regex>
#include <iostream>

namespace chtl {

// Scanner实现类
class CHTLUnifiedScanner::Impl {
public:
    explicit Impl(const ScannerConfig& config) 
        : config_(config), currentLine_(1), currentColumn_(1) {}
    
    std::vector<CodeFragment> scan(const std::string& sourceCode, 
                                   const std::string& sourcePath) {
        reset();
        sourceCode_ = sourceCode;
        sourcePath_ = sourcePath;
        
        try {
            return performScan();
        } catch (const std::exception& e) {
            lastError_ = std::string("扫描错误: ") + e.what();
            return {};
        }
    }
    
    std::vector<CodeFragment> scanFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            lastError_ = "无法打开文件: " + filePath;
            return {};
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return scan(buffer.str(), filePath);
    }
    
    std::string getLastError() const { return lastError_; }
    
    CHTLUnifiedScanner::ScanStats getStats() const { return stats_; }

private:
    // 扫描状态
    enum class ScanState {
        NORMAL,
        IN_STRING,
        IN_COMMENT,
        IN_STYLE,
        IN_SCRIPT,
        IN_ORIGIN,
        IN_TEMPLATE,
        IN_CUSTOM,
        IN_CONFIGURATION
    };
    
    // 重置扫描器状态
    void reset() {
        pos_ = 0;
        currentLine_ = 1;
        currentColumn_ = 1;
        lastError_.clear();
        fragments_.clear();
        stats_ = {};
        stateStack_.clear();
        stateStack_.push(ScanState::NORMAL);
    }
    
    // 执行扫描
    std::vector<CodeFragment> performScan() {
        while (pos_ < sourceCode_.length()) {
            scanNext();
        }
        
        // 处理未闭合的片段
        finalizeFragments();
        
        // 更新统计信息
        updateStats();
        
        return fragments_;
    }
    
    // 扫描下一个元素
    void scanNext() {
        skipWhitespace();
        
        if (pos_ >= sourceCode_.length()) {
            return;
        }
        
        // 检查注释
        if (checkComment()) {
            return;
        }
        
        // 检查特殊块
        if (checkSpecialBlock()) {
            return;
        }
        
        // 检查普通CHTL元素
        scanCHTLElement();
    }
    
    // 检查注释
    bool checkComment() {
        if (peek() == '/' && peek(1) == '/') {
            // 单行注释
            scanSingleLineComment();
            return true;
        } else if (peek() == '/' && peek(1) == '*') {
            // 多行注释
            scanMultiLineComment();
            return true;
        } else if (peek() == '-' && peek(1) == '-') {
            // CHTL识别注释
            scanCHTLComment();
            return true;
        }
        return false;
    }
    
    // 检查特殊块
    bool checkSpecialBlock() {
        // 检查 [Origin] 块
        if (checkKeyword("[Origin]")) {
            scanOriginBlock();
            return true;
        }
        
        // 检查 [Template] 块
        if (checkKeyword("[Template]")) {
            scanTemplateBlock();
            return true;
        }
        
        // 检查 [Custom] 块
        if (checkKeyword("[Custom]")) {
            scanCustomBlock();
            return true;
        }
        
        // 检查 [Configuration] 块
        if (checkKeyword("[Configuration]")) {
            scanConfigurationBlock();
            return true;
        }
        
        // 检查 style 块
        if (checkKeyword("style") && peek(skipWhitespaceCount()) == '{') {
            scanStyleBlock();
            return true;
        }
        
        // 检查 script 块
        if (checkKeyword("script") && peek(skipWhitespaceCount()) == '{') {
            scanScriptBlock();
            return true;
        }
        
        return false;
    }
    
    // 扫描样式块
    void scanStyleBlock() {
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        size_t startPos = pos_;
        
        // 跳过 "style"
        pos_ += 5;
        currentColumn_ += 5;
        skipWhitespace();
        
        // 检查是否是局部样式块（在元素内部）
        bool isLocalStyle = isInsideElement();
        
        // 跳过 '{'
        advance();
        
        std::string content;
        int braceCount = 1;
        
        while (pos_ < sourceCode_.length() && braceCount > 0) {
            if (peek() == '{') {
                braceCount++;
            } else if (peek() == '}') {
                braceCount--;
                if (braceCount == 0) {
                    break;
                }
            }
            content += peek();
            advance();
        }
        
        // 跳过 '}'
        if (peek() == '}') {
            advance();
        }
        
        // 创建片段
        CodeFragment fragment;
        fragment.type = isLocalStyle ? CodeFragmentType::CHTL : CodeFragmentType::CSS;
        fragment.content = content;
        fragment.startLine = startLine;
        fragment.startColumn = startColumn;
        fragment.endLine = currentLine_;
        fragment.endColumn = currentColumn_;
        fragment.sourcePath = sourcePath_;
        fragment.metadata["isLocalStyle"] = isLocalStyle ? "true" : "false";
        
        fragments_.push_back(fragment);
    }
    
    // 扫描脚本块
    void scanScriptBlock() {
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        size_t startPos = pos_;
        
        // 跳过 "script"
        pos_ += 6;
        currentColumn_ += 6;
        skipWhitespace();
        
        // 跳过 '{'
        advance();
        
        std::string content;
        int braceCount = 1;
        bool hasCHTLJSSyntax = false;
        
        while (pos_ < sourceCode_.length() && braceCount > 0) {
            // 检查CHTL JS语法
            if (peek() == '{' && peek(1) == '{') {
                hasCHTLJSSyntax = true;
            } else if (peek() == '-' && peek(1) == '>') {
                hasCHTLJSSyntax = true;
            }
            
            if (peek() == '{') {
                braceCount++;
            } else if (peek() == '}') {
                braceCount--;
                if (braceCount == 0) {
                    break;
                }
            }
            content += peek();
            advance();
        }
        
        // 跳过 '}'
        if (peek() == '}') {
            advance();
        }
        
        // 创建片段
        CodeFragment fragment;
        fragment.type = hasCHTLJSSyntax ? CodeFragmentType::CHTL_JS : CodeFragmentType::JAVASCRIPT;
        fragment.content = content;
        fragment.startLine = startLine;
        fragment.startColumn = startColumn;
        fragment.endLine = currentLine_;
        fragment.endColumn = currentColumn_;
        fragment.sourcePath = sourcePath_;
        
        fragments_.push_back(fragment);
    }
    
    // 扫描原始嵌入块
    void scanOriginBlock() {
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        
        // 跳过 "[Origin]"
        pos_ += 8;
        currentColumn_ += 8;
        skipWhitespace();
        
        // 读取类型 (@Html, @Style, @JavaScript)
        std::string originType = scanIdentifier();
        skipWhitespace();
        
        // 可能有名称
        std::string name;
        if (peek() != '{') {
            name = scanIdentifier();
            skipWhitespace();
        }
        
        // 跳过 '{'
        advance();
        
        std::string content;
        int braceCount = 1;
        
        while (pos_ < sourceCode_.length() && braceCount > 0) {
            if (peek() == '{') {
                braceCount++;
            } else if (peek() == '}') {
                braceCount--;
                if (braceCount == 0) {
                    break;
                }
            }
            content += peek();
            advance();
        }
        
        // 跳过 '}'
        if (peek() == '}') {
            advance();
        }
        
        // 创建片段
        CodeFragment fragment;
        
        if (originType == "@Html") {
            fragment.type = CodeFragmentType::HTML_ORIGIN;
        } else if (originType == "@Style") {
            fragment.type = CodeFragmentType::CSS;
        } else if (originType == "@JavaScript") {
            fragment.type = CodeFragmentType::JAVASCRIPT;
        } else {
            fragment.type = CodeFragmentType::UNKNOWN;
        }
        
        fragment.content = content;
        fragment.startLine = startLine;
        fragment.startColumn = startColumn;
        fragment.endLine = currentLine_;
        fragment.endColumn = currentColumn_;
        fragment.sourcePath = sourcePath_;
        fragment.metadata["originType"] = originType;
        if (!name.empty()) {
            fragment.metadata["name"] = name;
        }
        
        fragments_.push_back(fragment);
    }
    
    // 扫描CHTL元素
    void scanCHTLElement() {
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        size_t startPos = pos_;
        
        // 收集CHTL内容直到遇到特殊块
        std::string content;
        
        while (pos_ < sourceCode_.length()) {
            // 检查是否遇到特殊块的开始
            if (checkComment() || checkSpecialBlock()) {
                break;
            }
            
            content += peek();
            advance();
        }
        
        if (!content.empty()) {
            CodeFragment fragment;
            fragment.type = CodeFragmentType::CHTL;
            fragment.content = content;
            fragment.startLine = startLine;
            fragment.startColumn = startColumn;
            fragment.endLine = currentLine_;
            fragment.endColumn = currentColumn_;
            fragment.sourcePath = sourcePath_;
            
            fragments_.push_back(fragment);
        }
    }
    
    // 辅助函数
    char peek(size_t offset = 0) const {
        size_t index = pos_ + offset;
        return (index < sourceCode_.length()) ? sourceCode_[index] : '\0';
    }
    
    void advance() {
        if (pos_ < sourceCode_.length()) {
            if (sourceCode_[pos_] == '\n') {
                currentLine_++;
                currentColumn_ = 1;
            } else {
                currentColumn_++;
            }
            pos_++;
        }
    }
    
    void skipWhitespace() {
        while (pos_ < sourceCode_.length() && std::isspace(sourceCode_[pos_])) {
            advance();
        }
    }
    
    size_t skipWhitespaceCount() const {
        size_t count = 0;
        size_t tempPos = pos_;
        while (tempPos < sourceCode_.length() && std::isspace(sourceCode_[tempPos])) {
            tempPos++;
            count++;
        }
        return count;
    }
    
    bool checkKeyword(const std::string& keyword) const {
        if (pos_ + keyword.length() > sourceCode_.length()) {
            return false;
        }
        return sourceCode_.substr(pos_, keyword.length()) == keyword;
    }
    
    std::string scanIdentifier() {
        std::string identifier;
        while (pos_ < sourceCode_.length() && 
               (std::isalnum(sourceCode_[pos_]) || sourceCode_[pos_] == '_' || sourceCode_[pos_] == '@')) {
            identifier += sourceCode_[pos_];
            advance();
        }
        return identifier;
    }
    
    bool isInsideElement() const {
        // 简化实现：通过检查前面是否有未闭合的元素标签
        // 实际实现需要更复杂的上下文跟踪
        return stateStack_.size() > 1;
    }
    
    void scanSingleLineComment() {
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        std::string content;
        
        // 跳过 "//"
        advance();
        advance();
        
        while (pos_ < sourceCode_.length() && peek() != '\n') {
            content += peek();
            advance();
        }
        
        if (config_.preserveComments) {
            CodeFragment fragment;
            fragment.type = CodeFragmentType::COMMENT;
            fragment.content = "//" + content;
            fragment.startLine = startLine;
            fragment.startColumn = startColumn;
            fragment.endLine = currentLine_;
            fragment.endColumn = currentColumn_;
            fragment.sourcePath = sourcePath_;
            fragments_.push_back(fragment);
        }
    }
    
    void scanMultiLineComment() {
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        std::string content = "/*";
        
        // 跳过 "/*"
        advance();
        advance();
        
        while (pos_ < sourceCode_.length()) {
            if (peek() == '*' && peek(1) == '/') {
                content += "*/";
                advance();
                advance();
                break;
            }
            content += peek();
            advance();
        }
        
        if (config_.preserveComments) {
            CodeFragment fragment;
            fragment.type = CodeFragmentType::COMMENT;
            fragment.content = content;
            fragment.startLine = startLine;
            fragment.startColumn = startColumn;
            fragment.endLine = currentLine_;
            fragment.endColumn = currentColumn_;
            fragment.sourcePath = sourcePath_;
            fragments_.push_back(fragment);
        }
    }
    
    void scanCHTLComment() {
        // CHTL识别注释，需要保留
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        std::string content = "--";
        
        // 跳过 "--"
        advance();
        advance();
        
        while (pos_ < sourceCode_.length() && peek() != '\n') {
            content += peek();
            advance();
        }
        
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL;
        fragment.content = content;
        fragment.startLine = startLine;
        fragment.startColumn = startColumn;
        fragment.endLine = currentLine_;
        fragment.endColumn = currentColumn_;
        fragment.sourcePath = sourcePath_;
        fragment.metadata["isCHTLComment"] = "true";
        fragments_.push_back(fragment);
    }
    
    void scanTemplateBlock() {
        // 简化实现，完整实现需要更多细节
        scanCHTLElement();
    }
    
    void scanCustomBlock() {
        // 简化实现
        scanCHTLElement();
    }
    
    void scanConfigurationBlock() {
        // 简化实现
        scanCHTLElement();
    }
    
    void finalizeFragments() {
        // 处理未完成的片段
        // 合并相邻的相同类型片段（如果需要）
    }
    
    void updateStats() {
        stats_.totalFragments = fragments_.size();
        stats_.totalLines = currentLine_;
        
        for (const auto& fragment : fragments_) {
            switch (fragment.type) {
                case CodeFragmentType::CHTL:
                    stats_.chtlFragments++;
                    break;
                case CodeFragmentType::CHTL_JS:
                    stats_.chtlJsFragments++;
                    break;
                case CodeFragmentType::CSS:
                    stats_.cssFragments++;
                    break;
                case CodeFragmentType::JAVASCRIPT:
                    stats_.jsFragments++;
                    break;
                case CodeFragmentType::HTML_ORIGIN:
                    stats_.htmlFragments++;
                    break;
                case CodeFragmentType::COMMENT:
                    stats_.commentFragments++;
                    break;
                default:
                    break;
            }
        }
    }
    
private:
    ScannerConfig config_;
    std::string sourceCode_;
    std::string sourcePath_;
    std::string lastError_;
    
    size_t pos_;
    size_t currentLine_;
    size_t currentColumn_;
    
    std::vector<CodeFragment> fragments_;
    CHTLUnifiedScanner::ScanStats stats_;
    std::stack<ScanState> stateStack_;
};

// CHTLUnifiedScanner 实现
CHTLUnifiedScanner::CHTLUnifiedScanner(const ScannerConfig& config)
    : pImpl(std::make_unique<Impl>(config)) {}

CHTLUnifiedScanner::~CHTLUnifiedScanner() = default;

std::vector<CodeFragment> CHTLUnifiedScanner::scan(const std::string& sourceCode, 
                                                   const std::string& sourcePath) {
    return pImpl->scan(sourceCode, sourcePath);
}

std::vector<CodeFragment> CHTLUnifiedScanner::scanFile(const std::string& filePath) {
    return pImpl->scanFile(filePath);
}

std::string CHTLUnifiedScanner::getLastError() const {
    return pImpl->getLastError();
}

CHTLUnifiedScanner::ScanStats CHTLUnifiedScanner::getStats() const {
    return pImpl->getStats();
}

// 辅助函数实现
std::string fragmentTypeToString(CodeFragmentType type) {
    switch (type) {
        case CodeFragmentType::CHTL: return "CHTL";
        case CodeFragmentType::CHTL_JS: return "CHTL_JS";
        case CodeFragmentType::CSS: return "CSS";
        case CodeFragmentType::JAVASCRIPT: return "JavaScript";
        case CodeFragmentType::HTML_ORIGIN: return "HTML_Origin";
        case CodeFragmentType::COMMENT: return "Comment";
        default: return "Unknown";
    }
}

void printFragment(const CodeFragment& fragment, bool verbose) {
    std::cout << "片段类型: " << fragmentTypeToString(fragment.type) << "\n";
    std::cout << "位置: " << fragment.sourcePath 
              << " [" << fragment.startLine << ":" << fragment.startColumn 
              << " - " << fragment.endLine << ":" << fragment.endColumn << "]\n";
    
    if (verbose) {
        std::cout << "内容:\n" << fragment.content << "\n";
        
        if (!fragment.metadata.empty()) {
            std::cout << "元数据:\n";
            for (const auto& [key, value] : fragment.metadata) {
                std::cout << "  " << key << ": " << value << "\n";
            }
        }
    }
    
    std::cout << "---\n";
}

} // namespace chtl