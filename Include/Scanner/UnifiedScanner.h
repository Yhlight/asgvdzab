#pragma once

#include "Common/FragmentTypes.h"
#include "Scanner/Token.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

namespace Chtl {

/**
 * 扫描器状态枚举
 */
enum class ScannerState {
    NORMAL,             // 正常状态
    IN_STRING,          // 字符串内部
    IN_COMMENT,         // 注释内部
    IN_BLOCK_COMMENT,   // 块注释内部
    IN_CHTL_BLOCK,      // CHTL块内部
    IN_STYLE_BLOCK,     // 样式块内部
    IN_SCRIPT_BLOCK,    // 脚本块内部
    IN_TEMPLATE_BLOCK,  // 模板块内部
    IN_CUSTOM_BLOCK,    // 自定义块内部
    IN_ORIGIN_BLOCK,    // 原始嵌入块内部
    IN_CONFIGURATION    // 配置块内部
};

/**
 * 块类型识别器
 */
class BlockTypeDetector {
public:
    static FragmentType DetectBlockType(const std::string& blockHeader);
    static bool IsStyleBlock(const std::string& content);
    static bool IsScriptBlock(const std::string& content);
    static bool IsChtlJsBlock(const std::string& content);
    static bool HasEnhancedSelectors(const std::string& content);
    static bool HasVirtualObjects(const std::string& content);
};

/**
 * 括号匹配器
 */
class BraceMatcher {
public:
    struct BraceInfo {
        char Type;          // '{', '[', '('
        size_t Position;    // 位置
        size_t Line;        // 行号
        size_t Column;      // 列号
    };
    
    void Push(char brace, size_t pos, size_t line, size_t col);
    bool Pop(char brace);
    bool IsEmpty() const { return Stack_.empty(); }
    size_t GetDepth() const { return Stack_.size(); }
    const BraceInfo& Top() const { return Stack_.back(); }
    
    void Clear() { Stack_.clear(); }

private:
    std::vector<BraceInfo> Stack_;
    
    bool IsMatchingPair(char open, char close) const;
};

/**
 * CHTL统一扫描器 - 精准代码切割器
 */
class ChtlUnifiedScanner {
public:
    ChtlUnifiedScanner();
    ~ChtlUnifiedScanner();
    
    /**
     * 扫描源代码并切割成片段
     */
    std::unique_ptr<FragmentContainer> Scan(const std::string& sourceCode, 
                                          const std::string& filename = "");
    
    /**
     * 扫描文件
     */
    std::unique_ptr<FragmentContainer> ScanFile(const std::string& filename);
    
    /**
     * 配置扫描器
     */
    void SetDebugMode(bool debug) { DebugMode_ = debug; }
    void SetStrictMode(bool strict) { StrictMode_ = strict; }
    void SetPreserveComments(bool preserve) { PreserveComments_ = preserve; }
    
    /**
     * 获取扫描统计信息
     */
    size_t GetTotalLines() const { return TotalLines_; }
    size_t GetTotalFragments() const { return TotalFragments_; }
    const std::vector<std::string>& GetErrors() const { return Errors_; }
    const std::vector<std::string>& GetWarnings() const { return Warnings_; }

private:
    // 扫描器状态
    ScannerState State_;
    size_t CurrentPos_;
    size_t CurrentLine_;
    size_t CurrentColumn_;
    std::string CurrentFilename_;
    
    // 扫描器配置
    bool DebugMode_;
    bool StrictMode_;
    bool PreserveComments_;
    
    // 统计信息
    size_t TotalLines_;
    size_t TotalFragments_;
    std::vector<std::string> Errors_;
    std::vector<std::string> Warnings_;
    
    // 工具对象
    std::unique_ptr<BraceMatcher> BraceMatcher_;
    std::unique_ptr<BlockTypeDetector> TypeDetector_;
    
    // CHTL关键字集合
    std::unordered_set<std::string> ChtlKeywords_;
    std::unordered_set<std::string> ChtlBlockKeywords_;
    
    /**
     * 初始化关键字集合
     */
    void InitializeKeywords();
    
    /**
     * 扫描主循环
     */
    void ScanInternal(const std::string& sourceCode, FragmentContainer* container);
    
    /**
     * 处理不同类型的块
     */
    void ProcessNormalText(const std::string& sourceCode, size_t& pos, FragmentContainer* container);
    void ProcessChtlBlock(const std::string& sourceCode, size_t& pos, FragmentContainer* container);
    void ProcessStyleBlock(const std::string& sourceCode, size_t& pos, FragmentContainer* container);
    void ProcessScriptBlock(const std::string& sourceCode, size_t& pos, FragmentContainer* container);
    void ProcessTemplateBlock(const std::string& sourceCode, size_t& pos, FragmentContainer* container);
    void ProcessOriginBlock(const std::string& sourceCode, size_t& pos, FragmentContainer* container);
    void ProcessConfigurationBlock(const std::string& sourceCode, size_t& pos, FragmentContainer* container);
    
    /**
     * 字符串和注释处理
     */
    void SkipString(const std::string& sourceCode, size_t& pos, char quote);
    void SkipLineComment(const std::string& sourceCode, size_t& pos);
    void SkipBlockComment(const std::string& sourceCode, size_t& pos);
    
    /**
     * 块结束检测
     */
    bool IsBlockEnd(const std::string& sourceCode, size_t pos) const;
    size_t FindBlockEnd(const std::string& sourceCode, size_t startPos) const;
    
    /**
     * 关键字识别
     */
    bool IsKeywordAt(const std::string& sourceCode, size_t pos, const std::string& keyword) const;
    std::string ExtractKeyword(const std::string& sourceCode, size_t pos) const;
    
    /**
     * 位置管理
     */
    void UpdatePosition(char ch);
    SourceLocation GetCurrentLocation() const;
    
    /**
     * 错误处理
     */
    void AddError(const std::string& message);
    void AddWarning(const std::string& message);
    
    /**
     * 工具函数
     */
    bool IsWhitespace(char ch) const;
    bool IsAlpha(char ch) const;
    bool IsAlphaNumeric(char ch) const;
    bool IsValidIdentifierChar(char ch) const;
    
    /**
     * 片段创建
     */
    std::unique_ptr<CodeFragment> CreateFragment(FragmentType type, 
                                               const std::string& content,
                                               const SourceLocation& location);
    
    /**
     * 内容预处理
     */
    std::string PreprocessContent(const std::string& content, FragmentType type);
    
    /**
     * 特殊块处理
     */
    void HandleLocalStyleBlock(const std::string& content, FragmentContainer* container);
    void HandleLocalScriptBlock(const std::string& content, FragmentContainer* container);
    void HandleGlobalStyleBlock(const std::string& content, FragmentContainer* container);
    void HandleGlobalScriptBlock(const std::string& content, FragmentContainer* container);
};

/**
 * 扫描器工厂
 */
class ScannerFactory {
public:
    static std::unique_ptr<ChtlUnifiedScanner> CreateScanner();
    static std::unique_ptr<ChtlUnifiedScanner> CreateDebugScanner();
    static std::unique_ptr<ChtlUnifiedScanner> CreateStrictScanner();
};

} // namespace Chtl