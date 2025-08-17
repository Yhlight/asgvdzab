#include "../../include/common/Utils.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

namespace CHTL {
namespace Utils {

// 字符串工具函数
std::string trim(const std::string& str) {
    return trimRight(trimLeft(str));
}

std::string trimLeft(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && isWhitespace(*start)) {
        start++;
    }
    return std::string(start, str.end());
}

std::string trimRight(const std::string& str) {
    auto end = str.end();
    while (end != str.begin() && isWhitespace(*(end - 1))) {
        end--;
    }
    return std::string(str.begin(), end);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    
    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    
    return result;
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    
    result.push_back(str.substr(start));
    return result;
}

std::string join(const std::vector<std::string>& parts, const std::string& separator) {
    if (parts.empty()) return "";
    
    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += separator + parts[i];
    }
    
    return result;
}

bool startsWith(const std::string& str, const std::string& prefix) {
    return str.length() >= prefix.length() && 
           str.substr(0, prefix.length()) == prefix;
}

bool endsWith(const std::string& str, const std::string& suffix) {
    return str.length() >= suffix.length() && 
           str.substr(str.length() - suffix.length()) == suffix;
}

std::string replace(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    size_t pos = 0;
    
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    
    return result;
}

std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// 字符检查函数
bool isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool isValidIdentifierStart(char c) {
    return isAlpha(c) || c == '_';
}

bool isValidIdentifierChar(char c) {
    return isAlphaNumeric(c) || c == '_' || c == '-';
}

// 文件操作函数
std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool writeFile(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    return true;
}

bool fileExists(const std::string& filepath) {
    std::ifstream file(filepath);
    return file.good();
}

std::string getFileExtension(const std::string& filepath) {
    size_t pos = filepath.find_last_of('.');
    if (pos == std::string::npos) {
        return "";
    }
    return filepath.substr(pos + 1);
}

std::string getFileName(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return filepath;
    }
    return filepath.substr(pos + 1);
}

std::string getDirectoryPath(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return "";
    }
    return filepath.substr(0, pos);
}

// HTML转义函数
std::string escapeHtml(const std::string& str) {
    std::string result = str;
    result = replace(result, "&", "&amp;");
    result = replace(result, "<", "&lt;");
    result = replace(result, ">", "&gt;");
    result = replace(result, "\"", "&quot;");
    result = replace(result, "'", "&#39;");
    return result;
}

std::string unescapeHtml(const std::string& str) {
    std::string result = str;
    result = replace(result, "&lt;", "<");
    result = replace(result, "&gt;", ">");
    result = replace(result, "&quot;", "\"");
    result = replace(result, "&#39;", "'");
    result = replace(result, "&amp;", "&");
    return result;
}

// CSS工具函数
bool isValidCSSProperty(const std::string& property) {
    // 简单的CSS属性验证
    if (property.empty()) return false;
    
    // CSS属性名只能包含字母、数字、连字符
    for (char c : property) {
        if (!isAlphaNumeric(c) && c != '-') {
            return false;
        }
    }
    
    return true;
}

bool isValidCSSValue(const std::string& value) {
    // 简单的CSS值验证
    return !value.empty();
}

std::string normalizeCSSSelector(const std::string& selector) {
    return trim(selector);
}

// JS工具函数
bool isValidJSIdentifier(const std::string& identifier) {
    if (identifier.empty()) return false;
    
    if (!isValidIdentifierStart(identifier[0])) {
        return false;
    }
    
    for (size_t i = 1; i < identifier.length(); ++i) {
        if (!isValidIdentifierChar(identifier[i])) {
            return false;
        }
    }
    
    return true;
}

std::string escapeJSString(const std::string& str) {
    std::string result = str;
    result = replace(result, "\\", "\\\\");
    result = replace(result, "\"", "\\\"");
    result = replace(result, "'", "\\'");
    result = replace(result, "\n", "\\n");
    result = replace(result, "\r", "\\r");
    result = replace(result, "\t", "\\t");
    return result;
}

// 错误处理
std::string formatError(const std::string& message, size_t line, size_t column) {
    return "错误 [" + std::to_string(line) + ":" + std::to_string(column) + "] " + message;
}

std::string formatWarning(const std::string& message, size_t line, size_t column) {
    return "警告 [" + std::to_string(line) + ":" + std::to_string(column) + "] " + message;
}

// 调试工具
void debugLog(const std::string& message) {
    std::cout << "[DEBUG] " << message << std::endl;
}

void debugLog(const std::string& tag, const std::string& message) {
    std::cout << "[DEBUG:" << tag << "] " << message << std::endl;
}

} // namespace Utils
} // namespace CHTL