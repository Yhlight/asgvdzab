#include "CHTLUnifiedScanner.hpp"
#include <algorithm>
#include <cctype>
#include <set>

CHTLUnifiedScanner::CHTLUnifiedScanner() 
    : currentState_(ScanState::CHTL), currentPos_(0) {
}

CHTLUnifiedScanner::~CHTLUnifiedScanner() {
}

std::vector<CodeFragment> CHTLUnifiedScanner::scan(const std::string& source) {
    source_ = source;
    currentPos_ = 0;
    fragments_.clear();
    currentState_ = ScanState::CHTL;
    
    while (currentPos_ < source_.length()) {
        switch (currentState_) {
            case ScanState::CHTL:
                processCHTL();
                break;
            case ScanState::CHTL_JS:
                processCHTLJS();
                break;
            case ScanState::CSS:
                processCSS();
                break;
            case ScanState::JAVASCRIPT:
                processJavaScript();
                break;
            default:
                consume(); // 处理其他状态
                break;
        }
    }
    
    // 优化片段合并策略
    optimizeFragments();
    
    return fragments_;
}

void CHTLUnifiedScanner::processCHTL() {
    size_t startLine = getCurrentLine();
    size_t startCol = getCurrentColumn();
    std::string content;
    
    while (currentPos_ < source_.length()) {
        char ch = peek();
        
        // 检查注释
        if (ch == '/' && peek(1) == '/') {
            // 单行注释
            while (currentPos_ < source_.length() && peek() != '\n') {
                content += consume();
            }
            if (peek() == '\n') {
                content += consume();
            }
            continue;
        }
        
        if (ch == '/' && peek(1) == '*') {
            // 多行注释
            content += consume(); // '/'
            content += consume(); // '*'
            while (currentPos_ < source_.length()) {
                if (peek() == '*' && peek(1) == '/') {
                    content += consume(); // '*'
                    content += consume(); // '/'
                    break;
                }
                content += consume();
            }
            continue;
        }
        
        // 检查字符串
        if (ch == '"' || ch == '\'') {
            char quote = ch;
            content += consume();
            while (currentPos_ < source_.length()) {
                char c = peek();
                if (c == quote) {
                    content += consume();
                    break;
                }
                if (c == '\\') {
                    content += consume();
                    if (currentPos_ < source_.length()) {
                        content += consume();
                    }
                } else {
                    content += consume();
                }
            }
            continue;
        }
        
        // 检查style块
        if (ch == 's' && source_.substr(currentPos_, 5) == "style" && 
            (currentPos_ + 5 >= source_.length() || !std::isalnum(source_[currentPos_ + 5]))) {
            // 检查前面是否有合适的上下文
            size_t lookback = currentPos_ > 0 ? currentPos_ - 1 : 0;
            while (lookback > 0 && std::isspace(source_[lookback])) {
                lookback--;
            }
            
            // style块开始
            content += "style";
            currentPos_ += 5;
            skipWhitespace();
            
            if (peek() == '{') {
                // 保存当前CHTL片段
                if (!content.empty()) {
                    addFragment(FragmentType::CHTL, content, startLine, startCol);
                }
                
                // 切换到CSS状态
                currentState_ = ScanState::CSS;
                return;
            }
        }
        
        // 检查script块
        if (ch == 's' && source_.substr(currentPos_, 6) == "script" && 
            (currentPos_ + 6 >= source_.length() || !std::isalnum(source_[currentPos_ + 6]))) {
            content += "script";
            currentPos_ += 6;
            skipWhitespace();
            
            if (peek() == '{') {
                // 保存当前CHTL片段
                if (!content.empty()) {
                    addFragment(FragmentType::CHTL, content, startLine, startCol);
                }
                
                // 检查是否包含CHTL JS语法
                size_t lookAhead = currentPos_ + 1;
                bool hasCHTLJS = false;
                int braceCount = 1;
                
                while (lookAhead < source_.length() && braceCount > 0) {
                    if (source_[lookAhead] == '{' && lookAhead + 1 < source_.length() && 
                        source_[lookAhead + 1] == '{') {
                        hasCHTLJS = true;
                        break;
                    }
                    if (source_[lookAhead] == '{') braceCount++;
                    if (source_[lookAhead] == '}') braceCount--;
                    lookAhead++;
                }
                
                // 切换到相应状态
                currentState_ = hasCHTLJS ? ScanState::CHTL_JS : ScanState::JAVASCRIPT;
                return;
            }
        }
        
        // 普通字符
        content += consume();
    }
    
    // 保存最后的片段
    if (!content.empty()) {
        addFragment(FragmentType::CHTL, content, startLine, startCol);
    }
}

