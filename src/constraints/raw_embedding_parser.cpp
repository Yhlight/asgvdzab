#include "constraints/raw_embedding_parser.hpp"
#include <cctype>
#include <algorithm>

namespace chtl {

RawEmbeddingParser::RawEmbeddingParser() {
}

std::vector<RawEmbeddingBlock> RawEmbeddingParser::parseRawEmbeddings(const std::string& text) {
    std::vector<RawEmbeddingBlock> blocks;
    foundTypes_.clear();
    
    size_t pos = 0;
    
    while (pos < text.length()) {
        // 跳过注释和字符串中的内容
        pos = skipCommentsAndStrings(text, pos);
        if (pos >= text.length()) break;
        
        // 查找 [Origin]
        if (matchString(text, pos, "[Origin]")) {
            size_t originStart = pos;
            pos += 8; // "[Origin]" 长度
            pos = skipWhitespace(text, pos);
            
            // 期待 @Type
            if (pos < text.length() && text[pos] == '@') {
                pos++; // 跳过 @
                
                RawEmbeddingBlock block;
                block.startPos = originStart;
                
                // 解析类型名
                pos = parseIdentifier(text, pos, block.type);
                
                if (!block.type.empty()) {
                    foundTypes_.insert(block.type);
                    pos = skipWhitespace(text, pos);
                    
                    // 检查是否有名称
                    if (pos < text.length() && std::isalpha(text[pos])) {
                        pos = parseIdentifier(text, pos, block.name);
                        pos = skipWhitespace(text, pos);
                    }
                    
                    // 检查是否为引用（;结尾）或定义（{开始）
                    if (pos < text.length()) {
                        if (text[pos] == ';') {
                            // 原始嵌入引用
                            block.isReference = true;
                            block.endPos = pos + 1;
                            blocks.push_back(block);
                            pos++;
                        } else if (text[pos] == '{') {
                            // 原始嵌入定义，查找匹配的右大括号
                            size_t contentStart = pos + 1;
                            size_t braceEnd = findMatchingBrace(text, pos);
                            
                            if (braceEnd != std::string::npos) {
                                block.content = text.substr(contentStart, braceEnd - contentStart);
                                block.endPos = braceEnd + 1;
                                blocks.push_back(block);
                                pos = braceEnd + 1;
                            } else {
                                // 找不到匹配的右大括号，跳过
                                pos++;
                            }
                        } else {
                            // 无效格式，继续寻找
                            pos++;
                        }
                    } else {
                        // 文本结束，无效格式
                        break;
                    }
                } else {
                    // 无效类型，继续寻找
                    pos++;
                }
            } else {
                // 不是有效的原始嵌入，继续寻找
                pos++;
            }
        } else {
            pos++;
        }
    }
    
    return blocks;
}

std::string RawEmbeddingParser::removeRawEmbeddingBlocks(const std::string& text) {
    auto blocks = parseRawEmbeddings(text);
    
    if (blocks.empty()) {
        return text;
    }
    
    // 按起始位置倒序排列，从后往前删除，避免位置偏移
    std::sort(blocks.begin(), blocks.end(), 
              [](const RawEmbeddingBlock& a, const RawEmbeddingBlock& b) {
                  return a.startPos > b.startPos;
              });
    
    std::string result = text;
    for (const auto& block : blocks) {
        if (block.endPos > block.startPos && block.endPos <= result.length()) {
            result.erase(block.startPos, block.endPos - block.startPos);
        }
    }
    
    return result;
}

bool RawEmbeddingParser::isValidRawEmbedding(const std::string& text) {
    auto blocks = parseRawEmbeddings(text);
    
    // 检查是否只有一个块
    if (blocks.size() != 1) {
        return false;
    }
    
    const auto& block = blocks[0];
    
    // 修剪前导和尾随空白
    std::string trimmed = text;
    size_t start = trimmed.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return false; // 空字符串不是有效的原始嵌入
    }
    
    size_t end = trimmed.find_last_not_of(" \t\n\r");
    trimmed = trimmed.substr(start, end - start + 1);
    
