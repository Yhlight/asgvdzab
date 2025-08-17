#include "scanner/scanner.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <regex>

namespace chtl {

CHTLUnifiedScanner::CHTLUnifiedScanner() 
    : debug_mode_(false),
      current_state_(ScanState::NORMAL),
      current_line_start_(1),
      current_line_(1),
      brace_count_(0),
      string_delimiter_('\0') {
}

CHTLUnifiedScanner::~CHTLUnifiedScanner() {
}

std::vector<CodeFragment> CHTLUnifiedScanner::scan(const std::string& source_code, 
                                                   const std::string& file_path) {
    // 清空之前的结果
    fragments_.clear();
    errors_.clear();
    current_file_ = file_path;
    
    // 重置状态
    current_state_ = ScanState::NORMAL;
    current_content_.clear();
    current_context_.clear();
    current_line_start_ = 1;
    current_line_ = 1;
    brace_count_ = 0;
    
    // 按行处理
    std::istringstream stream(source_code);
    std::string line;
    size_t line_num = 1;
    
    while (std::getline(stream, line)) {
        processLine(line, line_num);
        line_num++;
    }
    
    // 处理最后的片段
    if (!current_content_.empty()) {
        extractFragment();
    }
    
    if (debug_mode_) {
        std::cout << "Scanner found " << fragments_.size() << " fragments\n";
    }
    
    return fragments_;
}

std::vector<CodeFragment> CHTLUnifiedScanner::scanFile(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        errors_.push_back("Cannot open file: " + file_path);
        return {};
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();
    
    return scan(content, file_path);
}

void CHTLUnifiedScanner::processLine(const std::string& line, size_t line_num) {
    current_line_ = line_num;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char ch = line[i];
        
        // 处理注释
        if (current_state_ != ScanState::IN_STRING && 
            current_state_ != ScanState::IN_COMMENT) {
            // 单行注释
            if (i < line.length() - 1 && ch == '/' && line[i + 1] == '/') {
                // 跳过本行剩余部分
                current_content_ += line.substr(i) + "\n";
                return;
            }
            // 多行注释开始
            if (i < line.length() - 1 && ch == '/' && line[i + 1] == '*') {
                current_state_ = ScanState::IN_COMMENT;
                current_content_ += "/*";
                i++; // 跳过*
                continue;
            }
        }
        
        // 处理多行注释结束
        if (current_state_ == ScanState::IN_COMMENT) {
            current_content_ += ch;
            if (i < line.length() - 1 && ch == '*' && line[i + 1] == '/') {
                current_content_ += '/';
                current_state_ = ScanState::NORMAL;
                i++; // 跳过/
            }
            continue;
        }
        
        // 处理字符串
        if (current_state_ != ScanState::IN_COMMENT) {
            if (current_state_ == ScanState::IN_STRING) {
                current_content_ += ch;
                if (ch == string_delimiter_ && (i == 0 || line[i-1] != '\\')) {
                    current_state_ = ScanState::NORMAL;
                    string_delimiter_ = '\0';
                }
                continue;
            } else if (ch == '"' || ch == '\'') {
                current_content_ += ch;
                current_state_ = ScanState::IN_STRING;
                string_delimiter_ = ch;
                continue;
            }
        }
        
        // 处理特殊块
        handleStateTransition(line, i);
        
        // 处理花括号
        if (current_state_ != ScanState::IN_STRING && 
            current_state_ != ScanState::IN_COMMENT) {
            if (ch == '{') {
                brace_count_++;
            } else if (ch == '}') {
                if (brace_count_ > 0) {
                    brace_count_--;
                }
                
                // 检查是否结束当前块
                if (brace_count_ == 0 && 
                    (current_state_ == ScanState::IN_STYLE ||
                     current_state_ == ScanState::IN_SCRIPT ||
                     current_state_ == ScanState::IN_ORIGIN_HTML ||
                     current_state_ == ScanState::IN_ORIGIN_CSS ||
                     current_state_ == ScanState::IN_ORIGIN_JS)) {
                    current_content_ += ch;
                    extractFragment();
                    continue;
                }
            }
        }
        
        current_content_ += ch;
    }
    
    current_content_ += "\n";
}