void CHTLUnifiedScanner::processCHTLJS() {
    size_t startLine = getCurrentLine();
    size_t startCol = getCurrentColumn();
    std::string content = "script";
    
    skipWhitespace();
    
    if (peek() == '{') {
        content += consume(); // '{'
        int braceCount = 1;
        
        while (currentPos_ < source_.length() && braceCount > 0) {
            char ch = peek();
            
            // 处理CHTL JS选择器 {{}}
            if (ch == '{' && peek(1) == '{') {
                content += consume(); // '{'
                content += consume(); // '{'
                
                // 读取选择器内容
                while (currentPos_ < source_.length()) {
                    if (peek() == '}' && peek(1) == '}') {
                        content += consume(); // '}'
                        content += consume(); // '}'
                        break;
                    }
                    content += consume();
                }
                continue;
            }
            
            // 处理字符串
            if (ch == '"' || ch == '\'') {
                char quote = ch;
                content += consume();
                while (currentPos_ < source_.length()) {
                    char c = peek();
                    if (c == quote) {
                        content += consume();
                        break;
                    }
                    if (c == '\\') {
                        content += consume();
                        if (currentPos_ < source_.length()) {
                            content += consume();
                        }
                    } else {
                        content += consume();
                    }
                }
                continue;
            }
            
            // 处理大括号
            if (ch == '{') {
                braceCount++;
            } else if (ch == '}') {
                braceCount--;
            }
            
            content += consume();
        }
    }
    
    addFragment(FragmentType::CHTL_JS, content, startLine, startCol);
    currentState_ = ScanState::CHTL;
}

void CHTLUnifiedScanner::processCSS() {
    size_t startLine = getCurrentLine();
    size_t startCol = getCurrentColumn();
    std::string content = "style";
    
    skipWhitespace();
    
    if (peek() == '{') {
        content += consume(); // '{'
        int braceCount = 1;
        
        while (currentPos_ < source_.length() && braceCount > 0) {
            char ch = peek();
            
            // 处理字符串
            if (ch == '"' || ch == '\'') {
                char quote = ch;
                content += consume();
                while (currentPos_ < source_.length()) {
                    char c = peek();
                    if (c == quote) {
                        content += consume();
                        break;
                    }
                    if (c == '\\') {
                        content += consume();
                        if (currentPos_ < source_.length()) {
                            content += consume();
                        }
                    } else {
                        content += consume();
                    }
                }
                continue;
            }
            
            // 处理注释
            if (ch == '/' && peek(1) == '*') {
                content += consume(); // '/'
                content += consume(); // '*'
                while (currentPos_ < source_.length()) {
                    if (peek() == '*' && peek(1) == '/') {
                        content += consume(); // '*'
                        content += consume(); // '/'
                        break;
                    }
                    content += consume();
                }
                continue;
            }
            
            // 处理大括号
            if (ch == '{') {
                braceCount++;
            } else if (ch == '}') {
                braceCount--;
            }
            
            content += consume();
        }
    }
    
    addFragment(FragmentType::CSS, content, startLine, startCol);
    currentState_ = ScanState::CHTL;
}

