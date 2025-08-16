#include "scanner/unified_scanner.h"
#include "common/utils.h"
#include <algorithm>
#include <cctype>
#include <set>

namespace chtl {
namespace scanner {

// CHTL关键字集合
static const std::set<std::string> CHTL_KEYWORDS = {
    "text", "style", "script", "inherit", "delete", "insert",
    "after", "before", "replace", "at", "top", "bottom",
    "from", "as", "except"
};

// HTML元素名集合（常见元素）
static const std::set<std::string> HTML_ELEMENTS = {
    "html", "head", "body", "div", "span", "p", "a", "img",
    "ul", "ol", "li", "table", "tr", "td", "th", "form",
    "input", "button", "textarea", "select", "option",
    "h1", "h2", "h3", "h4", "h5", "h6", "header", "footer",
    "nav", "section", "article", "aside", "main", "br", "hr"
};

UnifiedScanner::UnifiedScanner() {
    clearDiagnostics();
}

UnifiedScanner::~UnifiedScanner() = default;

std::vector<CodeSegment> UnifiedScanner::scan(const std::string& source, 
                                             const std::string& filename) {
    std::vector<CodeSegment> segments;
    context_ = ScannerContext();
    context_.currentFile = filename;
    clearDiagnostics();
    
    size_t position = 0;
    bool inSingleLineComment = false;
    bool inMultiLineComment = false;
    bool inString = false;
    char stringDelimiter = '\0';
    
    currentSegment_ = CodeSegment(CodeSegmentType::CHTL);
    currentSegment_.location.filename = filename;
    segmentBuffer_.clear();
    
    while (position < source.length()) {
        char ch = source[position];
        char nextCh = (position + 1 < source.length()) ? source[position + 1] : '\0';
        
        // 处理注释
        if (!inString) {
            if (!inSingleLineComment && !inMultiLineComment) {
                if (ch == '/' && nextCh == '/') {
                    inSingleLineComment = true;
                    segmentBuffer_ += ch;
                    updateLocation(ch);
                    position++;
                    continue;
                } else if (ch == '/' && nextCh == '*') {
                    inMultiLineComment = true;
                    segmentBuffer_ += ch;
                    updateLocation(ch);
                    position++;
                    continue;
                }
            }
            
            if (inSingleLineComment && ch == '\n') {
                inSingleLineComment = false;
            } else if (inMultiLineComment && ch == '*' && nextCh == '/') {
                segmentBuffer_ += ch;
                updateLocation(ch);
                position++;
                ch = source[position];
                segmentBuffer_ += ch;
                updateLocation(ch);
                position++;
                inMultiLineComment = false;
                continue;
            }
        }
        
        // 如果在注释中，只是添加字符并继续
        if (inSingleLineComment || inMultiLineComment) {
            segmentBuffer_ += ch;
            updateLocation(ch);
            position++;
            continue;
        }
        
        // 处理字符串
        if (!inString && (ch == '"' || ch == '\'')) {
            inString = true;
            stringDelimiter = ch;
        } else if (inString && ch == stringDelimiter && source[position - 1] != '\\') {
            inString = false;
            stringDelimiter = '\0';
        }
        
        // 如果在字符串中，只是添加字符并继续
        if (inString) {
            segmentBuffer_ += ch;
            updateLocation(ch);
            position++;
            continue;
        }
        
        // 处理特殊关键字和块
        if (std::isalpha(ch)) {
            size_t wordStart = position;
            std::string word = extractWord(source, position);
            
            // 检查是否是style块
            if (word == "style" && context_.state == ScannerState::NORMAL) {
                // 查找下一个非空白字符
                size_t tempPos = position;
                skipWhitespace(source, tempPos);
                if (tempPos < source.length() && source[tempPos] == '{') {
                    // 保存当前缓冲区内容
                    if (!segmentBuffer_.empty()) {
                        currentSegment_.content = segmentBuffer_;
                        segments.push_back(currentSegment_);
                        segmentBuffer_.clear();
                    }
                    
                    // 进入style块状态
                    context_.state = ScannerState::IN_STYLE_BLOCK;
                    handleStyleBlock(wordStart, segments);
                    position = tempPos;
                    continue;
                }
            }
            // 检查是否是script块
            else if (word == "script" && context_.state == ScannerState::NORMAL) {
                size_t tempPos = position;
                skipWhitespace(source, tempPos);
                if (tempPos < source.length() && source[tempPos] == '{') {
                    if (!segmentBuffer_.empty()) {
                        currentSegment_.content = segmentBuffer_;
                        segments.push_back(currentSegment_);
                        segmentBuffer_.clear();
                    }
                    
                    context_.state = ScannerState::IN_SCRIPT_BLOCK;
                    handleScriptBlock(wordStart, segments);
                    position = tempPos;
                    continue;
                }
            }
            
            // 回退到单词开始位置，正常处理
            position = wordStart;
        }
        
        // 处理[Origin]块
        if (ch == '[' && matchKeyword(source, position, "[Origin]")) {
            // 处理原始嵌入块
            position += 8; // 跳过"[Origin]"
            skipWhitespace(source, position);
            
            if (position < source.length() && source[position] == '@') {
                position++; // 跳过@
                std::string originType = extractWord(source, position);
                handleOriginBlock(originType, segments);
                continue;
            }
        }
        
        // 处理花括号
        if (ch == '{') {
            handleBraceOpen(segments);
        } else if (ch == '}') {
            handleBraceClose(segments);
        }
        
        // 添加字符到缓冲区
        segmentBuffer_ += ch;
        updateLocation(ch);
        position++;
    }
    
    // 处理剩余的内容
    if (!segmentBuffer_.empty()) {
        currentSegment_.content = segmentBuffer_;
        segments.push_back(currentSegment_);
    }
    
    return segments;
}

void UnifiedScanner::processCharacter(char ch, size_t position, 
                                     std::vector<CodeSegment>& segments) {
    segmentBuffer_ += ch;
    updateLocation(ch);
}

void UnifiedScanner::updateLocation(char ch) {
    if (ch == '\n') {
        context_.lineNumber++;
        context_.columnNumber = 1;
    } else {
        context_.columnNumber++;
    }
}

bool UnifiedScanner::isKeyword(const std::string& word) const {
    return CHTL_KEYWORDS.find(utils::toLower(word)) != CHTL_KEYWORDS.end();
}

bool UnifiedScanner::isElementName(const std::string& word) const {
    return HTML_ELEMENTS.find(utils::toLower(word)) != HTML_ELEMENTS.end();
}

void UnifiedScanner::handleBraceOpen(std::vector<CodeSegment>& segments) {
    context_.braceDepth++;
    
    // 检查是否进入元素内部
    if (context_.state == ScannerState::NORMAL) {
        // 查找最近的标识符作为元素名
        size_t pos = segmentBuffer_.rfind('\n');
        if (pos == std::string::npos) pos = 0;
        
        std::string lastLine = segmentBuffer_.substr(pos);
        lastLine = utils::trim(lastLine);
        
        if (isElementName(lastLine)) {
            context_.elementStack.push(lastLine);
            context_.currentElement = lastLine;
        }
    }
}

void UnifiedScanner::handleBraceClose(std::vector<CodeSegment>& segments) {
    if (context_.braceDepth > 0) {
        context_.braceDepth--;
    }
    
    // 检查是否退出特定块
    if (context_.braceDepth == 0) {
        if (context_.state == ScannerState::IN_STYLE_BLOCK) {
            // 结束style块
            currentSegment_.content = segmentBuffer_;
            segments.push_back(currentSegment_);
            segmentBuffer_.clear();
            
            // 恢复到CHTL状态
            context_.state = ScannerState::NORMAL;
            currentSegment_ = CodeSegment(CodeSegmentType::CHTL);
            currentSegment_.location.filename = context_.currentFile;
            currentSegment_.location.line = context_.lineNumber;
            currentSegment_.location.column = context_.columnNumber;
        } else if (context_.state == ScannerState::IN_SCRIPT_BLOCK) {
            // 结束script块
            currentSegment_.content = segmentBuffer_;
            segments.push_back(currentSegment_);
            segmentBuffer_.clear();
            
            context_.state = ScannerState::NORMAL;
            currentSegment_ = CodeSegment(CodeSegmentType::CHTL);
            currentSegment_.location.filename = context_.currentFile;
            currentSegment_.location.line = context_.lineNumber;
            currentSegment_.location.column = context_.columnNumber;
        }
    }
    
    // 退出元素
    if (!context_.elementStack.empty() && context_.braceDepth < context_.elementStack.size()) {
        context_.elementStack.pop();
        if (!context_.elementStack.empty()) {
            context_.currentElement = context_.elementStack.top();
        } else {
            context_.currentElement.clear();
        }
    }
}

void UnifiedScanner::handleStyleBlock(size_t position, std::vector<CodeSegment>& segments) {
    // 创建新的代码片段
    CodeSegmentType segmentType = isInLocalContext() ? CodeSegmentType::CHTL : CodeSegmentType::CSS;
    currentSegment_ = CodeSegment(segmentType);
    currentSegment_.location.filename = context_.currentFile;
    currentSegment_.location.line = context_.lineNumber;
    currentSegment_.location.column = context_.columnNumber;
    currentSegment_.parentElement = context_.currentElement;
    
    segmentBuffer_ = "style";
}

void UnifiedScanner::handleScriptBlock(size_t position, std::vector<CodeSegment>& segments) {
    // 判断是局部script还是全局script
    CodeSegmentType segmentType = isInLocalContext() ? CodeSegmentType::CHTL_JS : CodeSegmentType::JAVASCRIPT;
    currentSegment_ = CodeSegment(segmentType);
    currentSegment_.location.filename = context_.currentFile;
    currentSegment_.location.line = context_.lineNumber;
    currentSegment_.location.column = context_.columnNumber;
    currentSegment_.parentElement = context_.currentElement;
    
    segmentBuffer_ = "script";
}

void UnifiedScanner::handleOriginBlock(const std::string& originType, 
                                      std::vector<CodeSegment>& segments) {
    // 根据原始块类型创建相应的代码片段
    CodeSegmentType segmentType = CodeSegmentType::UNKNOWN;
    
    if (originType == "Html" || originType == "html") {
        segmentType = CodeSegmentType::HTML_RAW;
    } else if (originType == "Style" || originType == "style") {
        segmentType = CodeSegmentType::CSS;
    } else if (originType == "JavaScript" || originType == "javascript") {
        segmentType = CodeSegmentType::JAVASCRIPT;
    }
    
    if (!segmentBuffer_.empty()) {
        currentSegment_.content = segmentBuffer_;
        segments.push_back(currentSegment_);
        segmentBuffer_.clear();
    }
    
    currentSegment_ = CodeSegment(segmentType);
    currentSegment_.location.filename = context_.currentFile;
    currentSegment_.location.line = context_.lineNumber;
    currentSegment_.location.column = context_.columnNumber;
    
    context_.state = ScannerState::IN_ORIGIN_BLOCK;
}

void UnifiedScanner::flushCurrentSegment(std::vector<CodeSegment>& segments) {
    if (!segmentBuffer_.empty()) {
        currentSegment_.content = segmentBuffer_;
        segments.push_back(currentSegment_);
        segmentBuffer_.clear();
    }
}

CodeSegmentType UnifiedScanner::detectSegmentType(const std::string& content) const {
    // 简单的启发式方法来检测代码类型
    std::string trimmed = utils::trim(content);
    
    if (trimmed.find("style") == 0 && trimmed.find('{') != std::string::npos) {
        return isInLocalContext() ? CodeSegmentType::CHTL : CodeSegmentType::CSS;
    }
    if (trimmed.find("script") == 0 && trimmed.find('{') != std::string::npos) {
        return isInLocalContext() ? CodeSegmentType::CHTL_JS : CodeSegmentType::JAVASCRIPT;
    }
    
    return CodeSegmentType::CHTL;
}

bool UnifiedScanner::isInLocalContext() const {
    // 判断是否在元素内部（局部上下文）
    return !context_.elementStack.empty();
}

std::string UnifiedScanner::extractWord(const std::string& source, size_t& position) const {
    std::string word;
    while (position < source.length() && 
           (std::isalnum(source[position]) || source[position] == '_')) {
        word += source[position];
        position++;
    }
    return word;
}

void UnifiedScanner::skipWhitespace(const std::string& source, size_t& position) {
    while (position < source.length() && std::isspace(source[position])) {
        if (source[position] == '\n') {
            context_.lineNumber++;
            context_.columnNumber = 1;
        } else {
            context_.columnNumber++;
        }
        position++;
    }
}

bool UnifiedScanner::matchKeyword(const std::string& source, size_t position, 
                                 const std::string& keyword) const {
    if (position + keyword.length() > source.length()) {
        return false;
    }
    return source.substr(position, keyword.length()) == keyword;
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