void CHTLUnifiedScanner::handleStateTransition(const std::string& line, size_t pos) {
    if (current_state_ != ScanState::NORMAL) {
        return;
    }
    
    // 检查style块
    if (isStyleBlock(line, pos)) {
        if (!current_content_.empty()) {
            extractFragment();
        }
        current_state_ = ScanState::IN_STYLE;
        current_line_start_ = current_line_;
        brace_count_ = 0;
        return;
    }
    
    // 检查script块
    if (isScriptBlock(line, pos)) {
        if (!current_content_.empty()) {
            extractFragment();
        }
        current_state_ = ScanState::IN_SCRIPT;
        current_line_start_ = current_line_;
        brace_count_ = 0;
        return;
    }
    
    // 检查[Origin]块
    if (isOriginBlock(line, pos)) {
        if (!current_content_.empty()) {
            extractFragment();
        }
        
        // 判断Origin类型
        if (line.find("@Html") != std::string::npos) {
            current_state_ = ScanState::IN_ORIGIN_HTML;
        } else if (line.find("@Style") != std::string::npos) {
            current_state_ = ScanState::IN_ORIGIN_CSS;
        } else if (line.find("@JavaScript") != std::string::npos) {
            current_state_ = ScanState::IN_ORIGIN_JS;
        }
        
        current_line_start_ = current_line_;
        brace_count_ = 0;
        return;
    }
}

void CHTLUnifiedScanner::extractFragment() {
    CodeFragmentType type = CodeFragmentType::CHTL;
    
    switch (current_state_) {
        case ScanState::IN_STYLE:
            // 判断是局部style还是全局style
            type = isGlobalStyleBlock(current_content_, 0) ? 
                   CodeFragmentType::CSS : CodeFragmentType::CHTL;
            break;
        case ScanState::IN_SCRIPT:
            // 需要进一步判断是CHTL JS还是纯JS
            type = (current_content_.find("{{") != std::string::npos) ?
                   CodeFragmentType::CHTL_JS : CodeFragmentType::JAVASCRIPT;
            break;
        case ScanState::IN_ORIGIN_HTML:
            type = CodeFragmentType::HTML_TEXT;
            break;
        case ScanState::IN_ORIGIN_CSS:
            type = CodeFragmentType::CSS;
            break;
        case ScanState::IN_ORIGIN_JS:
            type = CodeFragmentType::JAVASCRIPT;
            break;
        default:
            type = CodeFragmentType::CHTL;
            break;
    }
    
    CodeFragment fragment(type, current_content_, 
                         current_line_start_, current_line_);
    fragment.source_file = current_file_;
    fragment.context = current_context_;
    
    fragments_.push_back(fragment);
    
    // 重置状态
    current_state_ = ScanState::NORMAL;
    current_content_.clear();
    current_context_.clear();
    current_line_start_ = current_line_ + 1;
}

bool CHTLUnifiedScanner::isStyleBlock(const std::string& content, size_t pos) {
    std::string trimmed = trim(content.substr(pos));
    return trimmed.find("style") == 0 && 
           (trimmed.length() == 5 || std::isspace(trimmed[5]) || trimmed[5] == '{');
}

bool CHTLUnifiedScanner::isScriptBlock(const std::string& content, size_t pos) {
    std::string trimmed = trim(content.substr(pos));
    return trimmed.find("script") == 0 && 
           (trimmed.length() == 6 || std::isspace(trimmed[6]) || trimmed[6] == '{');
}

bool CHTLUnifiedScanner::isOriginBlock(const std::string& content, size_t pos) {
    std::string trimmed = trim(content.substr(pos));
    return trimmed.find("[Origin]") == 0;
}

bool CHTLUnifiedScanner::isGlobalStyleBlock(const std::string& content, size_t pos) {
    // 全局style块的特征：
    // 1. 没有父元素上下文
    // 2. 在顶层定义
    // 这里简化判断，实际需要更复杂的上下文分析
    return current_context_.empty() && brace_count_ == 0;
}

std::string CHTLUnifiedScanner::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

bool CHTLUnifiedScanner::startsWith(const std::string& str, 
                                    const std::string& prefix, size_t pos) {
    return str.substr(pos, prefix.length()) == prefix;
}

} // namespace chtl