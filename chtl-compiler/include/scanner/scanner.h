#ifndef CHTL_SCANNER_H
#define CHTL_SCANNER_H

#include "common/types.h"
#include <string>
#include <vector>
#include <memory>

namespace chtl {

class CHTLUnifiedScanner {
public:
    CHTLUnifiedScanner();
    ~CHTLUnifiedScanner();
    
    // 扫描CHTL源文件，返回切割后的代码片段
    std::vector<CodeFragment> scan(const std::string& source_code, 
                                  const std::string& file_path = "");
    
    // 从文件扫描
    std::vector<CodeFragment> scanFile(const std::string& file_path);
    
    // 设置调试模式
    void setDebugMode(bool debug) { debug_mode_ = debug; }
    
    // 获取扫描错误信息
    const std::vector<std::string>& getErrors() const { return errors_; }
    
    // 清空错误信息
    void clearErrors() { errors_.clear(); }

private:
    // 扫描状态枚举
    enum class ScanState {
        NORMAL,           // 普通CHTL代码
        IN_STYLE,         // 在style块内
        IN_SCRIPT,        // 在script块内
        IN_COMMENT,       // 在注释内
        IN_STRING,        // 在字符串内
        IN_ORIGIN_HTML,   // 在[Origin] @Html块内
        IN_ORIGIN_CSS,    // 在[Origin] @Style块内
        IN_ORIGIN_JS      // 在[Origin] @JavaScript块内
    };
    
    // 内部扫描实现
    void processChar(char ch, size_t pos);
    void processLine(const std::string& line, size_t line_num);
    void handleStateTransition(const std::string& content, size_t pos);
    void extractFragment();
    
    // 识别代码块类型
    bool isStyleBlock(const std::string& content, size_t pos);
    bool isScriptBlock(const std::string& content, size_t pos);
    bool isOriginBlock(const std::string& content, size_t pos);
    bool isGlobalStyleBlock(const std::string& content, size_t pos);
    
    // 工具函数
    std::string trim(const std::string& str);
    bool startsWith(const std::string& str, const std::string& prefix, size_t pos = 0);
    
    // 成员变量
    bool debug_mode_;
    std::vector<std::string> errors_;
    std::vector<CodeFragment> fragments_;
    
    // 扫描状态
    ScanState current_state_;
    std::string current_content_;
    std::string current_context_;
    size_t current_line_start_;
    size_t current_line_;
    size_t brace_count_;  // 花括号计数
    char string_delimiter_;  // 字符串定界符
    
    // 文件信息
    std::string current_file_;
};

} // namespace chtl

#endif // CHTL_SCANNER_H