#ifndef CHTL_UNIFIED_SCANNER_H
#define CHTL_UNIFIED_SCANNER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

// 代码片段类型枚举
enum class CodeFragmentType {
    CHTL,           // CHTL主体代码
    CHTL_JS,        // CHTL JS代码（script块中的增强JS语法）
    CSS,            // 纯CSS代码（全局style）
    JAVASCRIPT,     // 纯JavaScript代码
    HTML_ORIGIN,    // 原始HTML嵌入
    COMMENT,        // 注释
    UNKNOWN
};

// 代码片段结构
struct CodeFragment {
    CodeFragmentType type;
    std::string content;
    size_t startLine;
    size_t startColumn;
    size_t endLine;
    size_t endColumn;
    std::string sourcePath;
    
    // 额外的元数据
    std::unordered_map<std::string, std::string> metadata;
    
    CodeFragment() : type(CodeFragmentType::UNKNOWN), 
                     startLine(0), startColumn(0), 
                     endLine(0), endColumn(0) {}
};

// 扫描器配置
struct ScannerConfig {
    bool preserveComments = false;          // 是否保留注释
    bool enableDebugInfo = false;           // 是否启用调试信息
    bool strictMode = true;                 // 严格模式
    std::string encoding = "UTF-8";         // 文件编码
};

// CHTLUnifiedScanner - 精准代码切割器
class CHTLUnifiedScanner {
public:
    explicit CHTLUnifiedScanner(const ScannerConfig& config = ScannerConfig());
    ~CHTLUnifiedScanner();
    
    // 扫描CHTL源代码，返回切割后的代码片段
    std::vector<CodeFragment> scan(const std::string& sourceCode, 
                                   const std::string& sourcePath = "<input>");
    
    // 从文件扫描
    std::vector<CodeFragment> scanFile(const std::string& filePath);
    
    // 获取最后的错误信息
    std::string getLastError() const;
    
    // 获取扫描统计信息
    struct ScanStats {
        size_t totalFragments = 0;
        size_t chtlFragments = 0;
        size_t chtlJsFragments = 0;
        size_t cssFragments = 0;
        size_t jsFragments = 0;
        size_t htmlFragments = 0;
        size_t commentFragments = 0;
        size_t totalLines = 0;
    };
    ScanStats getStats() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

// 辅助函数：将片段类型转换为字符串
std::string fragmentTypeToString(CodeFragmentType type);

// 辅助函数：打印片段信息（用于调试）
void printFragment(const CodeFragment& fragment, bool verbose = false);

} // namespace chtl

#endif // CHTL_UNIFIED_SCANNER_H