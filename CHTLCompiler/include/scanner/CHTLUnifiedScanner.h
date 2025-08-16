#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <queue>

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
    
    // 片段完整性标志
    bool isComplete;        // 是否为完整片段
    bool needsExpansion;    // 是否需要向前扩增
    
    CodeFragment(FragmentType t, const std::string& c, 
                 size_t sl, size_t el, size_t sc, size_t ec, 
                 const std::string& sf = "")
        : type(t), content(c), startLine(sl), endLine(el), 
          startColumn(sc), endColumn(ec), sourceFile(sf),
          isComplete(false), needsExpansion(false) {}
};

// 扫描器配置
struct ScannerConfig {
    size_t initialSliceSize;      // 初始切片大小
    size_t maxSliceSize;          // 最大切片大小
    size_t expansionStep;         // 扩增步长
    size_t minFragmentSize;       // 最小片段大小
    bool enableContextAnalysis;   // 是否启用上下文分析
    bool enableSmartSplitting;    // 是否启用智能切割
    
    ScannerConfig() : 
        initialSliceSize(1000), maxSliceSize(10000), 
        expansionStep(500), minFragmentSize(100),
        enableContextAnalysis(true), enableSmartSplitting(true) {}
    
    ScannerConfig(size_t initial, size_t max, size_t step, size_t min, bool context, bool smart) :
        initialSliceSize(initial), maxSliceSize(max),
        expansionStep(step), minFragmentSize(min),
        enableContextAnalysis(context), enableSmartSplitting(smart) {}
};

// CHTL统一扫描器类
class CHTLUnifiedScanner {
public:
    CHTLUnifiedScanner(const ScannerConfig& config = ScannerConfig());
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
        size_t totalExpansions;
        size_t totalSplits;
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
    
    // 核心扫描方法
    std::vector<CodeFragment> performSmartScanning(const std::string& sourceCode);
    std::vector<CodeFragment> expandFragmentIfNeeded(const std::string& source, 
                                                    CodeFragment& fragment, 
                                                    size_t& currentPos);
    
    // 片段类型检测
    FragmentType detectFragmentType(const std::string& content, size_t line, size_t column);
    bool isFragmentComplete(const std::string& content, FragmentType type);
    bool needsFragmentExpansion(const std::string& content, FragmentType type);
    
    // 具体类型的完整性检查
    bool isCHTLFragmentComplete(const std::string& content);
    bool isCHTLJSFragmentComplete(const std::string& content);
    bool isCSSFragmentComplete(const std::string& content);
    bool isJavaScriptFragmentComplete(const std::string& content);
    
    // 智能切割方法
    std::vector<CodeFragment> splitFragmentIntoTokens(const CodeFragment& fragment);
    std::vector<CodeFragment> splitCHTLJSIntoTokens(const CodeFragment& fragment);
    std::vector<CodeFragment> splitCHTLIntoTokens(const CodeFragment& fragment);
    bool isTokenBoundary(const std::string& content, size_t pos);
    bool isCHTLTokenBoundary(const std::string& content, size_t pos);
    bool isCHTLJSTokenBoundary(const std::string& content, size_t pos);
    
    // 上下文分析
    bool analyzeContext(const std::string& content, size_t pos, FragmentType type);
    bool isBalancedBrackets(const std::string& content, size_t start, size_t end);
    bool isCompleteStatement(const std::string& content, size_t start, size_t end);
    
    // 边界查找方法
    size_t findNextCHTLJSTokenBoundary(const std::string& content, size_t pos);
    size_t findNextCHTLTokenBoundary(const std::string& content, size_t pos);
    size_t findMatchingBrace(const std::string& content, size_t startPos);
    
    // 辅助方法
    void addError(const std::string& error);
    void updateStats(FragmentType type);
    size_t findNextSafeCutPoint(const std::string& content, size_t start, size_t end);
    size_t findPreviousSafeCutPoint(const std::string& content, size_t start);
    
    // 正则表达式模式（用于类型检测）
    void initializePatterns();
};

} // namespace chtl