    // 重新解析修剪后的文本
    auto trimmedBlocks = parseRawEmbeddings(trimmed);
    if (trimmedBlocks.size() != 1) {
        return false;
    }
    
    const auto& trimmedBlock = trimmedBlocks[0];
    
    // 检查块是否覆盖整个修剪后的文本
    return trimmedBlock.startPos == 0 && trimmedBlock.endPos == trimmed.length();
}

size_t RawEmbeddingParser::skipCommentsAndStrings(const std::string& text, size_t pos) {
    while (pos < text.length()) {
        char ch = text[pos];
        
        // 检查单行注释
        if (ch == '/' && pos + 1 < text.length() && text[pos + 1] == '/') {
            // 跳到行尾
            pos += 2;
            while (pos < text.length() && text[pos] != '\n' && text[pos] != '\r') {
                pos++;
            }
            if (pos < text.length()) pos++; // 跳过换行符
            continue;
        }
        
        // 检查多行注释
        if (ch == '/' && pos + 1 < text.length() && text[pos + 1] == '*') {
            pos += 2;
            while (pos + 1 < text.length()) {
                if (text[pos] == '*' && text[pos + 1] == '/') {
                    pos += 2;
                    break;
                }
                pos++;
            }
            continue;
        }
        
        // 检查双引号字符串
        if (ch == '"') {
            pos++;
            bool escaped = false;
            while (pos < text.length()) {
                if (escaped) {
                    escaped = false;
                } else if (text[pos] == '\\') {
                    escaped = true;
                } else if (text[pos] == '"') {
                    pos++;
                    break;
                }
                pos++;
            }
            continue;
        }
        
        // 检查单引号字符串
        if (ch == '\'') {
            pos++;
            bool escaped = false;
            while (pos < text.length()) {
                if (escaped) {
                    escaped = false;
                } else if (text[pos] == '\\') {
                    escaped = true;
                } else if (text[pos] == '\'') {
                    pos++;
                    break;
                }
                pos++;
            }
            continue;
        }
        
        // 检查模板字符串
        if (ch == '`') {
            pos++;
            bool escaped = false;
            while (pos < text.length()) {
                if (escaped) {
                    escaped = false;
                } else if (text[pos] == '\\') {
                    escaped = true;
                } else if (text[pos] == '`') {
                    pos++;
                    break;
                }
                pos++;
            }
            continue;
        }
        
        // 如果不是注释或字符串，返回当前位置
        break;
    }
    
    return pos;
}

size_t RawEmbeddingParser::skipWhitespace(const std::string& text, size_t pos) {
    while (pos < text.length() && std::isspace(text[pos])) {
        pos++;
    }
    return pos;
}

size_t RawEmbeddingParser::parseIdentifier(const std::string& text, size_t pos, std::string& identifier) {
    identifier.clear();
    
    // 标识符必须以字母开头
    if (pos >= text.length() || !std::isalpha(text[pos])) {
        return pos;
    }
    
    // 解析标识符字符（字母、数字、下划线）
    while (pos < text.length() && 
           (std::isalnum(text[pos]) || text[pos] == '_')) {
        identifier += text[pos];
        pos++;
    }
    
    return pos;
}

size_t RawEmbeddingParser::findMatchingBrace(const std::string& text, size_t startPos) {
    if (startPos >= text.length() || text[startPos] != '{') {
        return std::string::npos;
    }
    
    int braceCount = 1;
    size_t pos = startPos + 1;
    
    while (pos < text.length() && braceCount > 0) {
        // 跳过注释和字符串中的大括号
        size_t oldPos = pos;
        pos = skipCommentsAndStrings(text, pos);
        
        if (pos != oldPos) {
            // 跳过了注释或字符串，继续
            continue;
        }
        
        char ch = text[pos];
        if (ch == '{') {
            braceCount++;
        } else if (ch == '}') {
            braceCount--;
        }
        
        pos++;
    }
    
    return (braceCount == 0) ? pos - 1 : std::string::npos;
}

bool RawEmbeddingParser::matchString(const std::string& text, size_t pos, const std::string& target) {
    if (pos + target.length() > text.length()) {
        return false;
    }
    
    return text.substr(pos, target.length()) == target;
}

} // namespace chtl