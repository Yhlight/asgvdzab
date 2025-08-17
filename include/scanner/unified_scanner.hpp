#ifndef CHTL_UNIFIED_SCANNER_HPP
#define CHTL_UNIFIED_SCANNER_HPP

#include "core/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <regex>
#include <unordered_set>
#include <functional>

namespace CHTL {

// 最小单元类型
enum class MinimalUnitType {
    CHTL_SELECTOR,      // {{selector}}
    CHTL_ARROW,         // ->
    CHTL_LISTEN,        // listen
    CHTL_DELEGATE,      // delegate
    CHTL_ANIMATE,       // animate
    TEMPLATE_VAR,       // ThemeColor(varName)
    CUSTOM_VAR,         // CustomVar(varName = value)
    NAMESPACE_FROM,     // from namespace
    COMPLETE_STATEMENT, // 完整语句
    UNKNOWN
};

// 最小单元
struct MinimalUnit {
    MinimalUnitType type;
    std::string content;
    SourceLocation location;
    size_t length;
    
    MinimalUnit(MinimalUnitType t, const std::string& c, const SourceLocation& loc, size_t len)
        : type(t), content(c), location(loc), length(len) {}
};

// 切片验证器
class SliceValidator {
public:
    // 检查切片边界是否合理（括号平衡、不在字符串/注释内部等）
    static bool isValidSliceBoundary(const std::string& text, size_t position);
    
    // 检查是否在字符串内部
    static bool isInsideString(const std::string& text, size_t position);
    
    // 检查是否在注释内部
    static bool isInsideComment(const std::string& text, size_t position);
    
    // 检查括号是否平衡
    static bool isBalancedBraces(const std::string& text, size_t start, size_t end);
    
private:
    static std::pair<char, char> findStringDelimiters(const std::string& text, size_t position);
    static std::pair<size_t, size_t> findCommentBounds(const std::string& text, size_t position);
};

// CHTL/CHTL JS最小单元识别器
class MinimalUnitDetector {
public:
    explicit MinimalUnitDetector(const CompilerConfig& config);
    
    // 检测指定位置是否是CHTL/CHTL JS最小单元的开始
    std::optional<MinimalUnit> detectUnit(const std::string& text, size_t position);
    
    // 获取所有可能的最小单元起始模式
    const std::vector<std::regex>& getUnitPatterns() const { return unitPatterns_; }
    
private:
    CompilerConfig config_;
    std::vector<std::regex> unitPatterns_;
    std::unordered_map<std::string, MinimalUnitType> keywordToType_;
    
    void initializePatterns();
    MinimalUnitType classifyUnit(const std::string& content);
};

// 上下文分析器
class ContextAnalyzer {
public:
    explicit ContextAnalyzer(const CompilerConfig& config);
    
    // 分析当前代码片段的上下文
    std::unordered_map<std::string, std::string> analyzeContext(
        const std::string& text, size_t start, size_t end);
    
    // 判断当前上下文是否允许CHTL语法
    bool allowsCHTLSyntax(const std::unordered_map<std::string, std::string>& context);
    
    // 判断当前上下文是否允许CHTL JS语法
    bool allowsCHTLJSSyntax(const std::unordered_map<std::string, std::string>& context);
    
private:
    CompilerConfig config_;
    
    std::string detectBlockType(const std::string& text, size_t position);
    int calculateNestingLevel(const std::string& text, size_t position);
};

// CHTLUnifiedScanner - 精准代码切割器
class CHTLUnifiedScanner {
public:
    explicit CHTLUnifiedScanner(const CompilerConfig& config = CompilerConfig{});
    
    // 主要接口：扫描源代码并返回代码片段
    std::vector<CodeFragment> scan(const std::string& source, const std::string& filename = "");
    
    // 设置配置
    void setConfig(const CompilerConfig& config);
    const CompilerConfig& getConfig() const { return config_; }
    
    // 错误信息
    const std::vector<CompileError>& getErrors() const { return errors_; }
    void clearErrors() { errors_.clear(); }
    
private:
    CompilerConfig config_;
    std::unique_ptr<MinimalUnitDetector> unitDetector_;
    std::unique_ptr<ContextAnalyzer> contextAnalyzer_;
    std::vector<CompileError> errors_;
    
    // 核心扫描算法
    std::vector<CodeFragment> performVariableLengthSlicing(const std::string& source, const std::string& filename);
    
    // 检测片段类型
    FragmentType detectFragmentType(const std::string& content, const std::unordered_map<std::string, std::string>& context);
    
    // 可变长度切片
    size_t findOptimalSliceEnd(const std::string& source, size_t start, size_t initialEnd);
    
    // 最小单元二次切割
    std::vector<CodeFragment> performMinimalUnitCutting(const CodeFragment& fragment);
    
    // CHTL/CHTL JS片段连续性检查
    bool shouldMergeWithNext(const CodeFragment& current, const CodeFragment& next);
    
    // 片段验证和优化
    std::vector<CodeFragment> validateAndOptimizeFragments(std::vector<CodeFragment> fragments);
    
    // 错误处理
    void reportError(const std::string& message, const SourceLocation& location);
    void reportWarning(const std::string& message, const SourceLocation& location);
    
    // 辅助方法
    SourceLocation calculateLocation(const std::string& source, size_t offset, const std::string& filename);
    std::string extractContent(const std::string& source, size_t start, size_t end);
    
    // 特殊语法检测
    bool containsCHTLSyntax(const std::string& content);
    bool containsCHTLJSSyntax(const std::string& content);
    bool containsTemplateSyntax(const std::string& content);
    bool containsNamespaceSyntax(const std::string& content);
    
    // 块类型检测
    enum class BlockType {
        GLOBAL_STYLE,
        LOCAL_STYLE,
        LOCAL_SCRIPT,
        GLOBAL_SCRIPT,
        ELEMENT_CONTENT,
        TEMPLATE_BLOCK,
        CUSTOM_BLOCK,
        IMPORT_BLOCK,
        NAMESPACE_BLOCK,
        CONFIGURATION_BLOCK,
        ORIGIN_BLOCK,
        UNKNOWN
    };
    
    BlockType detectBlockType(const std::string& content, const SourceLocation& location);
    std::string getBlockTypeString(BlockType type);
};

} // namespace CHTL

#endif // CHTL_UNIFIED_SCANNER_HPP