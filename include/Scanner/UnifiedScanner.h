#pragma once

#include "../Core/Types.h"
#include "../Core/Token.h"
#include "../Core/Context.h"
#include "../Core/State.h"
#include <queue>
#include <regex>

namespace CHTL {
namespace Scanner {

// 扫描器配置
struct ScannerConfig {
    size_t maxFragmentSize = 8192;     // 最大片段大小
    size_t minFragmentSize = 64;       // 最小片段大小
    size_t expandSize = 256;           // 扩增大小
    bool enableDebug = false;          // 调试模式
    bool preserveWhitespace = true;    // 保留空白字符
    bool generateTokens = true;        // 生成tokens
};

// 扫描状态
struct ScanState {
    size_t position = 0;               // 当前位置
    size_t line = 1;                   // 当前行号
    size_t column = 1;                 // 当前列号
    Core::ScannerState state = Core::ScannerState::NORMAL;
    Core::String currentFragment;      // 当前片段
    Core::FragmentType fragmentType = Core::FragmentType::UNKNOWN;
    bool inQuote = false;              // 是否在引号中
    char quoteChar = '\0';             // 引号字符
    int braceLevel = 0;                // 花括号嵌套层级
    int parenLevel = 0;                // 圆括号嵌套层级
    int bracketLevel = 0;              // 方括号嵌套层级
};

// CHTL统一扫描器
class CHTLUnifiedScanner {
public:
    explicit CHTLUnifiedScanner(const ScannerConfig& config = ScannerConfig{});
    ~CHTLUnifiedScanner() = default;

    // 主要扫描接口
    Core::CodeFragmentList scanSource(const Core::String& source, 
                                     const Core::String& filename = "");
    
    // 扫描单个片段
    Core::CodeFragmentPtr scanFragment(const Core::String& content, 
                                      Core::FragmentType type,
                                      const Core::Position& startPos);
    
    // Token生成
    std::vector<Core::TokenPtr> tokenizeFragment(const Core::CodeFragmentPtr& fragment);
    std::vector<Core::TokenPtr> tokenizeCHTL(const Core::String& content, 
                                            const Core::Position& startPos);
    std::vector<Core::TokenPtr> tokenizeCHTLJS(const Core::String& content, 
                                              const Core::Position& startPos);
    
    // 片段类型检测
    Core::FragmentType detectFragmentType(const Core::String& content, 
                                         size_t startPos = 0) const;
    
    // 精准切割功能
    bool needsExpansion(const Core::String& content, size_t cutPos) const;
    size_t findOptimalCutPosition(const Core::String& content, 
                                 size_t startPos, size_t maxPos) const;
    size_t expandFragment(const Core::String& source, size_t startPos, 
                         size_t currentEnd) const;
    
    // 最小单元切割
    std::vector<Core::String> splitToMinimalUnits(const Core::String& content, 
                                                 Core::FragmentType type) const;
    
    // 上下文感知合并
    Core::CodeFragmentList mergeContextualFragments(
        const std::vector<Core::String>& units, 
        Core::FragmentType type,
        const Core::Position& startPos) const;
    
    // 配置管理
    void setConfig(const ScannerConfig& config) { config_ = config; }
    const ScannerConfig& getConfig() const { return config_; }
    
    // 状态管理
    void reset();
    const ScanState& getCurrentState() const { return state_; }
    
    // 错误处理
    const Core::ErrorList& getErrors() const { return errors_; }
    bool hasErrors() const { return !errors_.empty(); }
    void clearErrors() { errors_.clear(); }

private:
    ScannerConfig config_;
    ScanState state_;
    Core::ErrorList errors_;
    
    // 正则表达式模式
    std::regex chtlBlockPattern_;
    std::regex chtlJsPattern_;
    std::regex cssBlockPattern_;
    std::regex jsBlockPattern_;
    std::regex selectorPattern_;
    std::regex commentPattern_;
    
    // 初始化
    void initializePatterns();
    
    // 扫描辅助函数
    void updatePosition(char ch);
    void addError(const Core::String& message, const Core::Position& pos);
    
    // 片段处理
    Core::CodeFragmentPtr createFragment(Core::FragmentType type, 
                                        const Core::String& content,
                                        const Core::Position& start,
                                        const Core::Position& end) const;
    
    // 字符分类
    bool isWhitespace(char ch) const;
    bool isAlpha(char ch) const;
    bool isDigit(char ch) const;
    bool isAlnum(char ch) const;
    bool isIdentifierStart(char ch) const;
    bool isIdentifierPart(char ch) const;
    
    // CHTL特殊语法检测
    bool isCHTLKeyword(const Core::String& word) const;
    bool isCHTLJSKeyword(const Core::String& word) const;
    bool isCHTLSelector(const Core::String& content, size_t pos) const;
    bool isCHTLBlock(const Core::String& content, size_t pos) const;
    
    // 括号匹配
    bool isOpenBracket(char ch) const;
    bool isCloseBracket(char ch) const;
    char getMatchingBracket(char ch) const;
    bool areBracketsBalanced(const Core::String& content) const;
    
    // 字符串处理
    bool isStringDelimiter(char ch) const;
    size_t skipString(const Core::String& content, size_t pos) const;
    size_t skipComment(const Core::String& content, size_t pos) const;
    
    // 片段边界检测
    bool isFragmentBoundary(const Core::String& content, size_t pos, 
                           Core::FragmentType currentType) const;
    bool isValidCutPosition(const Core::String& content, size_t pos, 
                           Core::FragmentType type) const;
    
    // CHTL语法单元
    struct CHTLUnit {
        Core::String content;
        Core::TokenType type;
        size_t startPos;
        size_t endPos;
    };
    
    std::vector<CHTLUnit> extractCHTLUnits(const Core::String& content) const;
    std::vector<CHTLUnit> extractCHTLJSUnits(const Core::String& content) const;
    
    // Token生成辅助
    Core::TokenPtr createToken(Core::TokenType type, const Core::String& text, 
                              const Core::Position& pos);
    Core::TokenPtr scanIdentifier(const Core::String& content, size_t& pos, 
                                 const Core::Position& startPos);
    Core::TokenPtr scanNumber(const Core::String& content, size_t& pos, 
                             const Core::Position& startPos);
    Core::TokenPtr scanString(const Core::String& content, size_t& pos, 
                             const Core::Position& startPos);
    Core::TokenPtr scanComment(const Core::String& content, size_t& pos, 
                              const Core::Position& startPos);
    
    // 调试输出
    void debugLog(const Core::String& message) const;
};

// 扫描器工厂
class ScannerFactory {
public:
    static std::unique_ptr<CHTLUnifiedScanner> createScanner(
        const ScannerConfig& config = ScannerConfig{});
    
    static ScannerConfig getDefaultConfig();
    static ScannerConfig getDebugConfig();
    static ScannerConfig getOptimizedConfig();
};

} // namespace Scanner
} // namespace CHTL