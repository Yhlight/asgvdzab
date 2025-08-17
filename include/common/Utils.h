#pragma once

#include <string>
#include <vector>
#include <map>

namespace CHTL {
namespace Utils {

// 字符串工具函数
std::string trim(const std::string& str);
std::string trimLeft(const std::string& str);
std::string trimRight(const std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);
std::vector<std::string> split(const std::string& str, const std::string& delimiter);
std::string join(const std::vector<std::string>& parts, const std::string& separator);
bool startsWith(const std::string& str, const std::string& prefix);
bool endsWith(const std::string& str, const std::string& suffix);
std::string replace(const std::string& str, const std::string& from, const std::string& to);
std::string toLower(const std::string& str);
std::string toUpper(const std::string& str);

// 字符检查函数
bool isWhitespace(char c);
bool isAlpha(char c);
bool isDigit(char c);
bool isAlphaNumeric(char c);
bool isValidIdentifierStart(char c);
bool isValidIdentifierChar(char c);

// 文件操作函数
std::string readFile(const std::string& filepath);
bool writeFile(const std::string& filepath, const std::string& content);
bool fileExists(const std::string& filepath);
std::string getFileExtension(const std::string& filepath);
std::string getFileName(const std::string& filepath);
std::string getDirectoryPath(const std::string& filepath);

// HTML转义函数
std::string escapeHtml(const std::string& str);
std::string unescapeHtml(const std::string& str);

// CSS工具函数
bool isValidCSSProperty(const std::string& property);
bool isValidCSSValue(const std::string& value);
std::string normalizeCSSSelector(const std::string& selector);

// JS工具函数
bool isValidJSIdentifier(const std::string& identifier);
std::string escapeJSString(const std::string& str);

// 错误处理
std::string formatError(const std::string& message, size_t line, size_t column);
std::string formatWarning(const std::string& message, size_t line, size_t column);

// 调试工具
void debugLog(const std::string& message);
void debugLog(const std::string& tag, const std::string& message);

} // namespace Utils
} // namespace CHTL