void CHTLUnifiedScanner::processJavaScript() {
    size_t startLine = getCurrentLine();
    size_t startCol = getCurrentColumn();
    std::string content = "script";
    
    skipWhitespace();
    
    if (peek() == '{') {
        content += consume(); // '{'
        int braceCount = 1;
        
        while (currentPos_ < source_.length() && braceCount > 0) {
            char ch = peek();
            
            // 处理字符串
            if (ch == '"' || ch == '\'' || ch == '`') {
                char quote = ch;
                content += consume();
                while (currentPos_ < source_.length()) {
                    char c = peek();
                    if (c == quote) {
                        content += consume();
                        break;
                    }
                    if (c == '\\') {
                        content += consume();
                        if (currentPos_ < source_.length()) {
                            content += consume();
                        }
                    } else {
                        content += consume();
                    }
                }
                continue;
            }
            
            // 处理注释
            if (ch == '/' && peek(1) == '/') {
                while (currentPos_ < source_.length() && peek() != '\n') {
                    content += consume();
                }
                if (peek() == '\n') {
                    content += consume();
                }
                continue;
            }
            
            if (ch == '/' && peek(1) == '*') {
                content += consume(); // '/'
                content += consume(); // '*'
                while (currentPos_ < source_.length()) {
                    if (peek() == '*' && peek(1) == '/') {
                        content += consume(); // '*'
                        content += consume(); // '/'
                        break;
                    }
                    content += consume();
                }
                continue;
            }
            
            // 处理大括号
            if (ch == '{') {
                braceCount++;
            } else if (ch == '}') {
                braceCount--;
            }
            
            content += consume();
        }
    }
    
    addFragment(FragmentType::JAVASCRIPT, content, startLine, startCol);
    currentState_ = ScanState::CHTL;
}

bool CHTLUnifiedScanner::isCHTLKeyword(const std::string& word) {
    static const std::set<std::string> keywords = {
        "text", "style", "script", "inherit", "delete", "insert",
        "after", "before", "replace", "at", "top", "bottom",
        "from", "as", "except"
    };
    return keywords.find(word) != keywords.end();
}

bool CHTLUnifiedScanner::isCHTLJSSelector(size_t pos) {
    return pos + 1 < source_.length() && 
           source_[pos] == '{' && 
           source_[pos + 1] == '{';
}

char CHTLUnifiedScanner::peek(size_t offset) {
    size_t pos = currentPos_ + offset;
    return (pos < source_.length()) ? source_[pos] : '\0';
}

char CHTLUnifiedScanner::consume() {
    return (currentPos_ < source_.length()) ? source_[currentPos_++] : '\0';
}

void CHTLUnifiedScanner::skipWhitespace() {
    while (currentPos_ < source_.length() && std::isspace(source_[currentPos_])) {
        currentPos_++;
    }
}

void CHTLUnifiedScanner::addFragment(FragmentType type, const std::string& content, 
                                     size_t startLine, size_t startCol) {
    size_t endLine = getCurrentLine();
    size_t endCol = getCurrentColumn();
    fragments_.emplace_back(type, content, startLine, startCol, endLine, endCol);
}

size_t CHTLUnifiedScanner::getCurrentLine() {
    size_t line = 1;
    for (size_t i = 0; i < currentPos_ && i < source_.length(); ++i) {
        if (source_[i] == '\n') {
            line++;
        }
    }
    return line;
}

size_t CHTLUnifiedScanner::getCurrentColumn() {
    size_t col = 1;
    for (size_t i = currentPos_; i > 0 && source_[i - 1] != '\n'; --i) {
        col++;
    }
    return col;
}

void CHTLUnifiedScanner::optimizeFragments() {
    // 合并相邻的同类型片段
    std::vector<CodeFragment> optimized;
    
    for (const auto& fragment : fragments_) {
        if (!optimized.empty() && 
            optimized.back().type == fragment.type &&
            optimized.back().endLine == fragment.startLine) {
            // 合并片段
            optimized.back().content += fragment.content;
            optimized.back().endLine = fragment.endLine;
            optimized.back().endColumn = fragment.endColumn;
        } else {
            optimized.push_back(fragment);
        }
    }
    
    fragments_ = std::move(optimized);
}