#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

// 代码片段类型枚举
enum class FragmentType {
    CHTL,           // CHTL片段
    CHTL_JS,        // CHTL JS片段
    CSS,            // CSS片段
    JAVASCRIPT      // JavaScript片段
};

// 代码片段结构
struct CodeFragment {
    FragmentType type;
    std::string content;
    size_t startLine;
    size_t endLine;
    size_t startColumn;
    size_t endColumn;
    std::string sourceFile;
    
    CodeFragment(FragmentType t, const std::string& c, 
                 size_t sl, size_t el, size_t sc, size_t ec, 
                 const std::string& sf = "")
        : type(t), content(c), startLine(sl), endLine(el), 
          startColumn(sc), endColumn(ec), sourceFile(sf) {}
};

// CHTL统一扫描器类
class CHTLUnifiedScanner {
public:
    CHTLUnifiedScanner();
    ~CHTLUnifiedScanner();
    
    // 扫描CHTL源代码文件
    std::vector<CodeFragment> scanFile(const std::string& filePath);
    
    // 扫描CHTL源代码字符串
    std::vector<CodeFragment> scanString(const std::string& sourceCode);
    
    // 获取扫描统计信息
    struct ScanStats {
        size_t totalFragments;
        size_t chtlFragments;
        size_t chtlJsFragments;
        size_t cssFragments;
        size_t javascriptFragments;
        size_t totalLines;
    };
    
    ScanStats getScanStats() const;
    
    // 设置扫描选项
    void setOptions(const std::unordered_map<std::string, std::string>& options);
    
    // 获取错误信息
    std::vector<std::string> getErrors() const;
    
    // 清除错误信息
    void clearErrors();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 私有方法
    void addError(const std::string& error);
    bool isValidFragment(const CodeFragment& fragment);
    FragmentType detectFragmentType(const std::string& content, size_t line, size_t column);
    std::string extractFragmentContent(const std::string& source, size_t start, size_t end);
    void updateStats(FragmentType type);
};

} // namespace chtl