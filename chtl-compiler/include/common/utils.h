#ifndef CHTL_COMMON_UTILS_H
#define CHTL_COMMON_UTILS_H

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>

namespace chtl {
namespace utils {

// 字符串处理工具
inline std::string trim(const std::string& str) {
    auto start = str.begin();
    auto end = str.end();
    
    // 移除开头空白
    while (start != end && std::isspace(static_cast<unsigned char>(*start))) {
        ++start;
    }
    
    // 移除结尾空白
    while (start != end && std::isspace(static_cast<unsigned char>(*(end - 1)))) {
        --end;
    }
    
    return std::string(start, end);
}

inline std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

inline std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

inline std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

inline std::string join(const std::vector<std::string>& vec, const std::string& delimiter) {
    std::string result;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) result += delimiter;
        result += vec[i];
    }
    return result;
}

inline bool startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && 
           str.compare(0, prefix.size(), prefix) == 0;
}

inline bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// 替换字符串中的所有匹配项
inline std::string replaceAll(const std::string& str, 
                              const std::string& from, 
                              const std::string& to) {
    if (from.empty()) return str;
    
    std::string result;
    size_t start = 0;
    size_t pos = 0;
    
    while ((pos = str.find(from, start)) != std::string::npos) {
        result.append(str, start, pos - start);
        result.append(to);
        start = pos + from.length();
    }
    
    result.append(str, start);
    return result;
}

// 生成唯一的类名或ID
inline std::string generateUniqueId(const std::string& prefix, size_t counter) {
    return prefix + "_" + std::to_string(counter);
}

// 检查是否是有效的标识符
inline bool isValidIdentifier(const std::string& str) {
    if (str.empty()) return false;
    
    // 首字符必须是字母或下划线
    if (!std::isalpha(str[0]) && str[0] != '_') return false;
    
    // 其余字符可以是字母、数字或下划线
    for (size_t i = 1; i < str.length(); ++i) {
        if (!std::isalnum(str[i]) && str[i] != '_') return false;
    }
    
    return true;
}

// 转义HTML特殊字符
inline std::string escapeHtml(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default: result += c;
        }
    }
    return result;
}

// 格式化错误消息
inline std::string formatError(const std::string& filename, 
                               size_t line, 
                               size_t column, 
                               const std::string& message) {
    std::stringstream ss;
    ss << filename << ":" << line << ":" << column << ": error: " << message;
    return ss.str();
}

inline std::string formatWarning(const std::string& filename, 
                                 size_t line, 
                                 size_t column, 
                                 const std::string& message) {
    std::stringstream ss;
    ss << filename << ":" << line << ":" << column << ": warning: " << message;
    return ss.str();
}

} // namespace utils
} // namespace chtl

#endif // CHTL_COMMON_UTILS_H