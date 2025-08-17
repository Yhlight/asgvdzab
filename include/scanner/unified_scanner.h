#pragma once

#include "common/fragment_types.h"
#include "scanner/token.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

namespace chtl {

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
    static FragmentType detectBlockType(const std::string& block_header);
    static bool isStyleBlock(const std::string& content);
    static bool isScriptBlock(const std::string& content);
    static bool isCHTLJSBlock(const std::string& content);
    static bool hasEnhancedSelectors(const std::string& content);
    static bool hasVirtualObjects(const std::string& content);
};

/**
 * 括号匹配器
 */
class BraceMatcher {
public:
    struct BraceInfo {
        char type;          // '{', '[', '('
        size_t position;    // 位置
        size_t line;        // 行号
        size_t column;      // 列号
    };
    
    void push(char brace, size_t pos, size_t line, size_t col);
    bool pop(char brace);
    bool isEmpty() const { return stack_.empty(); }
    size_t getDepth() const { return stack_.size(); }
    const BraceInfo& top() const { return stack_.back(); }
    
    void clear() { stack_.clear(); }

private:
    std::vector<BraceInfo> stack_;
    
    bool isMatchingPair(char open, char close) const;
};

/**
 * CHTL统一扫描器 - 精准代码切割器
 */
class CHTLUnifiedScanner {
public:
    CHTLUnifiedScanner();
    ~CHTLUnifiedScanner();
    
    /**
     * 扫描源代码并切割成片段
     */
    std::unique_ptr<FragmentContainer> scan(const std::string& source_code, 
                                          const std::string& filename = "");
    
    /**
     * 扫描文件
     */
    std::unique_ptr<FragmentContainer> scanFile(const std::string& filename);
    
    /**
     * 配置扫描器
     */
    void setDebugMode(bool debug) { debug_mode_ = debug; }
    void setStrictMode(bool strict) { strict_mode_ = strict; }
    void setPreserveComments(bool preserve) { preserve_comments_ = preserve; }
    
    /**
     * 获取扫描统计信息
     */
    size_t getTotalLines() const { return total_lines_; }
    size_t getTotalFragments() const { return total_fragments_; }
    const std::vector<std::string>& getErrors() const { return errors_; }
    const std::vector<std::string>& getWarnings() const { return warnings_; }

private:
    // 扫描器状态
    ScannerState state_;
    size_t current_pos_;
    size_t current_line_;
    size_t current_column_;
    std::string current_filename_;
    
    // 扫描器配置
    bool debug_mode_;
    bool strict_mode_;
    bool preserve_comments_;
    
    // 统计信息
    size_t total_lines_;
    size_t total_fragments_;
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    
    // 工具对象
    std::unique_ptr<BraceMatcher> brace_matcher_;
    std::unique_ptr<BlockTypeDetector> type_detector_;
    
    // CHTL关键字集合
    std::unordered_set<std::string> chtl_keywords_;
    std::unordered_set<std::string> chtl_block_keywords_;
    
    /**
     * 初始化关键字集合
     */
    void initializeKeywords();
    
    /**
     * 扫描主循环
     */
    void scanInternal(const std::string& source_code, FragmentContainer* container);
    
    /**
     * 处理不同类型的块
     */
    void processNormalText(const std::string& source_code, size_t& pos, FragmentContainer* container);
    void processCHTLBlock(const std::string& source_code, size_t& pos, FragmentContainer* container);
    void processStyleBlock(const std::string& source_code, size_t& pos, FragmentContainer* container);
    void processScriptBlock(const std::string& source_code, size_t& pos, FragmentContainer* container);
    void processTemplateBlock(const std::string& source_code, size_t& pos, FragmentContainer* container);
    void processOriginBlock(const std::string& source_code, size_t& pos, FragmentContainer* container);
    void processConfigurationBlock(const std::string& source_code, size_t& pos, FragmentContainer* container);
    
    /**
     * 字符串和注释处理
     */
    void skipString(const std::string& source_code, size_t& pos, char quote);
    void skipLineComment(const std::string& source_code, size_t& pos);
    void skipBlockComment(const std::string& source_code, size_t& pos);
    
    /**
     * 块结束检测
     */
    bool isBlockEnd(const std::string& source_code, size_t pos) const;
    size_t findBlockEnd(const std::string& source_code, size_t start_pos) const;
    
    /**
     * 关键字识别
     */
    bool isKeywordAt(const std::string& source_code, size_t pos, const std::string& keyword) const;
    std::string extractKeyword(const std::string& source_code, size_t pos) const;
    
    /**
     * 位置管理
     */
    void updatePosition(char ch);
    SourceLocation getCurrentLocation() const;
    
    /**
     * 错误处理
     */
    void addError(const std::string& message);
    void addWarning(const std::string& message);
    
    /**
     * 工具函数
     */
    bool isWhitespace(char ch) const;
    bool isAlpha(char ch) const;
    bool isAlphaNumeric(char ch) const;
    bool isValidIdentifierChar(char ch) const;
    
    /**
     * 片段创建
     */
    std::unique_ptr<CodeFragment> createFragment(FragmentType type, 
                                               const std::string& content,
                                               const SourceLocation& location);
    
    /**
     * 内容预处理
     */
    std::string preprocessContent(const std::string& content, FragmentType type);
    
    /**
     * 特殊块处理
     */
    void handleLocalStyleBlock(const std::string& content, FragmentContainer* container);
    void handleLocalScriptBlock(const std::string& content, FragmentContainer* container);
    void handleGlobalStyleBlock(const std::string& content, FragmentContainer* container);
    void handleGlobalScriptBlock(const std::string& content, FragmentContainer* container);
};

/**
 * 扫描器工厂
 */
class ScannerFactory {
public:
    static std::unique_ptr<CHTLUnifiedScanner> createScanner();
    static std::unique_ptr<CHTLUnifiedScanner> createDebugScanner();
    static std::unique_ptr<CHTLUnifiedScanner> createStrictScanner();
};

} // namespace chtl